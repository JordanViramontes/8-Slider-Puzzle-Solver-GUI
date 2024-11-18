#include "window.h"

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSizePolicy>
#include <vector>

Window::Window(QWidget *parent)
    : QWidget{parent}
{
    windowH = 640;
    windowV = 480;
    searchType = 0;
    PuzzleVec = {1,2,3,4,5,6,7,8,0};

    QGridLayout *grid = createGrid();

    setLayout(grid);
    setFixedSize(windowH, windowV);
}

QGridLayout *Window::createGrid() {
    QGridLayout *layout = new QGridLayout(this);

    //left column
    // QWidget *puzzleGraphicsWidget = createPuzzleGraphics();
    QWidget *puzzleInputWidget = createPuzzleInput();
    QWidget *searchInputWidget = createSearchInput();

    layout->addWidget(puzzleInputWidget, 1, 0);
    layout->addWidget(searchInputWidget, 2, 0);


    return layout;
}

QWidget *Window::createPuzzleInput() {
    // main group box and layout
    QGroupBox *box = new QGroupBox(tr("Input puzzle where \'0\' is blank: "), this);
    QGridLayout *layout = new QGridLayout(this);

    // layout widgets
    QLineEdit *inputPuzzleText = new QLineEdit(this);
    inputPuzzleText->setPlaceholderText("default: 123456780");
    inputPuzzleText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    updateButton = new QPushButton(this);
    updateButton->setText("Update Puzzle");
    updateButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    startButton = new QPushButton(this);
    startButton->setText("Start Search");
    startButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);


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
void Window::updateSearchType(int id) {
    searchType = id;
    qDebug() << "Updating search type to: " << searchType;
}



