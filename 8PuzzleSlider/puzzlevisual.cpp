#include "puzzlevisual.h"

#include <QGridLayout>
#include <QLabel>

using std::vector, std::shared_ptr;

PuzzleVisual::PuzzleVisual(QWidget *parent)
    : QWidget{parent}
{
    node = nullptr;
}

PuzzleVisual::PuzzleVisual(shared_ptr<Board> n,int dimSize, QWidget *parent) : QWidget{parent} {
    node = n;
    cellSize = 25;
    dimentionSize = dimSize;

    setLayout(createPuzzle());
}

QLayout *PuzzleVisual::createPuzzle() {
    // main layout to hold labels and spacing settings
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter);

    // create a label for each spot in the graphics and set it to default puzzle
    // then, add it into the layout
    // QFont labelFont( "Arial", 10, QFont::Bold); DEFAULT!!!!
    // cellSize = 25;
    QFont labelFont( "Arial", 10);

    // create labels
    for (int i = 0; i < dimentionSize; i++) {
        for (int j = 0; j < dimentionSize; j++) {
            // label
            QLabel *label = new QLabel(this);
            if (node->getVector().at(i).at(j) == 9) label->setText("");
            else label->setText(QString::number(node->getVector().at(i).at(j)));

            label->setFont(labelFont);
            label->setAlignment(Qt::AlignCenter);
            label->setFrameStyle(QFrame::Box | QFrame::Plain);
            label->setLineWidth(1);
            label->setFixedSize(cellSize, cellSize);

            layout->addWidget(label, i, j);
        }
    }

    // create widget to return
    return layout;
}
