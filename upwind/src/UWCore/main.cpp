#include <QtGui/QApplication>
#include "uwcore.h"

int main(int argc, char *argv[]){

 //   QApplication::setGraphicsSystem("opengl");
    QApplication a(argc, argv);

    UWCore::getInstance()->initialize();
    UWCore::getInstance()->getUIManager()->showMainMenu();
    int r = a.exec();

    UWCore::getInstance()->getPluginManager()->appClosing();

    return r;
}
