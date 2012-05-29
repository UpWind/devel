#ifndef RPS_H
#define RPS_H

#include "coreinterface.h"
#include "coreplugin.h"
#include <QObject>

namespace Ui {
    class Widget;
}

/**
 * Rock paper scissors
 */

class RPS : public CorePlugin
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)
private:
    Ui::Widget *ui;
    int forceToBeReconWith;
    int wins;
    int draws;
    int loses;
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
    RPS();
    virtual ~RPS();
    /**
      * This method will save the plugin settings using the given settings manager.
      * @param sm - SettingsManager that implements the saving and loading of settings.
      */
    virtual void saveTo(SettingsManager &sm);
    /**
      * This method will load the plugin settings using the given settings manager.
      * @param sm - SettingsManager that implements the saving and loading of settings.
      */
    virtual QString loadFrom(SettingsManager &sm);

public slots:
    /**
      * This calculates who wins, the player or the ai and informs this.
      * This is done using numbers 0, 1 and 2. 0 means rock, 1 paper and 2 scissors.
      * If available, the angle of the wind meter also affects the outcome with the
      * randomizer.
      */
    void calculateVictory();
    /**
      * This clears the statistics of the game. Every count goes to zero.
      */
    void clearStatistics();
};

#endif // RPS_H
