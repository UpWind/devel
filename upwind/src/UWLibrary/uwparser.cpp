#include "uwparser.h"
#include <QDebug>

UWParser::UWParser()
{
}

QStringList* UWParser::parseNMEAString(const QString &nmeaString){

    qDebug() << "Parse NMEA string uwparser line 10";
     QString msgID;
     QStringList *parsedNMEAString = 0;

     QStringList params = nmeaString.split(",");
     msgID = ((QString)params.at(0)).right(3);


     //call the right method according to the NMEA-string's type
     if(msgID == "HDG"){//Compass
         parsedNMEAString = parseCompassString(nmeaString);
     }

     return parsedNMEAString;
}

QStringList* UWParser::parseCompassString(const QString &nmeaString){

	(void)nmeaString;

    //do parsing here
    QStringList *parsedNMEAString = 0;
    return parsedNMEAString;
}
