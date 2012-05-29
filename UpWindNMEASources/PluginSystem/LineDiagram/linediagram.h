#ifndef LINEDIAGRAM_H
#define LINEDIAGRAM_H

#include "coreinterface.h"
#include "coreplugin.h"
#include <QObject>

namespace Ui {
    class Widget;
}

/**
 * Plugin for drawing the changes in the wind speed as a line diagram.
 */

class LineDiagram : public CorePlugin
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)
private:
    Ui::Widget *ui;
    QGraphicsScene scene;
    int lastx, lasty;
    int w, h;
    int next;
    int takeaway;
    int space;
    QList<QLineF> lines;
protected:
    /**
      * Creates the GUI of the plugin.
      */
    void CreateGUI();

    /**
      * Creates the scale of the diagram.
      */
    void createBounds();

    /**
      * Draws the lines of the diagram if the width of the diagram area has been reached.
      */
    void drawLines();

    /**
      * Draws more width to the scale of the diagram
      */
    void moreBounds();
protected slots:
    /**
      * Implements parseNMEAString.
      * Parses an NMEAString, loading the values from it and updating the GUI
      * This parses the speed from the MWD sentence as knots.
      * @param text - Reference to the QString which will be parsed
      */
    void parseNMEAString( const QString & text);
    /**
      * Implements updateNMEAString.
      * Slot is called by the timer. Calculates the new NMEA-string based on the available variables.
      * This is not in use in this plugin.
      */
    void updateNMEAString();

    /**
      * This clears the diagram and starts everything from start.
      */
    void clearDiagram();

public:
    LineDiagram();
    virtual ~LineDiagram();
};

#endif // LINEDIAGRAM_H
