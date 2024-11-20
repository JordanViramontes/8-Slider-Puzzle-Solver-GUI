#ifndef PUZZLEVISUAL_H
#define PUZZLEVISUAL_H

#include <QWidget>
#include <memory>

#include "graph.h"

class PuzzleVisual : public QWidget
{
    Q_OBJECT
private:
    int cellSize;
    int dimentionSize;
    std::shared_ptr<Board> node;
    QLayout *createPuzzle();

public:
    explicit PuzzleVisual(QWidget *parent = nullptr);
    PuzzleVisual(std::shared_ptr<Board> n, int dimSize, QWidget *parent = nullptr);

signals:
};

#endif // PUZZLEVISUAL_H
