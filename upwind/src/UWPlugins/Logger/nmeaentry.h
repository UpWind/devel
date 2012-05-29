#ifndef NMEAENTRY_H
#define NMEAENTRY_H

#include <QString>

/** Class declaration of NMEAEntry, a small class in which instance NMEA-String and the time it was received,
    is temporarily stored.
  */

class NMEAEntry
{
public:
    NMEAEntry();

    /**
      * Gets NMEA-String catched
      * @return QString that is the NMEA-String
      */
    QString getNMEA();

    /**
      * Gets the exact date and time when NMEA-String was received.
      * @return QString presenting the time of the reception of NMEA-String
      */
    QString getDate();

    /**
      * Sets NMEA-String
      * @param string - QString holding NMEA-data.
      */
    void setNMEA(QString string);

    /**
      * Sets the exact date and time when NMEA-String was received.
      * @param d - date and time as QString
      */
    void setDate(QString d);

private:
    QString nmeastring;
    QString date;
};

#endif // NMEAENTRY_H
