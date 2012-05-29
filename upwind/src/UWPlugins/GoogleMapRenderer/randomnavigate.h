#ifndef RANDOMNAVIGATE_H
#define RANDOMNAVIGATE_H
#include <QWebView>
#include <QTimer>

class randomNavigate
{
public:
    QPointF actual;
    randomNavigate();
    randomNavigate(QPointF a);
    void setCoordinates(QPointF a);
    QPointF getCoordinates();
    void move();
private:
    int randomValue;
    int randInt(int low, int high);
};

#endif // RANDOMNAVIGATE_H
