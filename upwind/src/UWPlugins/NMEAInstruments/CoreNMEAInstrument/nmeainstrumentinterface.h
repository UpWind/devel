#ifndef NMEAINSTRUMENTINTERFACE_H
#define NMEAINSTRUMENTINTERFACE_H

#include <QtGui>
#include <QObject>
#include "../../UWPluginInterface/uwplugininterface.h"

/** CoreInterface class is the plugin interface for all used plugins. It contains the
  * necessary methods for using the plugins.
  */
class NMEAInstrumentInterface : public UWPluginInterface
{
public:
    /** Virtual destructor of the interface class
      */
    virtual ~NMEAInstrumentInterface() {}

    /** Shows the GUI of the plugin
      */
    virtual void showPlugin() = 0;

    /** Hides the GUI of the plugin
      */
    virtual void hidePlugin() = 0;

    /** Getter for the UI of the instrument
      * @return QGroupBox* - the UI of the instrument in a QGroupBox*
      */
    virtual QGroupBox* getGUI() = 0;

    /** Parses an NMEAString, loading the values from it and updating the GUI
      * @param text - Reference to the QString which will be parsed
      */
    virtual void parseNMEAString(const QString & text) = 0;

    /** Saves the instrument size and position settings to xml file.
      */
    virtual void savePosition() = 0;

    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    virtual QString getName() = 0;

    /** Returns the most recent parsed values of the instrument NMEAdata.
      * @return QList<QObject> a list with the parsed data.
      */
    virtual QStringList getParsedNMEAValues() = 0;
};

Q_DECLARE_INTERFACE(NMEAInstrumentInterface,"NMEAInstrumentInterface")

#endif // NMEAINSTRUMENTINTERFACE_H
