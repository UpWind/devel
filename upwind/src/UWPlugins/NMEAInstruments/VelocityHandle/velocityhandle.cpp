#include <qmath.h>

#include "../../shared/uwmath.h"
#include "velocityhandle.h"
#include "ui_velocityhandle.h"

const static double METER_TO_KNOT = 1.94384449;
const float wheelCenterRadiusSquared = 900.0;

VelocityHandle::VelocityHandle(QWidget *parent) :
     m_index(0)
    ,m_velocityHandleScene(0)
    ,m_velocityHandle(0)
    ,m_handle(0)
    ,m_anchorOn(0)
    ,m_anchorOff(0)
    ,m_angle(0.0)
    ,m_initialAngle(0.0)
    ,m_turning(false)
    ,m_turningFactor(0.0)
{

    this->setParent(parent);
    m_ui.setupUi(this);
    this->move(20, 20);
    m_velocityHandleScene = new QGraphicsScene();
    this->instrumentView->setScene(m_velocityHandleScene);
    m_ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    m_sizes << 0.55;
    m_sizes << 0.85;
    m_sizes << 1.15;
    m_index = 0;
    initializeImages();

    m_handleRect = QRectF();

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

VelocityHandle::~VelocityHandle(){
    delete m_velocityHandle;
    delete m_velocityHandleScene;
}

void VelocityHandle::changeSize(){
    m_index = (m_index + 1) % 3;
    m_velocityHandle->setScale(m_sizes[m_index]);
    m_handle->setScale(m_sizes[m_index]);
    m_anchorOff->setScale(m_sizes[m_index]);
    m_anchorOn->setScale(m_sizes[m_index]);
    changeWidgetSize();
}

void VelocityHandle::mousePressEvent(QMouseEvent *event)
{
    QPoint center = pos() + QPoint(width()/2, height()/2);
    QVector2D distance = QVector2D(event->globalPos() - center);

    qDebug() << Q_FUNC_INFO << distance << wheelCenterRadiusSquared << distance.length();
//    if (distance.lengthSquared() > wheelCenterRadiusSquared) {
//        m_turning = true;
//        handlingPoint= pos() + QPoint(width()/2, height()/2);
//        m_initialAngle = angleFromPoint(event->globalPos()) - m_angle ;
//    } else {
        CoreNMEAInstrument::mousePressEvent(event);
//    }
}

void VelocityHandle::mouseReleaseEvent(QMouseEvent *event)
{
    m_turning = false;
    CoreNMEAInstrument::mouseReleaseEvent(event);
}

void VelocityHandle::mouseMoveEvent(QMouseEvent *event)
{
//    if (m_turning) {
////        float angle = angleFromPoint(event->globalPos());

//        angle = angle - m_initialAngle;

//        if (angle > 180.0) {
//            angle -= 360.0;
//        } else if (angle < -180.0) {
//            angle += 360.0;
//        }

//        if (angle < -160.0) {
//            angle = -160.0;
//        } else if (angle > 160.0) {
//            angle = 160.0;
//        }

//        this->setAngle(angle);
//        if (m_dataSimulatorControlInterface)
//            m_dataSimulatorControlInterface->setTurningSpeed(angle * m_turningFactor);
//    } else
        CoreNMEAInstrument::mouseMoveEvent(event);
}

void VelocityHandle::changeWidgetSize(){
    m_velocityHandleScene->setSceneRect(m_velocityHandleScene->itemsBoundingRect());
    QSize size;
    size.setHeight(m_velocityHandleScene->height());
    size.setWidth(m_velocityHandleScene->width());
    this->resize(size);
}

void VelocityHandle::initializeImages(){
    m_velocityHandle = loadImage(":background");
    m_handle = loadImage(":handle");
    m_anchorOn = loadImage(":anchorOn");
//    m_anchorOn->setPos(-m_velocityHandle->boundingRect().width()/2,0);
//    m_anchorOn->setVisible(false);
    m_anchorOff = loadImage(":anchorOff");
//    m_anchorOff->setPos(-m_velocityHandle->boundingRect().width()/2,0);
//    m_anchorOff->setVisible(true);

    m_velocityHandleScene->addItem(m_velocityHandle);
    m_velocityHandleScene->addItem(m_handle);
    m_velocityHandleScene->addItem(m_anchorOn);
    m_velocityHandleScene->addItem(m_anchorOff);
}

QGraphicsPixmapItem* VelocityHandle::loadImage(const QString& image, int* halfSizeImagePtr)
{
    QPixmap pixmap;
    pixmap.load(image);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);

    QSize halfSizeImage = pixmap.size() / 2;//.height() / 2;
//    if (halfSizeImagePtr)
//        *halfSizeImagePtr = halfSizeImage;

    item->setTransformationMode(Qt::SmoothTransformation);
    item->setScale(m_sizes[m_index]);
    item->setOffset(-halfSizeImage.width(), -halfSizeImage.height());

    return item;
}

void VelocityHandle::connectToSimulator(DataSimulatorControlInterface *simulator, QObject *simulatorObject)
{
    m_dataSimulatorControlInterface = simulator;
    m_dataSimulatorObject = simulatorObject;
}

void VelocityHandle::setAngle(int angle){
//    m_angle = angle;
//    m_velocityHandle->setRotation(m_angle);
}

void VelocityHandle::setParsedNMEAValues(QString angle, QString windSpeedMS, QString windSpeedKnots){
}

QString VelocityHandle::getName(){
    return "VelocityHandleWidget";
}

void VelocityHandle::parseNMEAString(const QString & text){
}

Q_EXPORT_PLUGIN2(velocityHandle, VelocityHandle)
