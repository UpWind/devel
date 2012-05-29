#include <QtGui/QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include "coastline.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene scene;
    scene.addItem(new CoastLine);
    QGraphicsView view;
    view.setScene(&scene);

    view.show();
    return a.exec();
}
