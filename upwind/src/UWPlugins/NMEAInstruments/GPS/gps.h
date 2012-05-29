#ifndef GPS_H
#define GPS_H

#include <QGroupBox>

#include "../CoreNMEAInstrument/corenmeainstrument.h"
#include "ui_gps.h"

/**
  *Class for the GPS Instrument
  */
class GPS : public CoreNMEAInstrument
{

    Q_OBJECT

public:
    /** Constructor of the GPS Instrument
      * @param parent - Parent widget of the plugin widget.
      */
    explicit GPS(QWidget *parent = 0);

    /** Destructor of GPS Instrument
      */
    ~GPS();

    /** Parse the NMEA string if it is a GPS NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);

    /** Get the name of the instrument
      * @return the name in a QString
      */
    QString getName();

private:
    QPoint latitudePosition;
    QPoint longitudePosition;
    int halfSizeImage;
    Ui::gps ui;
    QList<qreal> sizes;
    int index;
    QGraphicsScene *gpsScene;
    QGraphicsPixmapItem* gps;
    QGraphicsSimpleTextItem *latitude;
    QGraphicsSimpleTextItem *longitude;

    /** Changes the size of the widget with the value of sizes[]
      */
    void changeWidgetSize();

    /** Load the images of the GPS widget
      */
    void initializeImages();

public slots:
    /** Change the size of the widget calling the private method changeWidgetSize
      */
    void changeSize();
};
#endif // GPS_H
