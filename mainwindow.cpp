#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cgameboard.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CGameBoard* board = new CGameBoard(this);
    setCentralWidget(board);

    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), board, SLOT(next()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
