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
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    //left column
    puzzleGraphicsWidget = createPuzzleGraphics();
    QWidget *puzzleInputWidget = createPuzzleInput();
    QWidget *searchInputWidget = createSearchInput();

    puzzleGraphicsWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    puzzleInputWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    searchInputWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    layout->addWidget(puzzleGraphicsWidget, 0, 0);
    layout->addWidget(puzzleInputWidget, 1, 0);
    layout->addWidget(searchInputWidget, 2, 0);

    // right column
    QFrame *line;
    line = new QFrame;
    line->setFrameShape(QFrame::VLine);

    QGroupBox *temp = new QGroupBox();
    temp->setFixedSize(300, 300);

    layout->addWidget(line, 0, 1, 0, -1);
    layout->addWidget(temp, 0, 2);

    return layout;
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
            layout->addWidget(label, i, j);
        }
    }

    // create widget to return
    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    return widget;
}

void Window::UpdatePuzzleGraphics() {
    // called after puzzleVec has been updated
    for (unsigned int i = 0; i < puzzleVec.size(); i++) {
        puzzleGraphicsLabels.at(i)->setText(QString::number(puzzleVec.at(i)));
    }

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

    // connections when clicking buttons
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
    qDebug() << "starting search";
}

