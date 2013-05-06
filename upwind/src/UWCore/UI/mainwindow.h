#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void geometryChanged(QRect);

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
