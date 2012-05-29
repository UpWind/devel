#ifndef SAVINGTYPE_H
#define SAVINGTYPE_H
#include <QString>

/*! \file savingtype.h
    \brief This file includes SavingType which has the data of one single line to be saved
   */

//! SavingType class
/*!
  This class has data that needs to be saved on a one line in the file
*/
class SavingType
{
public:
    //! A constructor
    /*!
      This constructs the SavingType
      \param QString linestr const reference which has the NMEA line
      \param QString timestr const reference which has the time as ms from start of the application or the demo
    */
    SavingType(const QString &linestr, const QString &timestr);

    //! Give the NMEA line
    /*!
        \return QString which has the NMEA line
      */
    QString getLine();

    //! Give the time as ms
    /*!
        \return QString which has the time as ms from start of the application or the demo
      */
    QString getTime();

    //! Set the name of the boat
    /*!
        \param QString const reference which has the name of the boat
      */
    void setName(const QString &newname);

    //! Give the name of the boat
    /*!
        \return QString which has the name of the boat
      */
    QString getName();
private:
    /** \var QString line
      \brief This has the NMEA line
      */
    QString line;

    /** \var QString time
      \brief This has the time as ms from start of the application or the demo
      */
    QString time;

    /** \var QString name
      \brief This has the name of the boat
      */
    QString name;
};

#endif // SAVINGTYPE_H
