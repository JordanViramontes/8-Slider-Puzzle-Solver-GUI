#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "window.cpp"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Window *window = new Window;
    // QHBoxLayout *layout = new QHBoxLayout(this);
    // layout->addWidget(window);

    setCentralWidget(window);
    setFixedSize(640, 480);
}

MainWindow::~MainWindow()
{
    delete ui;
}
