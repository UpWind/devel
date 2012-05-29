#include "uwparser.h"

UWParser::UWParser()
{
}

QStringList* UWParser::parseNMEAString(const QString &nmeaString){


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
    //do parsing here
    QStringList *parsedNMEAString = 0;
    return parsedNMEAString;
}
