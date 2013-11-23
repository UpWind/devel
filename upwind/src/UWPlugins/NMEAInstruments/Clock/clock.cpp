#include "clock.h"
#include "ui_clock.h"
#include <QGraphicsItem>

Clock::Clock(QWidget *parent){
    this->setParent(parent);
    ui.setupUi(this);
    this->move(50,50);

    lcd = new QLCDNumber(this);
    ui.gridLayout->addWidget(lcd, 0, 0);

    clockScene = new QGraphicsScene();
    this->instrumentView = new InstrumentView(this);
    this->instrumentView->setScene(clockScene);
    ui.gridLayout->addWidget(this->instrumentView, 0, 0, 1, 1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    showTime();

    sizes << 0.0;
    sizes << 0.30;
    sizes << 0.60;
    index = 0;

    originalSize = new QSize(130, 70);

    setDefaultStyleSheet();
    changeWidgetSize();

    connect(this->instrumentView, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->instrumentView, SIGNAL(mouseDoubleClicked(QMouseEvent*)), this, SLOT(changeSize()));
    this->initializeSettings();
}

Clock::~Clock(){
    delete lcd;
    delete timer;
    delete clockScene;
}

void Clock::changeSize(){
    index = (index + 1) % 3;
    changeWidgetSize();
}

void Clock::showTime(){
    QTime timerr = QTime::currentTime();
    QString text = timerr.toString("hh:mm");
    if((timerr.second() % 2) == 0)
        text[2] = ' ';
    lcd->display(text);
    this->parsedNMEAValues.clear();
    this->parsedNMEAValues.append(text);
}

void Clock::changeWidgetSize(){
    lcd->resize(originalSize->width() + originalSize->width() * sizes[index], originalSize->height() + originalSize->height() * sizes[index]);
    this->resize(originalSize->width() + originalSize->width() * sizes[index], originalSize->height() + originalSize->height() * sizes[index]);
}

QString Clock::getName(){
    return "ClockWidget";
}

void Clock::parseNMEAString(const QString & text)
{
	(void)text;
   // qDebug() << "Parse NMEA string clock line 73";

}

Q_EXPORT_PLUGIN2(Clock, Clock)
