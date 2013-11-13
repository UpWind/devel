#include "gps.h"
#include "ui_gps.h"

GPS::GPS(QWidget *parent){
    this->setParent(parent);
    ui.setupUi(this);
    this->move(20, 20);
    gpsScene = new QGraphicsScene();

    this->instrumentView->setScene(gpsScene);
    ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    longitudePosition.setX(120);
    longitudePosition.setY(130);
    latitudePosition.setX(120);
    latitudePosition.setY(80);

    sizes << 0.55;
    sizes << 0.85;
    sizes << 1.15;
    index = 0;
    initializeImages();

    setDefaultStyleSheet();
    changeWidgetSize();

    connect(this->instrumentView, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseDoubleClicked(QMouseEvent*)), this, SLOT(changeSize()));
    this->initializeSettings();
}

GPS::~GPS(){
    delete gps;
    delete gpsScene;
}

void GPS::changeSize(){
    index = (index + 1) % 3;
    gps->setScale(sizes[index]);
    changeWidgetSize();
}

void GPS::changeWidgetSize(){
    gpsScene->setSceneRect(gpsScene->itemsBoundingRect());
    QSize size;
    size.setHeight(gpsScene->height());
    size.setWidth(gpsScene->width());
    latitude->setPos(latitudePosition.x() * sizes[index] - halfSizeImage*sizes[index], latitudePosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    longitude->setPos(longitudePosition.x() * sizes[index] - halfSizeImage*sizes[index], longitudePosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    latitude->setScale(sizes[index]);
    longitude->setScale(sizes[index]);
    this->resize(size);
}

void GPS::initializeImages(){
    QPixmap gpsPix;
    gpsPix.load(":gps");

    // Create new picture for transparent
    QPixmap transparent(gpsPix.size());

    // Do transparency
    transparent.fill(Qt::transparent);
    QPainter p(&transparent);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, gpsPix);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);

    // Set transparency level to 150 (possible values are 0-255)
    p.fillRect(transparent.rect(), QColor(0, 0, 0, 150));
    p.end();

    // Set original picture's reference to new transparent one
    gpsPix = transparent;

    gps = new QGraphicsPixmapItem(gpsPix);

    halfSizeImage = gpsPix.size().height() / 2;

    gps->setTransformationMode(Qt::SmoothTransformation);
    gps->setScale(sizes[index]);
    gps->setOffset(-halfSizeImage, -halfSizeImage);

    QFont b;
    gpsScene->addItem(gps);
    b.setStyleHint(QFont::Decorative);
    b.setBold(true);
    b.setPointSize(16);

    latitude=gpsScene->addSimpleText("Latitude: 0",b);
    longitude=gpsScene->addSimpleText("Longitude: 0",b);

    latitude->setPos(latitudePosition.x() * sizes[index] - halfSizeImage * sizes[index], latitudePosition.y() * sizes[index] - halfSizeImage * sizes[index]);
    longitude->setPos(longitudePosition.x() * sizes[index] - halfSizeImage * sizes[index], longitudePosition.y() * sizes[index] - halfSizeImage * sizes[index]);
}

QString GPS::getName(){
    return "GPSWidget";
}

void GPS::parseNMEAString( const QString & text){
    //$IIRMC,,A,2908.38,N,16438.18,E,0.0,0.0,251210,,*06
    if(text[3] == QChar('R') && text[4] == QChar('M') && text[5] == QChar('C')){
        this->parsedNMEAValues.clear();

        QStringList strList = text.split(",");

        double latitudeDouble = strList.at(3).toDouble();

        double longitudeDouble = strList.at(5).toDouble();

        longitudeDouble /=100;
        latitudeDouble /=100;

        QString latitudeValue = QString::number(latitudeDouble);
        this->parsedNMEAValues.append(latitudeValue);

        QString longitudeValue = QString::number(longitudeDouble);
        this->parsedNMEAValues.append(longitudeValue);

        if(strList.at(4) == "S"){
            this->parsedNMEAValues.append("S");
            latitude->setText("Latitude: -" + latitudeValue + "\tS");
        } else{
            this->parsedNMEAValues.append("N");
            latitude->setText("Latitude: " + latitudeValue + "\tN");
        }
        this->parsedNMEAValues.append(longitudeValue);
        if(strList.at(6) == "W"){
            this->parsedNMEAValues.append("W");
            longitude->setText("Longitude: -" + longitudeValue + "\tW");
        } else{
            this->parsedNMEAValues.append("E");
            longitude->setText("Longitude: " + longitudeValue + "\tE");
        }

    }
}

Q_EXPORT_PLUGIN2(gps, GPS)
