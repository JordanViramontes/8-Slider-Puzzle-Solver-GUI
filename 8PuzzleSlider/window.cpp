#include "window.h"
#include "puzzlevisual.h"

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
#include <QApplication>
#include <QStackedLayout>
#include <QStackedWidget>
#include <vector>

using std::vector, std::shared_ptr;

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
    puzzleInputMainWidget = createPuzzleInput();
    QWidget *searchInputWidget = createSearchInput();

    puzzleLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    puzzleGraphicsWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    puzzleInputMainWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    searchInputWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *leftColumn = new QVBoxLayout(this);
    leftColumn->addWidget(puzzleGraphicsWidget, 0);
    leftColumn->addWidget(puzzleLabel, 1);
    leftColumn->addWidget(puzzleInputMainWidget, 3);
    leftColumn->addWidget(searchInputWidget, 4);

    // right column
    outputBox = createOutputBox();
    scrollBox = createScrollBox();
    QLabel *scrollHeader = new QLabel(tr("Optimal Route Output:"), this);

    outputBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    scrollBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollHeader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    scrollHeader->setAlignment(Qt::AlignLeft);

    QVBoxLayout *rightColumn = new QVBoxLayout(this);
    rightColumn->addWidget(outputBox, 0);
    rightColumn->addWidget(scrollHeader, 1);
    rightColumn->addWidget(scrollBox, 2);

    rightColumn->setStretch(1, 0);


    // fix columns
    QFrame *line; // middle line
    line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);

    layout->addLayout(leftColumn, 0, 0);
    layout->addWidget(line, 0, 1, -1, 1); // middle line
    layout->addLayout(rightColumn, 0, 2);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 10);

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
    QFont labelFont( "Arial", 10, QFont::Bold);
    cellSize = 25;

    int vectorCount = 0;
    for (int i = 0; i < dimentionSize; i++) {
        for (int j = 0; j < dimentionSize; j++, vectorCount++) {
            QLabel *label = new QLabel(this);
            if (puzzleVec.at(vectorCount) == 0) label->setText("");
            else label->setText(QString::number(puzzleVec.at(vectorCount)));

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
        if (puzzleVec.at(i) == 0) { puzzleGraphicsLabels.at(i)->setText(""); }
        else { puzzleGraphicsLabels.at(i)->setText(QString::number(puzzleVec.at(i))); }
        // puzzle
    }

    puzzleLabel->setText(string);
    puzzleLabel->update();
    puzzleGraphicsWidget->update();
}

QWidget *Window::createPuzzleInput() {
    // main group box and layout
    QGroupBox *box = new QGroupBox(tr("Input puzzle. \'0\' means a blank space: "), this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    inputType = 0;

    //widgets
    QWidget *radioButtons = createInputRadioButtons();
    QWidget *inputText = createInputLineTextLayout();
    inputText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QWidget *pushButtons = createPushButtons();

    //layout
    layout->addWidget(radioButtons);
    layout->addWidget(inputText);
    layout->addWidget(pushButtons);

    box->setLayout(layout);
    return box;
}

QWidget *Window::createInputRadioButtons() {
    // buttons
    QRadioButton *radio1 = new QRadioButton(tr("Single Input"), this);
    QRadioButton *radio2 = new QRadioButton(tr("Multiple Inputs"), this);
    radio1->setChecked(true);

    // button group
    QButtonGroup *buttons = new QButtonGroup(this);
    buttons->addButton(radio1, 0);
    buttons->addButton(radio2, 1);

    // connection
    connect(buttons, SIGNAL(idClicked(int)),
            this, SLOT(updateInputType(int)));

    // layout and return
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(radio1);
    layout->addWidget(radio2);

    QWidget *w = new QWidget(this);
    w->setLayout(layout);
    return w;
}

QWidget *Window::createInputLineTextLayout() {
    inputWidgetType = new QStackedWidget(this);
    inputWidgetType->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // single line
    QLineEdit *singleLine = new QLineEdit(this);
    singleLine->setPlaceholderText("default: 123456780");
    singleLine->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    inputTextBoxes.push_back(singleLine);

    QSpacerItem *singleSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    QVBoxLayout *singleLayout = new QVBoxLayout(this);
    singleLayout->addWidget(singleLine);
    singleLayout->addItem(singleSpacer);
    QWidget *singleWidget = new QWidget(this);
    singleWidget->setLayout(singleLayout);

    inputWidgetType->addWidget(singleWidget);

    // multi lines
    QLineEdit *mult1 = new QLineEdit(this);
    mult1->setPlaceholderText("default: 123");
    QLineEdit *mult2 = new QLineEdit(this);
    mult2->setPlaceholderText("default: 456");
    QLineEdit *mult3 = new QLineEdit(this);
    mult3->setPlaceholderText("default: 780");

    mult1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    mult2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    mult3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    inputTextBoxes.push_back(mult1);
    inputTextBoxes.push_back(mult2);
    inputTextBoxes.push_back(mult3);

    QLabel *row1 = new QLabel(tr("Row 1:"), this);
    QLabel *row2 = new QLabel(tr("Row 2:"), this);
    QLabel *row3 = new QLabel(tr("Row 3:"), this);

    QGridLayout *multLayout = new QGridLayout(this);
    multLayout->addWidget(row1, 0, 0);
    multLayout->addWidget(mult1, 0, 1);
    multLayout->addWidget(row2, 1, 0);
    multLayout->addWidget(mult2, 1, 1);
    multLayout->addWidget(row3, 2, 0);
    multLayout->addWidget(mult3, 2, 1);

    QWidget *multWidget = new QWidget(this);
    multWidget->setLayout(multLayout);

    inputWidgetType->addWidget(multWidget);
    inputWidgetType->setCurrentIndex(0);

    return inputWidgetType;
}

QWidget *Window::createPushButtons() {
    // buttons
    QPushButton *updatePush = new QPushButton(tr("Update Puzzle"), this);

    QPushButton *startPush = new QPushButton(tr("Start Search"), this);

    // connections
    connect(updatePush, SIGNAL(clicked(bool)),
            this, SLOT(updatePuzzle()));

    connect(startPush, SIGNAL(clicked(bool)),
            this, SLOT(startSearch()));

    // layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(updatePush);
    layout->addWidget(startPush);

    QWidget *w = new QWidget(this);
    w->setLayout(layout);
    return w;
}

QWidget *Window::createInputPuzzleText() {
    QLineEdit *line = new QLineEdit(this);
    return line;
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
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    scrollAreaOutput = new QGridLayout(this);

    QWidget *w = new QWidget(this);
    w->setLayout(scrollAreaOutput);
    scroll->setWidget(w);

    return scroll;
}



// connections

void Window::updatePuzzle() {
    // initialize error message and new puzzle vector
    vector<QString> errorMessageVec;
    vector<int> newPuzzle;
    bool isInputFine = true;

    // determine validity based on input
    switch(inputType) {
        case(0): // single input
            for (unsigned int i = 0; i < inputTextBoxes.at(0)->text().size(); i++) {
                // push inputPuzzle character into new vector
                newPuzzle.push_back(inputTextBoxes.at(0)->text().at(i).digitValue());
            }

            isInputFine = isInputValid(newPuzzle, errorMessageVec);
            break;
        case(1): // multiple inputs
            for (unsigned int i = 1; i < inputTextBoxes.size(); i++) {
                for (unsigned int j = 0; j < inputTextBoxes.at(i)->text().size(); j++) {
                    newPuzzle.push_back(inputTextBoxes.at(i)->text().at(j).digitValue());
                }
            }

            isInputFine = isInputValid(newPuzzle, errorMessageVec);
            break;
        default:
            qDebug() << "INPUT TYPE INVALID";
            break;
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

bool Window::isInputValid(vector<int> newPuzzle, vector<QString> &errorMessageVec) {
    bool isInputFine = true;
    bool isInvalidChar = false;     // used for error handling
    bool isDuplicateChar = false;   // used for error handling

    if (newPuzzle.size() != puzzleSize) {
        qDebug() << newPuzzle.size() << ", " << puzzleSize;
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

    return isInputFine;
}

void Window::updateSearchType(int id) {
    searchType = id;
}

void Window::updateInputType(int id) {
    inputType = id; // new input styles

    switch(id) {
    case(0): // single input line
        inputWidgetType->setCurrentIndex(0);
        puzzleInputMainWidget->update();
        break;
    case(1): // multiple input lines
        inputWidgetType->setCurrentIndex(1);
        puzzleInputMainWidget->update();
        break;
    default:
        qDebug() << "ERROR WHEN UPDATING INPUT TYPE";
        break;
    }


}


void Window::startSearch() {
    // set searching icon
    outputBoxLabels.at(0)->setText("Searching for goal...");
    outputBoxLabels.at(1)->setText("");
    outputBox->update();
    QApplication::setOverrideCursor(Qt::WaitCursor);


    // prepare vector for graph
    vector<vector<int>> finalVec;
    int vectorCnt = 0;
    for (int i = 0; i < dimentionSize; i++) {
        // push back row
        vector<int> tempVector;
        finalVec.push_back(tempVector);

        // fill the row
        for (int j = 0; j < dimentionSize; j++) {
            if (puzzleVec.at(vectorCnt) == 0) { finalVec.at(i).push_back(9); }
            else { finalVec.at(i).push_back(puzzleVec.at(vectorCnt)); }
            vectorCnt++;
        }
    }

    // create graph and perform search
    Graph g(finalVec, searchType);
    g.ASearch();

    // get final output
    QString searchString = "";
    switch(searchType) {
    case (0):
        searchString = "Uniform Cost Search";
        break;
    case(1):
        searchString = "A* Euclidean Distance Heuristic";
        break;
    case(2):
        searchString = "A* Misplaced Tile Heuristic";
        break;
    default:
        searchString = "ERROR WHEN GETTING SEARCHTYPE";
        break;
    }

    QApplication::restoreOverrideCursor(); // reset mouse cursor
    QString finalOutput = g.getStringOutput();
    outputBoxLabels.at(0)->setText("By using: " + searchString + "...");
    outputBoxLabels.at(1)->setText(finalOutput);
    outputBox->update();

    // Update scroll box area
    delete scrollWidget;
    scrollWidget = createScrollOutput(g.getFinalBoard());
    scrollAreaOutput->addWidget(scrollWidget);

    scrollBox->update();
}

QWidget* Window::createScrollOutput(shared_ptr<Board> finalNode) {
    // core layout
    QVBoxLayout *initLayout = new QVBoxLayout(this);
    QLayout *coreLayout = createScrollOutputHelper(finalNode, initLayout);

    // headers
    QLabel *headerLeft = new QLabel("State:", this);
    QLabel *headerRight = new QLabel("Move:      ", this);
    headerLeft->setAlignment(Qt::AlignHCenter);
    headerRight->setAlignment(Qt::AlignHCenter);

    // line
    QFrame *line; // middle line
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);

    // final
    QGridLayout *finalLayout = new QGridLayout(this);
    finalLayout->addWidget(headerLeft, 0, 0);
    finalLayout->addWidget(headerRight, 0, 3);
    finalLayout->addWidget(line, 1, 0, 1, 4);
    finalLayout->addLayout(coreLayout, 2 ,0, -1, -1);
    finalLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);


    QWidget *w = new QWidget(this);
    w->setLayout(finalLayout);
    return w;
}

QLayout* Window::createScrollOutputHelper(shared_ptr<Board> n, QVBoxLayout *layout) {
    if (n == nullptr) return layout; // base case
    qDebug() << n->getDepth();
    createScrollOutputHelper(n->getParent(), layout);

    PuzzleVisual *testPuzzle = new PuzzleVisual(n, dimentionSize, this);

    // Explored: Yes!
    // G value (Depth) = 1
    // H value = 3
    // F value = 4
    QString testString = "";
    if (n->getExplored()) testString += "Explored: Yes!\n";
    else testString += "Explored: No!\n";
    testString += "G Value (Depth) = " + QString::number(n->getDepth()) + "\n";
    testString += "H Value = " + QString::number(n->getH()) + "\n";
    testString += "F Value = " + QString::number(n->getF());

    QFont bigFont("Sans Serif", 15, QFont::StyleItalic);
    QString testStringAGAIN = "Move Blank:\n";
    QString testString2 = "";

    switch(n->getPreviousMode()) {
    case(-1): // initNode
        testStringAGAIN = "Starting State";
        break;
    case(0): // up
        testString2 += "Up";
        break;
    case(1): // down
        testString2 += "Down";
        break;
    case(2): // left
        testString2 += "Left";
        break;
    case(3): // right
        testString2 += "Right";
        break;
    default:
        qDebug() << "ERROR GETTING MOVE FOR SCROLLBOX";
        break;
    }

    QLabel *moveLabel = new QLabel(testStringAGAIN, this);
    QLabel *secondLabel = new QLabel(testString2, this);
    secondLabel->setFont(bigFont);

    moveLabel->setAlignment(Qt::AlignCenter);
    secondLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *moveLabels = new QVBoxLayout(this);
    moveLabels->setSpacing(0);
    moveLabels->setAlignment(Qt::AlignCenter);
    moveLabels->addWidget(moveLabel);
    if (testString2.size() != 0) moveLabels->addWidget(secondLabel);

    QWidget *labelsWidget = new QWidget(this);
    labelsWidget->setLayout(moveLabels);

    QLabel *testLabel = new QLabel(testString, this);
    testLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QFrame *line; // middle line
    line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);

    QGridLayout *currentLayout = new QGridLayout(this);
    currentLayout->setSpacing(10);
    currentLayout->addWidget(testPuzzle, 0, 0);
    currentLayout->addWidget(testLabel, 0, 1);
    currentLayout->addWidget(line, 0, 2);
    currentLayout->addWidget(labelsWidget, 0, 3);

    layout->addLayout(currentLayout);
    return layout;
}

