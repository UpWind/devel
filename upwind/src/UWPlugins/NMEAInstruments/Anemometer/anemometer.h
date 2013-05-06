#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#include <QGroupBox>
#include "../CoreNMEAInstrument/corenmeainstrument.h"

#include "ui_anemometer.h"

namespace Ui {
class Anemometer;
}
/** Class for the Anemometer Instrument
  */
class Anemometer : public CoreNMEAInstrument
{
    Q_OBJECT
public:
    /** Constructor of the Anemometer Instrument
      * @param parent - Parent widget of the plugin widget.
      */
    explicit Anemometer(QWidget *parent = 0);

    /** Destructor of Anemometer Instrument
      */
    ~Anemometer();

    /** Parse the NMEA string if it is a Anomemeter NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);

    /** Get the name of the instrument
      * @return the name in a QString
      */
    QString getName();

private:
    QPoint knotsPosition;
    QPoint msPosition;
    int halfSizeImage;
    Ui::Anemometer ui;
    QList<qreal> sizes;
    int index;
    QGraphicsScene *anemometerScene;
    QGraphicsPixmapItem* anemometer;
    QGraphicsPixmapItem* hand;
    int m_angle;
    QGraphicsSimpleTextItem *knots;
    QGraphicsSimpleTextItem *ms;

    /** Add the values of the attributes to the QStringList of NMEAValues
      * @param angle - angle of the wind
      * @param windSpeedMS - wind speed in metres per second
      * @param windSpeedKnots - wind speed in knots
      */
    void setParsedNMEAValues(QString m_angle,  QString windSpeedMS, QString windSpeedKnots);

    /** Changes the size of the widget with the value of sizes[]
      */
    void changeWidgetSize();

    /** Load the images of the Anomometer widget
     */
    void initializeImages();

public slots:
    /** Set the angle of the wind
      */
    void setAngle(int m_angle);

    /** Change the size of the widget calling the private method changeWidgetSize
      */
    void changeSize();
};

#endif // ANEMOMETER_H
