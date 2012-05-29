#include "compass.h"

Compass::Compass(QWidget *parent){
    this->setParent(parent);
    ui.setupUi(this);

    compassScene = new QGraphicsScene();

    this->instrumentView->setScene(compassScene);
    ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    sizes << 0.5;
    sizes << 0.8;
    sizes << 1.1;
    index = 0;

    initializeImages();
    setDefaultStyleSheet();
    changeWidgetSize();

    angle = 0;

    connect(this->instrumentView, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseDoubleClicked(QMouseEvent*)), this, SLOT(changeSize()));
    this->initializeSettings();
}

Compass::~Compass(){
    delete compass;
    delete compassScene;
}

void Compass::changeSize(){
    index = (index + 1) % 3;
    compass->setScale(sizes[index]);
    hand->setScale(sizes[index]);
    changeWidgetSize();
}

void Compass::changeWidgetSize(){
    compassScene->setSceneRect(compassScene->itemsBoundingRect());
    QSize size;
    size.setHeight(compassScene->height());
    size.setWidth(compassScene->width());
    this->resize(size);
}

void Compass::initializeImages(){
    QPixmap compassPix;
    compassPix.load(":compass");
    compass = new QGraphicsPixmapItem(compassPix);

    int h = compassPix.size().height() / 2;
    int w = compassPix.size().width() / 2;

    compass->setTransformationMode(Qt::SmoothTransformation);
    compass->setScale(sizes[index]);
    compass->setOffset(-h, -w);

    QPixmap handPix;
    handPix.load(":hand");
    hand = new QGraphicsPixmapItem(handPix);

    hand->setTransformationMode(Qt::SmoothTransformation);
    hand->setScale(sizes[index]);
    hand->setOffset(-h, -w);

    compassScene->addItem(compass);
    compassScene->addItem(hand);
}

void Compass::setAngle(int angle){
    angle = 180 - angle;
    hand->resetTransform();
    hand->rotate(angle);
    this->parsedNMEAValues.clear();
    this->parsedNMEAValues.append(QString::number(angle));
}

QString Compass::getName(){
    return "CompassWidget";
}

void Compass::parseNMEAString(const QString & text){
    //$HCHDG,101.1,,,7.1,W*3C
    if(text[3] == QChar('H') && text[4] == QChar('D') && (text[5] == QChar('G') || text[5] == QChar('T'))){
        QStringList strList = text.split(",");
        float dialValueFloat = ((QString)strList.at(1)).toFloat();
        int dialValue = (int)dialValueFloat;
        angle = dialValue;
        dialValue >= 180 ? dialValue -=180 : dialValue += 180;
        setAngle(angle);
    }
}

Q_EXPORT_PLUGIN2(Compass, Compass)
