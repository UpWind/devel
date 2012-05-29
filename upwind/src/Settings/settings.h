#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QMap>
#include <QString>

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
    Settings(const QString &name);

    /**
      * Returns the setting defined in the parameter settingName.
      *
      *@param The name of the setting.
      *@return The setting as a QString.
      */
    QString getSetting(const QString &settingName);

    /**
      * Sets the value of the setting defined in settingName.
      * New value is defined by settingValue.
      * If the setting does not exist, it is created with initial value settingValue.
      *
      *@param The name of the setting.
      *@param The new value of the setting.
      */
    void setSetting(const QString &settingName, const QString &settingValue);

    /**
      * Returns all of the settings in a QMap.
      *
      *@return The QMap with the settings. key = setting name, value = setting value.
      */
    QMap<QString, QString> getSettings();


    void setOwnerName(const QString &name);

    void setFileName(const QString &name);

    void saveSettings();
    void loadSettings();
    void displaySettings();

private:
    QMap<QString, QString> settings;

    //The name of the owning plugin.
    QString ownerName;

    QString fileName;

signals:

public slots:

};

#endif // SETTINGS_H
