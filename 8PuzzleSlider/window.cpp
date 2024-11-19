#include "window.h"

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSizePolicy>
#include <QFrame>
#include <QMessageBox>
#include <QScrollArea>
#include <vector>

Window::Window(QWidget *parent)
    : QWidget{parent}
{
    windowH = 640;
    windowV = 480;
    searchType = 0;
    dimentionSize = 3;
    puzzleSize = dimentionSize * dimentionSize;
    puzzleVec = {1,2,3,4,5,6,7,8,0};

    QGridLayout *grid = createGrid();

    setLayout(grid);
    setFixedSize(windowH, windowV);
}

QGridLayout *Window::createGrid() {
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    //left column
    puzzleGraphicsWidget = createPuzzleGraphics();
    puzzleLabel = createPuzzleLabel();
    QSpacerItem *hSpacer = new QSpacerItem(1, 50);
    QWidget *puzzleInputWidget = createPuzzleInput();
    QWidget *searchInputWidget = createSearchInput();

    puzzleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    puzzleGraphicsWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    puzzleInputWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    searchInputWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *leftColumn = new QVBoxLayout(this);
    leftColumn->addWidget(puzzleGraphicsWidget, 0);
    leftColumn->addWidget(puzzleLabel, 1);
    leftColumn->addItem(hSpacer);
    leftColumn->addWidget(puzzleInputWidget, 3);
    leftColumn->addWidget(searchInputWidget, 4);

    // right column
    outputBox = createOutputBox();
    scrollBox = createScrollBox();

    outputBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    scrollBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *rightColumn = new QVBoxLayout(this);
    rightColumn->addWidget(outputBox, 0);
    rightColumn->addWidget(scrollBox, 1);


    // fix columns
    QFrame *line; // middle line
    line = new QFrame;
    line->setFrameShape(QFrame::VLine);

    layout->addLayout(leftColumn, 0, 0);
    layout->addWidget(line, 0, 1, -1, 1); // middle line
    layout->addLayout(rightColumn, 0, 2);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 5);

    return layout;
}

// left column

QLabel *Window::createPuzzleLabel() {
    // header label
    QString string = getVecString();
    QLabel* label = new QLabel(string, this);
    label->setAlignment(Qt::AlignCenter);

    return label;
}

QString Window::getVecString() {
    QString string = "Current Puzzle is:\n<";

    for (unsigned int i = 0; i < puzzleVec.size()-1; i++) {
        string += QString::number(puzzleVec.at(i)) + ", ";
    }
    string += QString::number(puzzleVec.at(puzzleVec.size()-1)) + ">";
    return string;
}

QWidget *Window::createPuzzleGraphics() {
    // main layout to hold labels and spacing settings
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter);

    // create a label for each spot in the graphics and set it to default puzzle
    // then, add it into the layout
    QFont labelFont( "Arial", 15, QFont::Bold);
    cellSize = 50;

    int vectorCount = 0;
    for (int i = 0; i < dimentionSize; i++) {
        for (int j = 0; j < dimentionSize; j++, vectorCount++) {
            QLabel *label = new QLabel(QString::number(puzzleVec.at(vectorCount)), this);
            label->setFont(labelFont);
            label->setAlignment(Qt::AlignCenter);
            label->setFrameStyle(QFrame::Box | QFrame::Plain);
            label->setLineWidth(1);
            label->setFixedSize(cellSize, cellSize);

            puzzleGraphicsLabels.push_back(label);
            layout->addWidget(label, i+1, j);
        }
    }

    // create widget to return
    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    return widget;
}

void Window::UpdatePuzzleGraphics() {
    // get string for puzzleLabel
    QString string = getVecString();

    // called after puzzleVec has been updated
    for (unsigned int i = 0; i < puzzleVec.size(); i++) {
        puzzleGraphicsLabels.at(i)->setText(QString::number(puzzleVec.at(i)));
        // puzzle
    }

    puzzleLabel->setText(string);
    puzzleLabel->update();
    puzzleGraphicsWidget->update();
}

QWidget *Window::createPuzzleInput() {
    // main group box and layout
    QGroupBox *box = new QGroupBox(tr("Input puzzle. \'0\' means a blank space: "), this);
    QGridLayout *layout = new QGridLayout(this);

    // layout widgets
    inputPuzzleText = new QLineEdit(this);
    inputPuzzleText->setPlaceholderText("default: 123456780");
    inputPuzzleText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QPushButton *updateButton;
    updateButton = new QPushButton(this);
    updateButton->setText("Update Puzzle");
    updateButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QPushButton *startButton;
    startButton = new QPushButton(this);
    startButton->setText("Start Search");
    startButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    // connections when clicking buttons / pressing enter
    connect(inputPuzzleText, SIGNAL(returnPressed()),
            this, SLOT(updatePuzzle()));

    connect(updateButton, SIGNAL(clicked(bool)),
            this, SLOT(updatePuzzle()));

    connect(startButton, SIGNAL(clicked(bool)),
            this, SLOT(startSearch()));

    // finalize layout
    layout->addWidget(inputPuzzleText, 0, 0, 1, 0);
    layout->addWidget(updateButton, 1, 0);
    layout->addWidget(startButton, 1, 1);

    box->setLayout(layout);
    return box;
}

QWidget *Window::createSearchInput() {
    // main group box
    QGroupBox *box = new QGroupBox(tr("Search Type: "), this);

    // buttons and button group
    QRadioButton *radio1 = new QRadioButton(tr("Uniform Cost Search"), this);
    QRadioButton *radio2 = new QRadioButton(tr("A* Euclidean Distance Heuristic"), this);
    QRadioButton *radio3 = new QRadioButton(tr("A* Misplaced Tile Heuristic"), this);
    radio1->setChecked(true);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(radio1, 0);
    buttonGroup->addButton(radio2, 1);
    buttonGroup->addButton(radio3, 2);

    // if button group has a difference, call update serachtype
    connect(buttonGroup, SIGNAL(idClicked(int)),
            this, SLOT(updateSearchType(int)));

    // layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    layout->addWidget(radio3);

    box->setLayout(layout);
    return box;
}



// right column

QWidget *Window::createOutputBox() {
    // main visual box and vertical layout
    QGroupBox *box = new QGroupBox(tr("Output:"), this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // labels
    QLabel *headerLabel = new QLabel(tr("Press \'Start Search\' button to start"), this);
    QLabel *outputLabel = new QLabel(this);


    // headerLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    headerLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    outputLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    // add labels to vector
    outputBoxLabels.push_back(headerLabel);
    outputBoxLabels.push_back(outputLabel);

    // add layout
    layout->addWidget(headerLabel, 0);
    layout->addWidget(outputLabel, 1);

    box->setLayout(layout);
    return box;
}

QWidget *Window::createScrollBox() {
    QScrollArea *scroll = new QScrollArea;
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return scroll;
}



// connections

void Window::updatePuzzle() {
    // initialize error message and new puzzle vector
    bool isInputFine = true;
    bool isInvalidChar = false;     // used for error handling
    bool isDuplicateChar = false;   // used for error handling
    vector<QString> errorMessageVec;
    vector<int> newPuzzle;
    for (unsigned int i = 0; i < inputPuzzleText->text().size(); i++) {
        // push inputPuzzle character into new vector
        newPuzzle.push_back(inputPuzzleText->text().at(i).digitValue());
    }

    // check if input is valid
    //input of nothing
    if (newPuzzle.size() != 9) {
        errorMessageVec.push_back("Invalid input size");
        isInputFine = false;
    }

    // iterate over vector
    for (unsigned int i = 0; i < newPuzzle.size(); i++) {
        // check character
        if (newPuzzle.at(i) < 0 || newPuzzle.at(i) > 8) {
            if (!isInvalidChar) {
                errorMessageVec.push_back("Invalid characer inputted");
                isInvalidChar = true;
            }
            isInputFine = false;
        }

        // check duplicates
        bool charCounter = false;
        for (unsigned int j = 0; j < newPuzzle.size(); j++) {
            if (newPuzzle.at(i) == newPuzzle.at(j)) {
                // one way flag, if character is counted twice than isinputfine is false
                if (charCounter) {
                    if (!isDuplicateChar) {
                        errorMessageVec.push_back("Duplicate characters inputted");
                        isDuplicateChar = true;
                    }
                    isInputFine = false;
                }

                charCounter = true;
            }
        }
    }

    // check for solved puzzle
    vector<int> tempCheck = {1,2,3,4,5,6,7,8,0};
    if (newPuzzle == tempCheck) {
        errorMessageVec.push_back("Inputted puzzle already solved!");
        isInputFine = false;
    }

    // if check went through
    if (isInputFine) {
        puzzleVec = newPuzzle;
        UpdatePuzzleGraphics();
    }
    else {
        QString finalError = "";
        for (unsigned int i = 0; i < errorMessageVec.size()-1; i++) {
            finalError += errorMessageVec.at(i) + ", ";
        }
        finalError += errorMessageVec.at(errorMessageVec.size()-1);

        QMessageBox errorBox;
        errorBox.setWindowTitle("Error Inputting Puzzle");
        errorBox.setText(finalError);
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.exec();
    }
}

void Window::updateSearchType(int id) {
    searchType = id;
    qDebug() << "Updating search type to: " << searchType;
}

void Window::startSearch() {
    // test strings:
    QString outputString = "The total amount of nodes expanded were: 1\n";
    outputString += "The total amount of nodes in the queue were: 4.\nThe depth of the goal was: 2.";

    outputBoxLabels.at(0)->setText("By using: Uniform Cost Search...");
    outputBoxLabels.at(1)->setText(outputString);
    outputBox->update();
}

