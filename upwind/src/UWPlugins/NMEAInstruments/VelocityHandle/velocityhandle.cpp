#include <qmath.h>

#include "../../shared/uwmath.h"
#include "velocityhandle.h"
#include "ui_velocityhandle.h"

const static double METER_TO_KNOT = 1.94384449;
const float handleCenterRadiusSquared = 900.0;

VelocityHandle::VelocityHandle(QWidget *parent) :
     m_index(0)
    ,m_velocityHandleScene(0)
    ,m_velocityHandle(0)
    ,m_handle(0)
    ,m_anchorOn(0)
    ,m_anchorOff(0)
    ,m_angle(0.0)
    ,m_initialAngle(0.0)
    ,m_dragging(false)
    ,m_turningFactor(0.0)
   , m_isAnchored(false)
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

    m_handleRect = QRectF(-40, -130, 80, 210);
    m_handlePosition = m_handleRect.bottomRight();

    m_anchorPosition = QPointF(0,145);

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
    QPoint handleCenter = pos() + QPoint(width()/2, height()/2) + (m_handlePosition * m_sizes[m_index]).toPoint();
    QVector2D handleDistance = QVector2D(event->globalPos() - handleCenter);

    QPoint anchorCenter = pos() + QPoint(width()/2, height()/2) + (m_anchorPosition * m_sizes[m_index]).toPoint();
    QVector2D anchorDistance = QVector2D(event->globalPos() - anchorCenter);

//    qDebug() << Q_FUNC_INFO << distance << wheelCenterRadiusSquared << distance.length();
    if (handleDistance.lengthSquared() < handleCenterRadiusSquared) {
        m_dragging = true;
        handlingPoint= pos() + QPoint(width()/2, height()/2);
//        m_initialAngle = angleFromPoint(event->globalPos()) - m_angle ;
    } else if (anchorDistance.lengthSquared() < handleCenterRadiusSquared) {
        m_isAnchored = !m_isAnchored;
        m_anchorOff->setVisible(!m_isAnchored);
        m_anchorOn->setVisible(m_isAnchored);
        if (m_dataSimulatorControlInterface) {
            m_dataSimulatorControlInterface->setAnchor(m_isAnchored);
        }
    } else {
        CoreNMEAInstrument::mousePressEvent(event);
    }
}

void VelocityHandle::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    CoreNMEAInstrument::mouseReleaseEvent(event);
}

void VelocityHandle::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        m_handlePosition = QPointF(event->globalPos() - pos() - QPoint(width()/2, height()/2)) / m_sizes[m_index];

        if (m_handlePosition.x() < m_handleRect.center().x()) {
            m_handlePosition.setX(m_handleRect.left());
            if (m_dataSimulatorControlInterface) {
                m_dataSimulatorControlInterface->setSail(false);
            }

        } else if (m_handlePosition.x() > m_handleRect.center().x()) {
            m_handlePosition.setX(m_handleRect.right());
            if (m_dataSimulatorControlInterface) {
                m_dataSimulatorControlInterface->setSail(true);
            }
        }

        if (m_handlePosition.y() < m_handleRect.top()) {
            m_handlePosition.setY(m_handleRect.top());
        } else if (m_handlePosition.y() > m_handleRect.bottom()) {
            m_handlePosition.setY(m_handleRect.bottom());
        }

        float multiplierPosition = (m_handleRect.bottom() - m_handlePosition.y()) / m_handleRect.height();
        if (m_dataSimulatorControlInterface) {
            m_dataSimulatorControlInterface->setVelocityMultiplier(multiplierPosition * 99.0 + 1.0);
        }


        if (m_dataSimulatorControlInterface) {
            //m_dataSimulatorControlInterface->setTurningSpeed();
        }

        m_handle->setPos(m_handlePosition.x() * m_sizes[m_index],
                         m_handlePosition.y() * m_sizes[m_index]);
    } else
        CoreNMEAInstrument::mouseMoveEvent(event);
}

void VelocityHandle::changeWidgetSize()
{
    m_velocityHandleScene->setSceneRect(m_velocityHandleScene->itemsBoundingRect());
    QSize size;
    size.setHeight(m_velocityHandleScene->height());
    size.setWidth(m_velocityHandleScene->width());

    m_handle->setPos(m_handlePosition.x() * m_sizes[m_index],
                     m_handlePosition.y() * m_sizes[m_index]);
    m_anchorOn->setPos(m_anchorPosition.x() * m_sizes[m_index],
                       m_anchorPosition.y() * m_sizes[m_index]);
    m_anchorOff->setPos(m_anchorPosition.x() * m_sizes[m_index],
                       m_anchorPosition.y() * m_sizes[m_index]);

    this->resize(size);
}

void VelocityHandle::initializeImages()
{
    m_velocityHandle = loadImage(":background");
    m_handle = loadImage(":handle");
    m_anchorOn = loadImage(":anchorOn");
    m_anchorOn->setVisible(false);
    m_anchorOff = loadImage(":anchorOff");

    m_velocityHandleScene->addItem(m_velocityHandle);
    m_velocityHandleScene->addItem(m_handle);
    m_velocityHandleScene->addItem(m_anchorOn);
    m_velocityHandleScene->addItem(m_anchorOff);

    m_handle->setPos(m_handlePosition.x() * m_sizes[m_index],
                     m_handlePosition.y() * m_sizes[m_index]);
    m_anchorOn->setPos(m_anchorPosition.x() * m_sizes[m_index],
                       m_anchorPosition.y() * m_sizes[m_index]);
    m_anchorOff->setPos(m_anchorPosition.x() * m_sizes[m_index],
                       m_anchorPosition.y() * m_sizes[m_index]);
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
