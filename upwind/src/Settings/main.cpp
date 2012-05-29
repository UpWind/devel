#include <QtGui/QApplication>
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Settings *s = new Settings();

    s->loadSettings();
    s->displaySettings();
    return a.exec();
}
