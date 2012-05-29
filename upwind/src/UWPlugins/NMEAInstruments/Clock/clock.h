#ifndef CLOCK_H
#define CLOCK_H

#include <QGroupBox>
#include <QGraphicsScene>

#include "../CoreNMEAInstrument/corenmeainstrument.h"
#include "ui_clock.h"

namespace Ui {
class Clock;
}

/**
  *Class for the Clock Instrument
  */
class Clock : public CoreNMEAInstrument
{
    Q_OBJECT

public:
    /** Constructor of the Clock Instrument
      * @param parent - Parent widget of the plugin widget.
      */
    explicit Clock(QWidget *parent = 0);

    /** Destructor of Clock Instrument
      */
    ~Clock();

    /** Parse the NMEA string if it is a Compass NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);

    /** Get the name of the instrument
      * @return the name in a QString
      */
    QString getName();

private:
    int halfSizeImage;
    int index;
    Ui::Clock ui;
    QList<qreal> sizes;
    QTimer* timer;
    QSize* originalSize;
    QGraphicsScene *clockScene;
    QLCDNumber* lcd;

    /** Changes the size of the widget with the value of sizes[]
      */
    void changeWidgetSize();

public slots:
    /** Change the size of the widget calling the private method changeWidgetSize
      */
    void changeSize();

private slots:
    /** Refresh the time in the LCD Display
      */
    void showTime();
};

#endif // CLOCK_H
