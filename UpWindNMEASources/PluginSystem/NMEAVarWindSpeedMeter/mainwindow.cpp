#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include<QDial>
#include<QDebug>
#include<QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //qDebug()<<"main window"<<time;
     //this->setMinimumWidth(700);
    //this->setFixedSize(sizeHint());
    //this->setm
    ui->setupUi(this);
    i=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::valuefromline(qreal p)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug()<<"mainwindow"<<p;
}

void MainWindow::getWindSpeed(qreal windspeed)
{
    qDebug() << __PRETTY_FUNCTION__;
   // qDebug()<<"mainwindow"<<windspeed;
    emit passToPlugin(windspeed);
}

//QDial* MainWindow::getDialValue()

//{
//    qDebug()<<"returned dialed value at the position"<<i;
//    return list.at(i++);
//}

void MainWindow::passSimulationTimeToWindow(int time)
{
    qDebug()<<"simulation time"<<time;
    view = new MyQGraphicsView(this);
    view->passSimulationTime(time);
    connect(view,SIGNAL(passToMainWindow(qreal))
            ,this,SLOT(getWindSpeed(qreal)));
   // connect(view,SIGNAL(passToMainWindowDials(QList<QDial *>))
     //                   ,this,SLOT(getwinddials(QList<QDial *>)));
    connect(view,SIGNAL(sendDialValueToWindow(QDial*)),this,SLOT(SendDialValueToPlugin(QDial*)));

    /*  v= new MyQGraphicsView();*/

    QPushButton *button = new QPushButton("Start Timer",this);
    QVBoxLayout *layout= new QVBoxLayout;

    layout->addWidget(view);

//    horizontalGroupBox = new QGroupBox(tr("Wind angles"));
//    horizontalGroupBoxspinboxes = new QGroupBox();

//    QHBoxLayout *hboxlayout = new QHBoxLayout;
//    QHBoxLayout *hboxlayoutspinboxes = new QHBoxLayout;

//    //QScrollArea *scroll=new QScrollArea();

//    for(int i=0;i<50;i++)
//    {
//        QDial *dial=  new QDial();

//        QSize size(25,25);
//        dial->setMinimumSize(size);
//        dial->setMaximumSize(size);
//        dial->setNotchesVisible(1);
//        //dial->setWrapping(1);
//        dial->setMaximum(359);
//        dial->setMinimum(0);
//        list.append(dial);
//        //list.append(dial);

//        QSpinBox * spin = new QSpinBox(this);
//        //    //QLabel *label = new QLabel(this);
//        //    //spinboxlist[i]= new QSpinBox(this);
//        spin->setMaximum(359);
//        spin->setMinimum(0);
//        spinboxlist.append(spin);

//        connect(list.at(i),SIGNAL(valueChanged(int)),spinboxlist.at(i),SLOT(setValue(int)));

//        connect(spinboxlist.at(i),SIGNAL(valueChanged(int)),list.at(i),SLOT(setValue(int)));
//        //int value= list.at(i)->value();
//        //qDebug()<<"dial value "<<value;
//        //connect(list.at(i),SIGNAL(valueChanged(int)),view,SLOT(setValuesOfList(int)));
//        hboxlayout->addWidget(list[i]);
//        hboxlayoutspinboxes->addWidget(spinboxlist[i]);
//        hboxlayoutspinboxes->addSpacing(0);
//qDebug()<<"dial width"<<dial->width();
//        //hboxlayout->addWidget(spinboxlist[i]);
//    }
//    horizontalGroupBox->setLayout(hboxlayout);
//    //scroll->setWidget(horizontalGroupBox);
//    horizontalGroupBoxspinboxes->setLayout(hboxlayoutspinboxes);

//    layout->addWidget(horizontalGroupBox);
//    //layout->addWidget(scroll);
//    layout->addWidget(horizontalGroupBoxspinboxes);
//    // QPushButton *dialbutton = new QPushButton("Dial values are ready",this);
//    //layout->addWidget(dialbutton);

    layout->addWidget(button);
    QWidget  *widget= new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle("Variable Wind speeds");
    //view->setValuesOfList(list);
    //connect(button,SIGNAL(clicked()),scene,SLOT(startTimer(100));
    connect(button,SIGNAL(clicked()),view,SLOT(startTimer()));

    // connect(dialbutton,SIGNAL(clicked()),this,SLOT(passListToView()));
    //connect(timer,SIGNAL(timeout()),scene,SLOT(advance()));

}
//void MainWindow::getwinddials(QList<QDial *> list)
//{
//    qDebug()<<"got list from view to mainwindow";

//    this->list=list;
//    qDebug()<<"first value in list"<<list.at(0)->value();
//}

void MainWindow::SendDialValueToPlugin(QDial *dial)
{
    qDebug()<<"got Dial Value from view"<<dial->value();
    emit SendDialToPlugin(dial);


}
