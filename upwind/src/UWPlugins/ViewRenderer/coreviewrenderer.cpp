#include "coreviewrenderer.h"
#include <QtGui>

CoreViewRenderer::CoreViewRenderer(){
    settings = 0;
}

QString CoreViewRenderer::getName(){
    return QString("CoreViewRenderer");
}

void  CoreViewRenderer::ConnectPlugin(UpWindSceneInterface* scene,QWidget* frame, CoreChartProvider* model){
    this->scene = scene;
    this->frame = frame;
    this->model = model;
}

void CoreViewRenderer::paint()
{
}

void CoreViewRenderer::addPluginSettingsToLayout(QLayout *layout){}

Settings* CoreViewRenderer::getSettings() {
    return settings;
}

void CoreViewRenderer::zoomIn()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::zoomOut()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::rotateLeft()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::rotateRight()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::expand()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::zoomBoat()
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::zoomToolActivated(bool)
{
    qDebug()<<Q_FUNC_INFO;
}

void CoreViewRenderer::simModeChanged(bool)
{
    qDebug()<<Q_FUNC_INFO;
}
void CoreViewRenderer::drawRoute(bool)
{
    qDebug()<<Q_FUNC_INFO;
}
