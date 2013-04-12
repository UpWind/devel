#ifndef CORENMEAINSTRUMENT_H
#define CORENMEAINSTRUMENT_H

#include <QWidget>
#include <QMouseEvent>
#include <QGroupBox>
#include <QPoint>
#include <QtGui>
#include <QString>
#include <QStringList>
#include <QtXml/QDomDocument>

#include "instrumentview.h"
#include "nmeainstrumentinterface.h"
#include "../../../Settings/settings.h"

/** Core class which implements the common methods of the NMEA Instruments
  * Inherits from QGroupBox class because it handles pix maps and QGraphicsView
  * Implements the NMEAInstrumentInterface and the UWPluginInterface (as every plugin, 2 interfaces)
  * It's children will be the NMEA Instruments
  */
class CoreNMEAInstrument : public QGroupBox, public NMEAInstrumentInterface
{
    Q_OBJECT
    Q_INTERFACES(NMEAInstrumentInterface UWPluginInterface)

public:

    /** Constructor of Core NMEA Instrument
      * @param parent - Parent widget of the core plugin
      */
    CoreNMEAInstrument(QWidget *parent = 0);

    /** Destructor of Core NMEA Instruments
      */
    ~CoreNMEAInstrument();

    /** Adds the plugin settings to the given layout.
      * @param layout - Pointer to a QLayout to which the plugin widget settings(UI) will be added
      */
    void addPluginSettingsToLayout(QLayout *layout);

    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    virtual QString getName() = 0;

    /** Getter for the Settings
      * @return the settings of the plugin in a Settings class
      */
    Settings* getSettings();

    /** Getter for the list of values which the instrument handles
      * @return the list of values in a QStringList
      */
    QStringList getParsedNMEAValues();

    /** Saves the position of the instrument in the map
      */
    void savePosition();

    /** Shows the GUI of the plugin
      */
    void showPlugin();

    /** Hides the GUI of the plugin
      */
    void hidePlugin();

    /** Getter for the UI of the instrument
      *@return QGroupBox* - the UI of the instrument in a QGroupBox*
      */
    QGroupBox* getGUI();

    /** Initializes the attribute of the settings
      */
    void initializeSettings();

    /** Parses an NMEAString, loading the values from it and updating the GUI
      * @param text - Reference to the QString which will be parsed
      */
    virtual void parseNMEAString( const QString & text) = 0;

public slots:
    /** Method to manage the movement of the mouse when the click is pressed on the instrument
      * @param event - the movement of the mouse is a QMouseEvent*
      */
    void mouseMoveEvent(QMouseEvent *event);

    /** Method to manage the click of the mouse when it is pressed on the instrument
      * @param event - the click of the mouse is a QMouseEvent*
      */
    void mousePressEvent(QMouseEvent *event);

    /** Method to manage the double click of the mouse when it is pressed on the instrument
      * @param event - the double click of the mouse is a QMouseEvent*
      */
    void mouseDoubleClickEvent ( QMouseEvent *event );

private:
    QPoint actualPoint;
    QPoint handlingPoint;
    Settings *settings;

protected:
    QString defaultStyleSheet;
    QStringList parsedNMEAValues;
    InstrumentView * instrumentView;

    /** Set the default style sheet
      */
    void setDefaultStyleSheet();
    /** Change the style
      */
    void setSelectedStyleSheet();

signals:
    void selectedS(bool);
};

#endif // CORENMEAINSTRUMENT_H
