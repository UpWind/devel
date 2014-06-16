#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtWidgets>

#include "mygraphicsview.h"
#include "myline.h"

//class MyQGraphicsScene;
class QGraphicsView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //MyQGraphicsScene *scene;
//    QGroupBox *horizontalGroupBox;
//    QGroupBox *horizontalGroupBoxspinboxes;
  //  QList<QSpinBox *> spinboxlist;
    //QGraphicsView *view;

public slots:
    //void passListToView();//so this is also not necessary
    void valuefromline(qreal);
    void getWindSpeed(qreal windspeed);
    //void getwinddials(QList<QDial*> list);
    void SendDialValueToPlugin(QDial* dial);
signals:
    void passToPlugin(qreal);
    void SendDialToPlugin(QDial*);
public:
    MyQGraphicsView *view;//i think it is not necessary to pass dials to view
    MyQGraphicsView *line;//to access windspeed elements

public:
//    QDial *getDialValue();
    void passSimulationTimeToWindow(int);
private:
       QList<QDial *> list;//to access this list from plugin this list is kept as public
       int i;
};

#endif // MAINWINDOW_H
