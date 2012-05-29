#ifndef GOOGLEMAPRENDERER_H
#define GOOGLEMAPRENDERER_H

#include "../ViewRenderer/coreviewrenderer.h"
#include "randomnavigate.h"
#include <QWebView>
#include "googlemapchart.h"

class QNetworkAccessManager;

class GoogleMapRenderer:  public CoreViewRenderer
{
   // Q_OBJECT

public:
        GoogleMapRenderer();
        void ConnectPlugin(UpWindSceneInterface* scene, QWidget* frame);
        QString getName();

private:
        GoogleMapChart* chart;
};

#endif // GOOGLEMAPRENDERER_H
