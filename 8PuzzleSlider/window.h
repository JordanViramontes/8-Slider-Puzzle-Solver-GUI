#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <vector>

using std::vector;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private:
    // variables
    int windowH;
    int windowV;
    int searchType;
    int puzzleSize;
    int dimentionSize;
    vector<int> puzzleVec;

    // main grid
    QGridLayout *createGrid();

    // left column
    QLineEdit *inputPuzzleText;
    QWidget *puzzleGraphicsWidget;
    vector<QLabel*> puzzleGraphicsLabels;
    int cellSize;

    QWidget *createPuzzleGraphics();
    QWidget *createPuzzleInput();
    QWidget *createSearchInput();
    void UpdatePuzzleGraphics();


    // right column


private slots:
    void updatePuzzle();
    void updateSearchType(int id);
    void startSearch();

signals:
};

#endif // WINDOW_H
