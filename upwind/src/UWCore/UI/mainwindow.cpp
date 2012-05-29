#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
}

MainWindow::~MainWindow(){
    delete ui;
}
