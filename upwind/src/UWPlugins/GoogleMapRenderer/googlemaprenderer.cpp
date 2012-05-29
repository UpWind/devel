#include "googlemaprenderer.h"

GoogleMapRenderer::GoogleMapRenderer()
{}

void  GoogleMapRenderer::ConnectPlugin(UpWindSceneInterface* scene, QWidget* frame){
    CoreViewRenderer::ConnectPlugin(scene, frame);
    chart= new GoogleMapChart(this->frame);
    chart->setGeometry(0,0,1280,1024);
    chart->load(QUrl("./index.html") ) ;
    chart->startNavigation();
    chart->showFullScreen();
    chart->lower();
}

QString GoogleMapRenderer::getName(){
    return "GoogleMapRenderer";
}

Q_EXPORT_PLUGIN2(googlemaprenderer, GoogleMapRenderer);
