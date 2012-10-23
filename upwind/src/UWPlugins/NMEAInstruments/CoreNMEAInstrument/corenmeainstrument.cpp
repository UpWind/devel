#include "corenmeainstrument.h"
#include <QFileInfo>

CoreNMEAInstrument::CoreNMEAInstrument(QWidget *parent){
    this->setParent(parent);
    this->instrumentView = new InstrumentView(this);
}

void CoreNMEAInstrument::initializeSettings(){
    QFileInfo info(this->getName() + ".xml");

    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else
        this->savePosition();
    this->move(settings->getSetting("XCoordinate").toInt(), settings->getSetting("YCoordinate").toInt());
}

CoreNMEAInstrument::~CoreNMEAInstrument(){
    delete this->instrumentView;
}

void CoreNMEAInstrument::mouseMoveEvent(QMouseEvent *event){
    QPoint relativePos(event->globalPos().x()-handlingPoint.x(), event->globalPos().y()-handlingPoint.y());
    actualPoint.setX(this->pos().x()+relativePos.x());
    actualPoint.setY(this->pos().y()+relativePos.y());

    this->move(actualPoint.x(), actualPoint.y());
    handlingPoint= event->globalPos();
}

void CoreNMEAInstrument::savePosition(){
    if(settings){
        settings->setSetting("XCoordinate", QString::number(this->x()));
        settings->setSetting("YCoordinate", QString::number(this->y()));
        settings->setSetting("Width", QString::number(this->size().width()));
        settings->setSetting("Height", QString::number(this->size().height()));
        settings->saveSettings();
    }
}

QStringList CoreNMEAInstrument::getParsedNMEAValues(){
    return this->parsedNMEAValues;
}

void CoreNMEAInstrument::mouseDoubleClickEvent (QMouseEvent *event)
{
	(void)event;
}

void CoreNMEAInstrument::mousePressEvent(QMouseEvent *event){
    handlingPoint= event->globalPos();
}

void CoreNMEAInstrument::addPluginSettingsToLayout(QLayout *layout) 
{
	(void)layout;
}

Settings* CoreNMEAInstrument::getSettings() {
    return this->settings;
}

void CoreNMEAInstrument::setDefaultStyleSheet(){
    defaultStyleSheet= this->styleSheet();
}

void CoreNMEAInstrument::setSelectedStyleSheet(){
    setStyleSheet("QGroupBox {background-color: rgba(0, 0, 0, 10%);	color:black; border: 0px;}"
                  "QGraphicsView{ background-color: rgba(0, 0, 0, 0%); } "
                  "QLabel { color:white; }");
}

void CoreNMEAInstrument::showPlugin(){
    this->show();
}

void CoreNMEAInstrument::hidePlugin(){
    this->hide();
}

QGroupBox* CoreNMEAInstrument::getGUI(){
    return this;
}
