#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include <QStackedLayout>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("UpWind Navigator");
    setWindowIcon(QIcon(":/buttons/Upwind Icon.svg"));
    this->setWindowState(Qt::WindowFullScreen);

#ifdef Q_OS_LINUX
    setGeometry(QApplication::desktop()->screenGeometry());
#endif //Q_OS_LINUX

    QStackedLayout *layout = new QStackedLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit geometryChanged(geometry());
    QWidget::resizeEvent(event);
}
