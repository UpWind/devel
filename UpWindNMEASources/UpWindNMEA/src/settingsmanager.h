#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QtXml>
#include <QPair>
#include <QList>

/*! \file settingsmanager.h
    \brief This file includes SettingsManager which is used for managing setting saving and loading
   */

//! SettingsManager class
/*!
  This class manages the way the settings of the application and plugins are saved and loaded. This uses
  XML-file for keeping the settings.
*/
class SettingsManager : public QObject
{
    Q_OBJECT
public:

    //! A constructor
    /*!
      This constructs the SettingsManager. It gives the parent to its superclass QObject.
      \param QObject *parent which is the parent of the object created here. If nothing is given, it is 0.
    */
    explicit SettingsManager(QObject *parent = 0);

    //! A destructor
    /*!
      This destructs the SettingsManager. It deletes the whole damn thing along with the doc and loadDoc,
      if they have been made (using command new).
    */
    ~SettingsManager();

    //! Save settings to a given file
    /*!
        \param QString filename const reference which has the path of the needed file
        \return boolean which has the return code which is either true (succesful) or false (unsuccesful)
      */
    bool save(const QString &filename);

    //! Puts the start of the object to get saved
    /*!
        \param QString name const reference which has the name of the object which wants it settings saved
        \param QString version const reference which has the version of the object which wants it settings saved
      */
    void saveHeader(const QString &name, const QString &version);

    //! Puts the setting of the setting object
    /*!
        \param QString name const reference which has the name of the setting
        \param QString value const reference which has the value of the setting
      */
    void saveProperty(const QString &name, const QString &value);

    //! Puts the start of the object which settings are to be loaded
    /*!
        \param QString name const reference which has the name of the object which wants it settings loaded
        \return QString which has the version of the object the settings are for
      */
    QString loadHeader(const QString &name);

    //! Gets the setting of the setting object
    /*!
        \param QString name const reference which has the name of the setting
        \param QString defValue const reference which has the value which is given if the there is no given setting
        \return QString which has the value of the asked setting
      */
    QString loadProperty(const QString &name, const QString &defValue);

    //! Load settings from a given file
    /*!
        \param QString filename const reference which has the path of the needed file
        \return boolean which has the return code which is either true (succesful) or false (unsuccesful)
      */
    bool load(const QString &filename);

//signals:

//public slots:

private:
    void setDomBasic();
    QDomDocument *doc;
    QDomElement root;
    QDomElement cn;
    QList<QPair<QString, QString> > entrySettings;
    QDomDocument *loadDoc;
    int next;


};

#endif // SETTINGSMANAGER_H
