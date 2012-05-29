#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTimer>
#include <QThread>
#include <QPair>
#include "savingtype.h"

/*! \file filecontroller.h
    \brief This file includes FileController which is used for saving data
   */

//! FileController class
/*!
  This class takes data and saves it to a file when asked. Can also empty the data when asked to.
  Extends QThread for the reason the read function which should be ran in a seperate thread. Everything else
  runs properly without it.
*/
class FileController : public QThread
{
   Q_OBJECT

public:
    //! A constructor
    /*!
      This constructs the FileController. It gives the parent to its superclass QThread.
      \param QObject *parent which is the parent of the object created here. If nothing is given, it is 0.
    */
    explicit FileController(QObject *parent = 0);

    //! A destructor
    /*!
      This destructs the FileController. It deletes the whole damn thing along with the readFileData,
      if it has been made (using command new).
    */
    virtual ~FileController();

    //! Save data to a given file.
    /*!
        \param QString filepath conts reference which has the path of the needed file
        \param Boolean true (default) if the traditional way is used, false uses also names of boats
        \return int which has the return code which is either 0 (succesful) or -1 (unsuccesful)
      */
    virtual int save(const QString &filepath, bool ask = true);

    //! Adds data to get saved
    /*!
      When data is being put to the text edit, serial port and/or server, this should also be called if
      saving is used at all in the application.
        \param QString line const reference which has the NMEA line
        \param QString time const reference which has the time as ms of the start of the application or demo
        \param QString name of the boat (default is default value)
        \param Boolean true (default) if the traditional way is used, false uses also names of boats
      */
    void addToBeSaved(const QString &line, const QString &time, QString name = "default", bool ask = true);

    //! Empties the data which previously was thought to get saved
    /*!
        Just empties the variable toBeSaved. When the data is cleared from the text edit in gui, this should
        also be called if saving is used at all in the application.
      */
    void emptyToBeSaved();

    //! Reads a txt file in a thread
    /*!
        This reads the txt file that is in the given file path. It stores the file into a QList<SavingType>.
        Then it emits lines in the QList according to the time stamps that are in the xml file.
        This does not care what is the first time stamp as this just makes it zero point.
        So if first time stamp is 10000 ms and second is 11000 ms, it will send the first line immediatly and
        the second one 1000 ms after the first line. Should only be used in a seperate thread so start()
        should be used, not this directly.
        \param QString filepath const reference which has the file path for the file to be read
      */
    virtual int read(const QString &filepath);

    //! Sets file path
    /*!
        This sets the file path that is needed in the read function when it is called when thread is in use.
        \param QString filepath const reference which has the file path for the file
      */
    void setFilepath(const QString &filepath);

    //! Says stop to the file read
    /*!
        This marks variable cannotRun to false so if the file reading is running it stops in a while.
      */
    void setCannotRun();

    //! Get data from a txt file
    /*!
        This gets data from the given txt file to a QList which has the data as a pair.
        \param QString filepath const reference which has the file path for the file
        \param Boolean true (default) if the traditional way is used, false uses also names of boats
        \return Pointer to the QList which has QPairs that have the line and the time as ms
      */
    virtual QList<QPair<QString, int> >* getFileData(const QString &filepath, bool ask = true);

signals:
    //! Signals given line
    /*!
        This emits the signal that a NMEA line wants to be given.
        \param QString line const reference which has the NMEA line
      */
    void giveLine(const QString &line);

protected:
    /** \var QList<SavingType> toBeSaved
      \brief This has the information to be saved and it has SavingType kind of objects
      */
    QList<SavingType> toBeSaved;

    /** \var QList<QPair<QString, int> > *readFileData
      \brief This has the information that has been read from a xml file in the getFileData function.
      */
    QList<QPair<QString, int> > *readFileData;

    /** \var QString filename
      \brief This has the file name and path which is used when reading a file.
      */
    QString filename;

    /** \var bool filepathChangeble
      \brief This informs if filepath can be changed in the read function.
      */
    bool filepathChangeble;

    /** \var bool cannotRun
      \brief This informs if the reading can continue.
      */
    bool cannotRun;

    //! Gets the current date
    /*!
        \return QString which has the date as QString (e.g. 28.09.2010)
      */
    QString getDate();

    //! The start of the seperate thread
    /*!
        This is overloaded function from the superclass QThread. This is called when the seperate thread starts.
        This calls the read function.
      */
    void run();


};

#endif // FILECONTROLLER_H
