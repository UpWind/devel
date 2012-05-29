#include "mygraphicsview.h"
#include <QPointF>
#include<QDebug>
#include<QPainter>
#include<QPainterPath>
#include<QTimer>
#include"myline.h"
#include<QDial>
#include <QGraphicsProxyWidget>
//#include<QScrollArea>

#include <QObject>

MyQGraphicsView::MyQGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    initialdial=0;
}

//void MyQGraphicsView::paintEvent(QPaintEvent *event)
//{
//    painter = new QPainter(this); // Initialise painter

//    QLinearGradient lGrad(QPointF(500, 350), QPointF(700, 400)); // Set a gradient background
//    lGrad.setColorAt(0, Qt::black);
//    lGrad.setColorAt(1, Qt::darkBlue);
//    painter->fillRect(0, 0, 800, 440, lGrad);

//}

void MyQGraphicsView::passSimulationTime(int simutime)
{
    QPainter painter;
    QColor color(255,0,0);
    painter.fillRect(QRect(0,0,20,20),color);

    qDebug()<<"simulation time in view in minutes"<<simutime;
    int simutimeinseconds= simutime*60;
     noofperiods= simutimeinseconds/5;
    //for each period 40 pixels, so caluclueate for noofperiods how many pixels will take
    int xsizeinpixels=noofperiods*40;
    xsizeinpixels+=100;//just for the arrangement of the items in the scene , allocating some more space

    scene = new QGraphicsScene();

    int x= xsizeinpixels;
    int y= 1000;

    this->setSceneRect(QRectF(-20, -20, x, y));
    this->setScene(scene);
    //scene->setBackgroundBrush(Qt::yellow);


    lineitem = new MyLine();

    connect(lineitem,SIGNAL(signal(qreal)),this,SLOT(windspeedFromLine(qreal)));
    connect(lineitem,SIGNAL(sendNewDialValue()),this,SLOT(SendNewDialValueToWindow()));

    scene->addItem(lineitem);

    ////    QScrollArea *scroll= new QScrollArea();
    ////    scroll->setBackgroundRole(QPalette::Dark);
    ////    scene->addItem(scroll);
    //    //scroll->setHorizontalScrollBarPolicy(Qt::WindowActive);

    //int maxxsize= 700-21-15;
    //int maxysize=700-21-15-15;
    //int gapy= maxysize/20;

    int gapy=32.45;//this is the gap in pixels between the items in y axis


    int gapx=40;
    int xinitialpos=0;
    int ypos=10;
    for(int i=20;i>=1;i--)
    {
        QString str;
        str.setNum(i);

        QGraphicsTextItem *item = new QGraphicsTextItem(str);
        item->setPos(mapToScene(-20,ypos));
        scene->addItem(item);
        ypos+=gapy;
        //maxysize = maxysize-gapy;
    }
    qDebug()<<"max y position"<<ypos;


  QGraphicsTextItem *item1 = new QGraphicsTextItem(tr("wind speeds in knots"));
    item1->setPos(mapToScene(-20,-20));
    scene->addItem(item1);


    for(int time=0;time<=simutimeinseconds;time= time+5)
    {
        QString str;
        str.setNum(time);
        QGraphicsTextItem *item = new QGraphicsTextItem(str);
//        if(time%2==0)
//        {
//        QLinearGradient lGrad(QPointF(xinitialpos, 668), QPointF(xinitialpos, 100));
//        lGrad.setColorAt(0, Qt::black);
//        lGrad.setColorAt(1, Qt::darkBlue);

//        }

        item->setPos(mapToScene(xinitialpos,668));
        scene->addItem(item);

//        QGraphicsLineItem *line = new QGraphicsLineItem();
//        line->setPos(mapToScene(xinitialpos,666));

        xinitialpos = xinitialpos+gapx;
//        QPen pen(Qt::yellow,3,Qt::DashDotLine,Qt::RoundCap,Qt::RoundJoin);

//       scene->addLine(xinitialpos+4,665,xinitialpos+4,20,pen);
    }

    QGraphicsTextItem *item2 = new QGraphicsTextItem(tr("time in seconds"));
    item2->setPos(mapToScene(668,681));
    scene->addItem(item2);

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),scene,SLOT(advance()));



    horizontalGroupBox = new QGroupBox(tr("Wind angles"));
    horizontalGroupBoxspinboxes = new QGroupBox();

    QHBoxLayout *hboxlayout = new QHBoxLayout;
    QHBoxLayout *hboxlayoutspinboxes = new QHBoxLayout;

    //    //QScrollArea *scroll=new QScrollArea();

    for(int i=0;i<noofperiods;i++)
    {
        QDial *dial=  new QDial();

        QSize size(35,35);
        dial->setMinimumSize(size);
        dial->setMaximumSize(size);
        dial->setNotchesVisible(1);
        dial->setWrapping(1);
        dial->setMaximum(359);
        dial->setMinimum(0);
        list.append(dial);
        //        //list.append(dial);

        QSpinBox * spin = new QSpinBox(this);
        //        // //QLabel *label = new QLabel(this);
        //        // //spinboxlist[i]= new QSpinBox(this);
        spin->setMaximum(359);
        spin->setMinimum(0);
        spin->setFixedSize(35,35);
        spinboxlist.append(spin);

        connect(list.at(i),SIGNAL(valueChanged(int)),spinboxlist.at(i),SLOT(setValue(int)));
        connect(spinboxlist.at(i),SIGNAL(valueChanged(int)),list.at(i),SLOT(setValue(int)));
        //        //int value= list.at(i)->value();
        //        //qDebug()<<"dial value "<<value;
        //        //connect(list.at(i),SIGNAL(valueChanged(int)),view,SLOT(setValuesOfList(int)));
        hboxlayout->addWidget(list[i]);
        hboxlayoutspinboxes->addWidget(spinboxlist[i]);
        //        hboxlayoutspinboxes->addSpacing(0);
        //qDebug()<<"dial width"<<dial->width();
        //        //hboxlayout->addWidget(spinboxlist[i]);
    }
    horizontalGroupBox->setLayout(hboxlayout);
    //    //scroll->setWidget(horizontalGroupBox);
    horizontalGroupBoxspinboxes->setLayout(hboxlayoutspinboxes);

    //horizontalGroupBox->set
    QGraphicsProxyWidget *proxy1= scene->addWidget(horizontalGroupBox);
    proxy1->setPos(0,700);

    QGraphicsProxyWidget *proxy2= scene->addWidget(horizontalGroupBoxspinboxes);
    proxy2->setPos(0,760);

    //QGraphicsProxyWidget *proxy1= scene->addWidget(horizontalGroupBoxspinboxes);
    //scene has advance () slot, what it does is it says every object in the scee
    //that it is time to advance one step or multiple steps
    //thast what animation is

}

void MyQGraphicsView::mousePressEvent(QMouseEvent * e)
{
    qreal rad = 1;
    QPointF pt = mapToScene(e->pos());
    //QPointF pt = (e->pos());
    qDebug()<<pt.x();
    qDebug()<<pt.y();
    if(pt.y()>=650)
        return;
    listofpoints.append(pt);
    qDebug()<<listofpoints.size();

    //    path.lineTo(pt.x(),pt.y());
    //  qDebug()<<"element count"<<path.elementCount();

    qDebug()<<"element count"<<listofpoints.size();

    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(pt.x()-rad, pt.y()-rad, rad*3.0, rad*3.0);

    ellipse->setPen(QPen());
    ellipse->setBrush(QBrush(Qt::SolidPattern));

    scene->addItem(ellipse);
}

void MyQGraphicsView::startTimer()
{
    //and in line , in docollision calculation happens for getting the original speed
    lineitem->passElements(listofpoints);//this will pass the wind speedds in pixels to the line
    //emit passToMainWindowDials(list);
    timer->start(1000);
}

void MyQGraphicsView::SendNewDialValueToWindow()
{
    qDebug()<<"In View, 5 sec is over, have to send new dial value to the window";
    qDebug()<<"Index of the dial currently is "<<initialdial;
    if(initialdial<noofperiods)
    emit sendDialValueToWindow(list.at(initialdial++));
}

void MyQGraphicsView::windspeedFromLine(qreal windspeed)
{
   // qDebug()<<"view"<<windspeed;
    emit passToMainWindow(windspeed);
}

