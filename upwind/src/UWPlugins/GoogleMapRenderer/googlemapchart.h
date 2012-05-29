#ifndef GOOGLEMAPCHART_H
#define GOOGLEMAPCHART_H

#include <QWebView>
#include "randomnavigate.h"

/*Temporal Chart, Google map visualitation*/


class QNetworkAccessManager;

class GoogleMapChart: public QWebView
{
        Q_OBJECT
public:
        GoogleMapChart(QWidget *parent=0);
        void startNavigation();
        void dimensions(int a,int b);
public slots:
        void replyFinished(QNetworkReply*);
        void loadCoordinates();
        void geoCode(QString);
        void clearCoordinates();
        void moveBoat();
        void setCoordinates(qreal x, qreal y);

signals:
        void reloadMap();

private:
        QNetworkAccessManager *manager;
        QList<QPointF> coordinates;
        QPointF target;
        randomNavigate *simulator;
        QTimer *timer;
        int pendingRequests;
};

#endif // GOOGLEMAPCHART_H
