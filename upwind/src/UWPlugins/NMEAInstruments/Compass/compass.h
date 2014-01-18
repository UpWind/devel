#ifndef COMPASS_H
#define COMPASS_H

#include "ui_compass.h"
#include "../CoreNMEAInstrument/corenmeainstrument.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QFrame>
#include <QPoint>
#include <QString>
#include <QWidget>
#include <QMouseEvent>
#include <QFrame>
#include <QPoint>
#include <QString>
#include <QtGui>

/** Class for the Compass Instrument
  */
class Compass : public CoreNMEAInstrument
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.Compass")

public:
    /** Constructor of the Compass Instrument
      * @param parent - Parent widget of the plugin widget.
      */
    Compass(QWidget *parent = 0);

    /** Destructor of Compass Instrument
      */
    ~Compass();

    /** Parse the NMEA string if it is a Compass NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);
    /** Get the name of the instrument
      * @return the name in a QString
      */
    QString getName();

private:
    QList<qreal> sizes;
    int index;
    Ui::Compass ui;
    QGraphicsScene *compassScene;
    QGraphicsPixmapItem* compass;
    QGraphicsPixmapItem* hand;
    int angle;

    /** Changes the size of the widget with the value of sizes[]
      */
    void changeWidgetSize();

    /** Load the images of the Compass widget
      */
    void initializeImages();

public slots:
    /** Set the angle of the compass
      */
    void setAngle(int angle);

    /** Change the size of the widget calling the private method changeWidgetSize
      */
    void changeSize();
};

#endif // COMPASS_H
