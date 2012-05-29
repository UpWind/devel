#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>

#include "Device.h"
#include "ConfigWriter.h"



class SettingsManager
{
public:
        static SettingsManager* getInstance();
        ~SettingsManager();


        bool getAntialiasing() const;
        bool getOpenGL() const;
        int getRotateFactor() const;
        int getInitialZoomLevel() const;
        int getZoomFactor() const;

        QString getDBName() const;
        QString getDBUser() const;
        QString getDBPass() const;
        QString getDBPort() const;
        QString getDBHost() const;
        QString getDBDriver() const;
        QString getChartTableXMLFile() const;

        bool getRotateMap() const;
        QString getBoatName() const;
        int getBoatRefresh() const;
        bool getUseAvgData() const;
        float getMinDepth() const;
        float getSignsound() const;
        int getMaxTurn() const;
        float getPOffset() const;
        float getAccu() const;
        bool getUpWindPerformance() const;


        Device getDevice();
        BoatInfo getBoatInfo();
        BoatInfo * getBoatInfoPointer();
        Log getLogger();


        void setOpenGL(bool openGL);
        void setAntialiasing(bool antialiasing);
        void setRotateFactor(int rotateFactor);
        void setInitialZoomLevel(int initialZoomLevel);
        void setZoomFactor(int zoomFactor);

        void setDBName(QString DBName);
        void setDBUser(QString DBUser);
        void setDBPass(QString DBPass);
        void setDBPort(QString DBPort);
        void setDBHost(QString DBHost);
        void setDBDriver(QString DBDriver);
        void setChartTableXMLFile(QString DBChartTableName);

        void setRotateMap(bool rotateMap);
        void setBoatName( QString);
        void setBoatRefresh( int);
        void setUseAvgData( bool);
        void setMinDepth( float);
        void setSignsound( float);
        void setMaxTurn( int);
        void setPOffset( float);
        void setAccu( float);
        void setUpWindPerformance(bool upWindPerformance);

        void setLog(bool log);
        void setLoadLogName(QString logname);
        void setLongitude(bool longi);
        void setLatitude(bool latitude);
        void setSpeedOverGround(bool speedOverGround);
        void setAppWindSpeed(bool appWindSpeed);
        void setAppWindAngle(bool appWindAngle);
        void setTWindSpeedM(bool tWindSpeedM);
        void setTWindSpeedKm(bool tWindSpeedKm);
        void setTWindSpeedKnots(bool tWindSpeedKnots);
        void setCompass(bool compass);
        void setWaterHeadingMag(bool waterHeadingMag);
        void setWaterHeadingT(bool waterHeadingT);
        void setWaterSpeedKnots(bool waterSpeedKnots);
        void setWaterSpeedKPH(bool waterSpeedKPH);
        void setDepth(bool depth);


        bool getLog();
        bool getLongitude();
        bool getLatitude();
        bool getSpeedOverGround();
        bool getAppWindSpeed();
        bool getAppWindAngle();
        bool getTWindSpeedM();
        bool getTWindSpeedKm();
        bool getTWindSpeedKnots();
        bool getCompass();
        bool getWaterHeadingMag();
        bool getWaterHeadingT();
        bool getWaterSpeedKnots();
        bool getWaterSpeedKPH();
        bool getDepth();

        void setDevice(Device device);
        void setBoatInfo();

        void loadSettings();
        void saveSettings();

private:
        SettingsManager();
        static SettingsManager *instance;

        Graphics graphics;
        Database db;
        Device device;
        General general;
        Log log;
        BoatInfo boatinfo;
        bool debug;
        ConfigWriter *xmlWriter;

};

#endif /* SETTINGSMANAGER_H_ */
