#ifndef UWPARSER_H
#define UWPARSER_H

#include <QString>
#include <QStringList>

class UWParser
{
public:
    UWParser();
    //Returns parsed values of the NMEA-string given as a parameter.
    static QStringList * parseNMEAString(const QString &nmeaString);

private:

    //One method for each NMEA type
    static QStringList * parseCompassString(const QString &nmeaString);
};

#endif // UWPARSER_H
