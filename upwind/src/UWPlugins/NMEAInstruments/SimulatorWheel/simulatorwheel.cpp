#include <qmath.h>

#include "../../shared/uwmath.h"
#include "simulatorwheel.h"
#include "ui_simulatorwheel.h"

const static double METER_TO_KNOT = 1.94384449;
const float handleCenterRadiusSquared = 900.0;

SimulatorWheel::SimulatorWheel(QWidget *parent)
    : m_halfSizeImage(0)
    ,m_index(0)
    ,m_simulatorWheelScene(0)
    ,m_simulatorWheel(0)
    ,m_hand(0)
    ,m_angle(0.0)
    ,m_initialAngle(0.0)
    ,m_turning(false)
    ,m_turningFactor(0.0)
{

    this->setParent(parent);
    m_ui.setupUi(this);
    this->move(20, 20);
    m_simulatorWheelScene = new QGraphicsScene();
    this->instrumentView->setScene(m_simulatorWheelScene);
    m_ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    m_sizes << 0.55;
    m_sizes << 0.85;
    m_sizes << 1.15;
    m_index = 0;
    initializeImages();

    setDefaultStyleSheet();
    changeWidgetSize();

    m_angle = 0;
    m_turningFactor = 0.5;

    connect(this->instrumentView, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseDoubleClicked(QMouseEvent*)), this, SLOT(changeSize()));
    this->initializeSettings();
}

SimulatorWheel::~SimulatorWheel(){
    delete m_simulatorWheel;
    delete m_simulatorWheelScene;
}

void SimulatorWheel::changeSize(){
    m_index = (m_index + 1) % 3;
    m_simulatorWheel->setScale(m_sizes[m_index]);
    changeWidgetSize();
}

void SimulatorWheel::mousePressEvent(QMouseEvent *event)
{
    QPoint center = pos() + QPoint(width()/2, height()/2);
    QVector2D distance = QVector2D(event->globalPos() - center);

    qDebug() << Q_FUNC_INFO << distance << handleCenterRadiusSquared << distance.length();
    if (distance.lengthSquared() > handleCenterRadiusSquared) {
        m_turning = true;
        handlingPoint= pos() + QPoint(width()/2, height()/2);
        m_initialAngle = angleFromPoint(event->globalPos()) - m_angle ;
    } else {
        CoreNMEAInstrument::mousePressEvent(event);
    }
}

void SimulatorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    m_turning = false;
    CoreNMEAInstrument::mouseReleaseEvent(event);
}

void SimulatorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_turning) {
        float angle = angleFromPoint(event->globalPos());

        angle = angle - m_initialAngle;

        if (angle > 180.0) {
            angle -= 360.0;
        } else if (angle < -180.0) {
            angle += 360.0;
        }

        if (angle < -160.0) {
            angle = -160.0;
        } else if (angle > 160.0) {
            angle = 160.0;
        }

        this->setAngle(angle);
        if (m_dataSimulatorControlInterface)
            m_dataSimulatorControlInterface->setTurningSpeed(angle * m_turningFactor);
    } else
        CoreNMEAInstrument::mouseMoveEvent(event);
}

float SimulatorWheel::angleFromPoint(const QPoint &point)
{
    float angle = 0.0;
    float vectorX = handlingPoint.x() - point.x();
    float vectorY = handlingPoint.y() - point.y();
    if (vectorX > 0) {
        angle = qAtan(vectorY / vectorX);
    } else if (vectorX < 0 && vectorY >= 0) {
        angle = qAtan(vectorY / vectorX) + M_PI;
    } else if (vectorX < 0 && vectorY < 0) {
        angle = qAtan(vectorY / vectorX) - M_PI;
    } else if (vectorX == 0 && vectorY > 0) {
        angle = M_PI_2;
    } else if (vectorX == 0 && vectorY < 0) {
        angle = -M_PI_2;
    } else {
        angle = 0.0;
    }

    angle = ( angle )/ M_PI * 180.0;

    return angle;
}

void SimulatorWheel::changeWidgetSize(){
    m_simulatorWheelScene->setSceneRect(m_simulatorWheelScene->itemsBoundingRect());
    QSize size;
    size.setHeight(m_simulatorWheelScene->height());
    size.setWidth(m_simulatorWheelScene->width());
    this->resize(size);
}

void SimulatorWheel::initializeImages(){
    QPixmap simulatorWheelPix;
    simulatorWheelPix.load(":steeringWheel");
    m_simulatorWheel = new QGraphicsPixmapItem(simulatorWheelPix);

    m_halfSizeImage = simulatorWheelPix.size().height() / 2;

    m_simulatorWheel->setTransformationMode(Qt::SmoothTransformation);
    m_simulatorWheel->setScale(m_sizes[m_index]);
    m_simulatorWheel->setOffset(-m_halfSizeImage, -m_halfSizeImage);

    m_simulatorWheelScene->addItem(m_simulatorWheel);
}

void SimulatorWheel::connectToSimulator(DataSimulatorControlInterface *simulator, QObject *simulatorObject)
{
    m_dataSimulatorControlInterface = simulator;
    m_dataSimulatorObject = simulatorObject;
}

void SimulatorWheel::setAngle(int angle){
    m_angle = angle;
    m_simulatorWheel->setRotation(m_angle);
}

void SimulatorWheel::setParsedNMEAValues(QString angle, QString windSpeedMS, QString windSpeedKnots){
}

QString SimulatorWheel::getName(){
    return "SimulatorWheelWidget";
}

void SimulatorWheel::parseNMEAString(const QString & text){
}
