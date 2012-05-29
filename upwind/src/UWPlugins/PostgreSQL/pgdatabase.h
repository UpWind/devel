#ifndef PGDATABASE_H
#define PGDATABASE_H

#include "../ChartReader/corechartreader.h"
#include "settingsui.h"

/** The PGDataBase class is used to look for all the UpWind DataBases, you have to choose
  * user,password,host options. Also, it creates a XML file for each of the DB with a UI editor
  */
class LayersManager;

class PGDataBase: public CoreChartReader
{
    Q_OBJECT

public:
    /** Create the class, the settings UI and initialize it
      */
    PGDataBase();
    ~PGDataBase();

    /** Return the name of the plugin
      */
    QString getName() ;

    /** Add the SettingsUI to one layout.
      * @param layout - layout from the Core program
      */
    void addPluginSettingsToLayout(QLayout *layout);

    /** Put the settings from a XML file, if  the XML file doesn't exists create new one with the default settings
      */
    void initializeSettings();

    /** Return the name of the DataBase
      * @return Database QString name
      */
    QString getDBName();

    /** Return the user of the DataBase
      * @return Database QString user
      */
    QString getDBUser();

    /** Return the password of the DataBase
      * @return Database QString password
      */
    QString getDBPass();

    /** Return the conection port of the DataBase
      * @return Database QString port
      */
    QString getDBPort();

    /** Return the host (i.e: localhost) of the DataBase
      * @return Database QString host
      */
    QString getDBHost();

    /** Return the driver (you can choose between Postgres or SQL) of the DataBase
      * @return Database QString driver
      */
    QString getDBDriver();

    /** Set the name of the database in use
      * @param  string - name of the dataBase
      */
    void setDBName(QString string);

    /** Set the user of the database in use
      * @param  string - user of the dataBase
      */
    void setDBUser(QString string);

    /** Set the password of the database in use
      * @param  string - password of the dataBase
      */
    void setDBPass(QString string);

    /** Set the port of the database in use
      * @param  string - port of the dataBase
      */
    void setDBPort(QString string);

    /** Set the host of the database in use
      * @param  string - host of the dataBase
      */
    void setDBHost(QString string);

    /** Set the driver of the database in use
      * @param  string - driver of the dataBase
      */
    void setDBDriver(QString string);
private slots:

private:
    //settings
    SettingsUI* settingsUI;
    QString DBName;
    QString DBUser;
    QString DBPass;
    QString DBPort;
    QString DBHost;
    QString DBDriver;
    LayersManager* layersManager;
};

#endif // PGDATABASE_H
