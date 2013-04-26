#ifndef VELOCITYHANDLE_H
#define VELOCITYHANDLE_H

#include <QGroupBox>
#include "../CoreNMEAInstrument/corenmeainstrument.h"

#include "ui_velocityhandle.h"
#include "../../NMEADataSimulator/datasimulatorcontrolinterface.h"
#include "velocityhandleinterface.h"

namespace Ui {
class VelocityHandle;
}
/** Class for the SimulatorWheel Instrument
  */
class VelocityHandle : public CoreNMEAInstrument, public VelocityHandleInterface
{
    Q_OBJECT
public:
    /** Constructor of the SimulatorWheel Instrument
      * @param parent - Parent widget of the plugin widget.
      */
    explicit VelocityHandle(QWidget *parent = 0);

    /** Destructor of SimulatorWheel Instrument
      */
    ~VelocityHandle();

    /** Parse the NMEA string if it is a Anomemeter NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);

    /** Get the name of the instrument
      * @return the name in a QString
      */
    QString getName();

public slots:
    /** Set the angle of the wind
      */
    void setAngle(int m_angle);

    /** Change the size of the widget calling the private method changeWidgetSize
      */
    void changeSize();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QGraphicsPixmapItem* loadImage(const QString& image, int* halfSizeImagePtr = 0);

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

    virtual void connectToSimulator(DataSimulatorControlInterface* simulator, QObject* simulatorObject);

    QPoint handlingPoint;

//    int m_halfSizeImage;
    Ui::SimulatorWheel m_ui;
    QList<qreal> m_sizes;
    int m_index;
    QGraphicsScene *m_velocityHandleScene;
    QGraphicsPixmapItem* m_velocityHandle;
    QGraphicsPixmapItem* m_handle;
    QGraphicsPixmapItem* m_anchorOn;
    QGraphicsPixmapItem* m_anchorOff;

    QRectF m_handleRect;

    float m_angle;
    float m_initialAngle;
    bool m_turning;
    float m_turningFactor;

    DataSimulatorControlInterface* m_dataSimulatorControlInterface;
    QObject* m_dataSimulatorObject;
//    QGraphicsSimpleTextItem *knots;
//    QGraphicsSimpleTextItem *ms;

};

#endif // VELOCITYHANDLE_H
