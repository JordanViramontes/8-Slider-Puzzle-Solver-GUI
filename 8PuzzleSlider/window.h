#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QStackedWidget>
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
    QLabel *puzzleLabel; //label showing puzzleVec
    QWidget *puzzleGraphicsWidget; //main widget for graphics
    vector<QLabel*> puzzleGraphicsLabels;
    int cellSize;
    int inputType;

    // input widget
    QWidget *puzzleInputMainWidget;
    QStackedWidget *inputWidgetType;
    vector<QLineEdit*> inputTextBoxes;
    QLayout *inputLinesLayout;

    QWidget *createPuzzleInput();
    QWidget *createInputRadioButtons();
    QWidget *createInputLineTextLayout();
    QWidget *createPushButtons();

    // rest of the widgets
    QLabel *createPuzzleLabel();
    QWidget *createPuzzleGraphics();
    QWidget *createInputPuzzleText();
    QWidget *createSearchInput();
    void UpdatePuzzleGraphics();
    QString getVecString();


    // right column
    QWidget *outputBox;
    QWidget *scrollBox;
    vector<QLabel*> outputBoxLabels;

    QWidget *createOutputBox();
    QWidget *createScrollBox();

    // additional helper functions
    bool isInputValid(vector<int> newPuzzle, vector<QString> &errorMessageVec);


private slots:
    void updatePuzzle();
    void updateSearchType(int id);
    void updateInputType(int id);
    void startSearch();

signals:
};

#endif // WINDOW_H
