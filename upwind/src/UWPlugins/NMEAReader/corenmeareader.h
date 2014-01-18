#ifndef CORENMEAREADER_H
#define CORENMEAREADER_H

#include <QThread>
#include <QString>

#include "nmeareaderinterface.h"
#include "../../Settings/settings.h"

/// size of the buffer that the "Average data" option of the boat will use
#define BUFFER_SIZE 4
/// for initialization of variables with a not valid value
#define NO_VALUE 2000

class CoreNMEAReader: public QThread, public NMEAReaderInterface
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.CoreNMEAReader")
    Q_INTERFACES(NMEAReaderInterface UWPluginInterface)

public:
    CoreNMEAReader();

    /** Add the SettingsUI to one layout.
      * @param layout - layout from the Core program
    */
    void addPluginSettingsToLayout(QLayout *layout) ;

    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    QString getName() ;

    /** Return the settigns
      */
    Settings* getSettings() ;

    /** Put the settings from a XML file, if  the XML file doesn't exists create new one with the default settings
      */
    void initializeSettings();

    static QString generateChecksum(QString & nString);

    void setRMC(QStringList);
    void setMWV(QStringList);
    void setVWT(QStringList);
    void setHDT(QStringList);
    void setHDM(QStringList);
    void setVHW(QStringList);
    void setDPT(QStringList);
    QString getRMCHour() const;
    QString getRMCStatus() const;
    QString getRMCLat() const;
    QString getRMCLong() const;
    QString getRMCSpeed() const;
    float getRMCTrackAngle() const;
    QString getRMCDate() const;
    QString getRMCMagVar() const;
    qreal getMWVWindAng() const;
    QString getMWVRefer() const;
    double getMWVWindSpeed() const;
    QString getMWVUnits() const;
    QString getMWVStatus() const;
    float getVWTCalcuWindAng() const;
    int getVWTCalcuWindDir() const;
    QString getVWTCalcuWindSpeed() const;
    qreal getVWTWindSpeedMS() const;
    qreal getVWTWindSpeedKmH() const;
    QString getHDTtrueHeading() const;
    QString getHDMHeading() const;
    QString getHDMMagnetic() const;
    QString getVHWHeadingTrue() const;
    QString getVHWHeadingMag() const;
    qreal getVHWSpeedWaterKnots() const;
    qreal getVHWSpeedKHP() const;
    QString getDPTDepth() const;

    QPointF getPosition();
    qreal getLatitude();
    qreal getLongitude();
    double getWindAngleRadians();

    float getAVGWindSpeed() const;
    float getAVGTWA() const;
    float getAVGHeading() const;

    void run();

private:
    float wsp[BUFFER_SIZE];
    float twa[BUFFER_SIZE];
    float hed[BUFFER_SIZE];
    int wsp_pos, twa_pos, hed_pos;
    bool debug;

    QStringList RMC;
    QString RMCHour;
    QString RMCStatus;
    QString RMCLat;
    QString RMCLatDir;
    QString RMCLong;
    QString RMCLongDir;
    QString RMCSpeed;
    QString RMCTrackAngle;
    QString RMCDate;
    QString RMCMagVar;
    QStringList MWV;
    QString MWVWindAng;
    QString MWVRefer;
    QString MWVWindSpeed;
    QString MWVUnits;
    QString MWVStatus;
    QStringList VWT;
    QString VWTCalcuWindAng;
    QString VWTCalcuWindDir;
    QString VWTCalcuWindSpeed;
    QString VWTCalcuWindSpeedUnit;
    QString VWTWindSpeedMS;
    QString VWTWindSpeedKmH;
    QStringList HDT;
    QString HDTtrueHeading;
    QStringList HDM;
    QString HDMHeading;
    QString HDMMagnetic;
    QStringList VHW;
    QString VHWHeadingTrue;
    QString VHWHeadingMag;
    QString VHWSpeedWaterKnots;
    QString VHWSpeedKHP;
    QStringList DPT;
    QString DPTDepth;
    QString DPTOffset;

protected:
    Settings *settings;

signals:

    void newNMEAString(const QString & text);
};

#endif // CORENMEAREADER_H
