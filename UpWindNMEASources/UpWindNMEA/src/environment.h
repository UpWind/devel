#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>
#include <QApplication>

/**
  * The Environment namespace has the file paths of the program.
  */
namespace Environment {

    inline QString getLogDirectory() {
        return QDir(QApplication::applicationDirPath() + "/logs/").absolutePath() + "/";
    }

    inline QString getDataDirectory() {
        return QDir(QApplication::applicationDirPath()+ "/data").absolutePath() + "/";
    }

    inline QString getPluginDirectory() {
        return QDir(QApplication::applicationDirPath() + "/plugins").absolutePath() + "/";
    }

    inline QString getSettingsFilepath() {
        return getDataDirectory() + "settings.xml";
    }

};

#endif // ENVIRONMENT_H
