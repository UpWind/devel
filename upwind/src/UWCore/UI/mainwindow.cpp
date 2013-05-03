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

    QStackedLayout *layout = new QStackedLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

MainWindow::~MainWindow(){
    delete ui;
}
