#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include "coreinterface.h"
#include "coreplugin.h"
#include <QObject>

namespace Ui {
    class %ProjectName%;
}

/*
 * Custom NMEA plugin.
 */

class %ProjectName% : public CorePlugin
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)
private:
    Ui::%ProjectName% *ui;
protected:
    /**
      * Creates the GUI of the plugin.
      */
    void CreateGUI();
protected slots:
    /**
      * Implements parseNMEAString.
      * Parses an NMEAString, loading the values from it and updating the GUI
      * @param text - Reference to the QString which will be parsed
      */
    void parseNMEAString( const QString & text);
    /**
      * Implements updateNMEAString.
      * Slot is called by the timer. Calculates the new NMEA-string based on the available variables.
      */
    void updateNMEAString();
public:
    %ProjectName%();
    virtual ~%ProjectName%();
};

#endif // %ProjectName:u%_H
