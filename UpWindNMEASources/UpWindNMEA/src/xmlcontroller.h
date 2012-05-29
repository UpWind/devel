#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H
#include "filecontroller.h"
#include <QString>
#include <QXmlStreamWriter>

/*! \file xmlcontroller.h
    \brief This file includes XMLController which is used for saving data as xml
   */

//! XMLController class
/*!
  This class takes data and saves it to a xml file when asked. extends FileController.
*/
class XMLController : public FileController
{
    Q_OBJECT
public:
    //! A constructor
    /*!
      This constructs the XMLController. It gives the parent to its superclass FileController.
      \param QObject *parent which is the parent of the object created here. If nothing is given, it is 0.
    */
    explicit XMLController(QObject *parent = 0);


    //! A destructor
    /*!
      This destructs the XMLController. It deletes the whole damn thing along with the readFileDataWithName,
      if it has been made (using command new).
    */
    ~XMLController();

    //! Save data to a given file as xml
    /*!
      This is overloaded function from the super class. This uses QXmlStreamWriter to save the file.
        \param QString filepath const reference which has the path of the needed file
        \param Boolean true (default) if the traditional way is used, false uses also names of boats
        \return int which has the return code which is either 0 (succesful) or -1 (unsuccesful)
      */
    int save(const QString &filepath, bool ask = true);

    //! Reads a xml file
    /*!
        This reads the xml file that is in the given file path. It stores the file into a QList<SavingType>.
        Then it emits lines in the QList according to the time stamps that are in the xml file.
        This does not care what is the first time stamp as this just makes it zero point.
        So if first time stamp is 10000 ms and second is 11000 ms, it will send the first line immediatly and
        the second one 1000 ms after the first line. Should only be used in a seperate thread so start()
        should be used, not this directly.
        \param QString filepath const reference which has the file path for the file to be read
      */
    int read(const QString &filepath);

    //! Get data from a file
    /*!
        This gets data from the given file to a QList which has the data as a pair.
        \param QString filepath const reference which has the file path for the file
        \param Boolean true (default) if the traditional way is used, false uses also names of boats
        \return Pointer to the QList which has QPairs that have the line and the time as ms
      */
    QList<QPair<QString, int> >* getFileData(const QString &filepath, bool ask = true);

    //! Get data from a file with names
    /*!
      This get data from the given file to a QList which has the data as SavingType
      \param QString filepath const reference which has the file path for the file
      \return Pointer to the QList which has SavingTypes that have the line, the time as ms and the name
    */
    QList<SavingType>* getFileDataWithName(const QString &filepath);

    //! Save data with names
    /*!
      This saves data with names to the given file
      \param QString filepath const reference which has the file path for the file
      \return int which has the return code which is either 0 (succesful) or -1 (unsuccesful)
    */
    int saveWithName(const QString &filepath);

protected:
    //! The start of the seperate thread
    /*!
        This is overloaded function from the superclass FileController. This is called when the seperate thread starts.
        This calls the read function.
      */
    void run();

    /** \var QList<SavingType> *readFileDataWithName
      \brief This has the information that has been read from a xml file in the getFileDataWithName function (has also names)
      */
    QList<SavingType> *readFileDataWithName;


};

#endif // XMLCONTROLLER_H
