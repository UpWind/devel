#include "myline.h"
#include <QDebug>

MyLine::MyLine()
{
    adv=0;
    timer=1;
    //    savetimervalue=0;
    interval= "start";
  start=true;
    // setPos(mapToScene(4,0));
}

//MyLine::MyLine(QGraphicsItem *parent) : QGraphicsItem(parent)
//{
//    adv = 1;
//    timer = 1;
//}

QRectF MyLine::boundingRect() const
{
    //bounding rect is the outer most region of our object
    return QRectF(10,10,4,649);
}

void MyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect=  boundingRect();
    QBrush brush(Qt::gray);

    //basic collission detection
    //scene is going to tell us where the objects are
    //it is going to tell us, is this item colliding with any item
    if(scene()->collidingItems(this).isEmpty())
    {
        //no collission
        brush.setColor(Qt::green);
    }
    else
    {
        //collission
        brush.setColor(Qt::red);
        //set the position
        DoCollision();
    }

    painter->fillRect(rect,brush);
    //painter->drawEllipse();
}

void MyLine::advance(int phase)
{
    qDebug()<<"timer value"<<timer;
    if(timer==10)
    {
        timer = 1;
        interval = "new";
        qDebug()<<"new interval";
        emit sendNewDialValue();
    }
    else//if timer is with in the period
    {
        timer++;


        if(start)//if it is the first period, then get the dial value
        {
        emit sendNewDialValue();
        }
         start=false;//to indicate that it is not starting
    }

    qDebug()<<"1 sec over";

    if(!phase)
    {
        qDebug()<<"entered into phase 0";
        return;
    }

    QPointF location1= (this->pos());
    qDebug()<<"current position";
    qDebug()<<location1.x();
    qDebug()<<location1.y();

    QPointF nextpos;
    nextpos.setX(location1.rx()+8);
    nextpos.setY(location1.y());

    //it maps current x y coordinates to scene's x y coordinates
    //setPos(mapToParent(0,-(speed)));
    setPos(nextpos);

    qDebug()<<"next position";
    qDebug()<<nextpos.x();
    qDebug()<<nextpos.y();
}

void MyLine::DoCollision()
{
    if(interval=="start"||interval=="new")
    {
    qDebug()<<"DO COLLISSION";
   // qDebug()<<"total no of winspeeds in line"<<listofpoint.size();

    qDebug()<<"adv value"<<adv;
    QPointF f = listofpoint[adv];

    adv++;
    //qDebug()<<"y point"<<f.y();

    qreal x;
    //629 is the pixel positin where 1 is pplaced
    //32.45 is the gap between the wind speeds in pixels
    //if the positions of the windspeeds are changed then 629 must be changed
    x= (629- f.y())/32.45;

    qreal windspeed= x+1;

    qDebug()<< "WINDSPEED"<<windspeed;
    windspeeds.append(windspeed);

    // savetimervalue= timer;

    // if(savetimervalue==5)
    // savetimervalue=0;

    interval = "old";
    //qDebug()<<"savetimervalue"<<savetimervalue;

    emit signal(windspeed);
    }

    else
    {
        qDebug()<<"interval"<<interval;
       adv++;//so that we go one step advance of that point in the list which we dont need
       qDebug()<<"missed one point for good";

    }
}

void MyLine::passElements(QList<QPointF> p)
{
    listofpoint = p;
    qDebug()<<"windspeed elements  size in line "<<listofpoint.count();
}
