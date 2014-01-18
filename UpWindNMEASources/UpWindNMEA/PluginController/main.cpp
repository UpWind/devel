#include <QApplication>
#include <QSplashScreen>

#include "controllerwindow.h"

#include "qeffects_p.h"

int main(int argc, char *argv[]) {

    const int splashTime = 1000;

    QApplication app(argc, argv);
    ControllerWindow w;

    QDir imageDir(qApp->applicationDirPath());
    imageDir.cd("plugins");

    QPixmap pixmap(imageDir.absoluteFilePath("logoupwind.png"));
    QSplashScreen *splash = new QSplashScreen(pixmap);

    splash->setAttribute(Qt::WA_DeleteOnClose);

    fadePlugin(splash, splashTime, false);
    splash->show();

    QTimer::singleShot(splashTime, &w, SLOT(show()));
    QTimer::singleShot(splashTime, splash, SLOT(close()));

    //w.show();

    return app.exec();

}
