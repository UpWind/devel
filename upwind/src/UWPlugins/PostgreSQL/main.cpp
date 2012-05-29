#include <QtGui/QApplication>
#include "settingsui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsUI w;
    w.show();

    return a.exec();
}
