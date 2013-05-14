#include "anemometer.h"
#include "ui_anemometer.h"

const static double METER_TO_KNOT = 1.94384449;

Anemometer::Anemometer(QWidget *parent){
    this->setParent(parent);
    ui.setupUi(this);
    this->move(20, 20);
    anemometerScene = new QGraphicsScene();
    this->instrumentView->setScene(anemometerScene);
    ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    knotsPosition.setX(96);
    knotsPosition.setY(136);
    msPosition.setX(86);
    msPosition.setY(219);

    sizes << 0.55;
    sizes << 0.85;
    sizes << 1.15;
    index = 0;
    initializeImages();

    setDefaultStyleSheet();
    changeWidgetSize();

    m_angle = 0;

    connect(this->instrumentView, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseDoubleClicked(QMouseEvent*)), this, SLOT(changeSize()));
    this->initializeSettings();
}

Anemometer::~Anemometer(){
    delete anemometer;
    delete anemometerScene;
}

void Anemometer::changeSize(){
    index = (index + 1) % 3;
    anemometer->setScale(sizes[index]);
    hand->setScale(sizes[index]);
    changeWidgetSize();
}

void Anemometer::changeWidgetSize(){
    anemometerScene->setSceneRect(anemometerScene->itemsBoundingRect());
    QSize size;
    size.setHeight(anemometerScene->height());
    size.setWidth(anemometerScene->width());
    knots->setPos(knotsPosition.x() * sizes[index] - halfSizeImage * sizes[index], knotsPosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    ms->setPos(msPosition.x() * sizes[index] - halfSizeImage*sizes[index], msPosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    knots->setScale(sizes[index]);
    ms->setScale(sizes[index]);
    this->resize(size);
}

void Anemometer::initializeImages(){
    QPixmap anemometerPix;
    anemometerPix.load(":anomometer");
    anemometer = new QGraphicsPixmapItem(anemometerPix);

    halfSizeImage = anemometerPix.size().height() / 2;

    anemometer->setTransformationMode(Qt::SmoothTransformation);
    anemometer->setScale(sizes[index]);
    anemometer->setOffset(-halfSizeImage, -halfSizeImage);

    QPixmap handPix;
    handPix.load(":handP");
    hand = new QGraphicsPixmapItem(handPix);

    hand->setTransformationMode(Qt::SmoothTransformation);
    hand->setScale(sizes[index]);
    hand->setOffset(-halfSizeImage, -halfSizeImage);

    QFont b;
    anemometerScene->addItem(anemometer);
    anemometerScene->addItem(hand);
    b.setBold(true);
    b.setPointSize(13);

    knots = anemometerScene->addSimpleText("0", b);
    ms = anemometerScene->addSimpleText("0", b);
    knots->font().setBold(true);

    knots->setPos(knotsPosition.x()*sizes[index] - halfSizeImage * sizes[index], knotsPosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    ms->setPos(msPosition.x() * sizes[index] - halfSizeImage * sizes[index], msPosition.y() * sizes[index] - halfSizeImage * sizes[index]);
}

void Anemometer::setAngle(int angle){
    m_angle = angle;
    hand->setRotation(angle);
}

void Anemometer::setParsedNMEAValues(QString angle, QString windSpeedMS, QString windSpeedKnots){
    this->parsedNMEAValues.clear();
    this->parsedNMEAValues.append(angle);
    this->parsedNMEAValues.append(windSpeedMS);
    this->parsedNMEAValues.append(windSpeedKnots);
}

QString Anemometer::getName(){
    return "AnemometerWidget";
}

void Anemometer::parseNMEAString(const QString & text){
    //$HCHDG,101.1,,,7.1,W*3C
    //$IIMWV,5,T,7,N,3.6,M,A,*73
    if(text[3] == QChar('M') && text[4] == QChar('W') && text[5] == QChar('V')){
        QStringList strList = text.split(",");
        QString angleString=(QString)strList.at(1);
        if(!angleString.isEmpty()){
            setAngle(angleString.toDouble() + 180.0);
            if(strList.size() > 6){
                knots->setText((QString)strList.at(3));
                ms->setText((QString)strList.at(5));
                this->setParsedNMEAValues(angleString, (QString)strList.at(3), (QString)strList.at(5));
            } else{
                QString meter = (QString)strList.at(3);
                double meterFromKnot = meter.toDouble();
                meterFromKnot = meterFromKnot * METER_TO_KNOT;
                ms->setText(meter);
                QString knotv = QString::number(meterFromKnot);
                knotv.truncate(3);
                knots->setText(knotv);
                this->setParsedNMEAValues(angleString, meter, knotv);
            }
        }
    }
}

Q_EXPORT_PLUGIN2(anemometer, Anemometer)
