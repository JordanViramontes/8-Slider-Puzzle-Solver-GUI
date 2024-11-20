#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
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
    int puzzleSize;
    int dimentionSize;
    int searchType;
    vector<int> puzzleVec;

    // main grid
    QGridLayout *createGrid();

    // left column
    QLabel *puzzleLabel;
    QWidget *puzzleGraphicsWidget;
    QLineEdit *inputPuzzleText;
    vector<QLabel*> puzzleGraphicsLabels;
    int cellSize;

    QLabel *createPuzzleLabel();
    QWidget *createPuzzleGraphics();
    QWidget *createPuzzleInput();
    QWidget *createSearchInput();
    void UpdatePuzzleGraphics();
    QString getVecString();


    // right column
    QWidget *outputBox;
    QWidget *scrollBox;
    vector<QLabel*> outputBoxLabels;

    QWidget *createOutputBox();
    QWidget *createScrollBox();


private slots:
    void updatePuzzle();
    void updateSearchType(int id);
    void startSearch();

signals:
};

#endif // WINDOW_H
