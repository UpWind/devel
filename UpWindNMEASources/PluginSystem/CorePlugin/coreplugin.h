#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include <QTimer>
#include "coreinterface.h"

class SettingsManager;

/**
  * PluginQWidget is the widget class that is used by plugins for their UI.
  * It is possible to configure the close action and event of this widget.
  */
class PluginQWidget: public QWidget{
    Q_OBJECT
protected:
    bool ignoresClose;

    virtual void closeEvent(QCloseEvent *e){
        emit onClose();
        if(ignoresClose)
            e->ignore();
    }

    virtual void resizeEvent(QResizeEvent *event){
        QWidget::resizeEvent(event);
        emit onResize(event);
    }

public:
    /**
      * Constructor of the PluginQWidget.
      * @param parent - Parent widget of the plugin widget.
      * @param f - Window flags of the widget
      */
    PluginQWidget(QWidget *parent = 0, Qt::WindowFlags f = 0):
            QWidget(parent, f), ignoresClose(true){
    }

    /**
     * Getter function of the ignoresClose attribute.
     * @return true if the widget ignores close events.
     */
    bool getIgnoresClose(){ return ignoresClose; }

    /**
      * Setter function of the ignoresClose attribute.
      * @param val - Set to true if you want the plugin to ignore close events.
      */
    void setIgnoresClose(bool val){
        ignoresClose = val;
    }
signals:
    /**
      * onClose signal is emitted at the start of a close event.
      */
    void onClose();
    /**
      * onResize signal is emitted after a resize event.
      */
    void onResize(QResizeEvent *event);
};

/**
  * CorePlugin is the base class of all used plugins. It implements most of the common methods.
  */
class CorePlugin : public QObject, public CoreInterface
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)


protected:
    //has the generated or received NMEAString
    QString NMEAString;
    /**
      * Contains the version of the plugin. Must be set in the constructor of a subclass plugin.
      * Default version is 1.0.0.
      */
    QString version;
    bool stopped;
    //the timer which generates the NMEAString
    QTimer *timer;
    //delay of the timer in milliseconds
    int timerDelay;

    int timerMultiplier;
    /**
      * GUI widget which contains the layout and inside the layout the GUI elements.
      * Must be used in the constructor to properly initialize the GUI.
      */
    PluginQWidget *plugin_widget;

    /**
      * Contains the name of the plugin. Must be set in the constructor of a subclass plugin.
      */
    QString pluginName;
    /**
      * Contains the message ids of the produced messages. Must be set in the constructor of a subclass plugin.
      * Format is ID1;ID2;ID3
      */
    QString produceIDs;
    /**
      * Contains the message ids of the parsed messages. Must be set in the constructor of a subclass plugin.
      * Plugin will not parse any ids that are not defined in parseIDs.
      * Format is ID1;ID2;ID3
      */
    QString parseIDs;
public:
    /**
      * Constructor of the CorePlugin class.
      */
    CorePlugin();
    /**
      * Virtual destructor of the CorePlugin class.
      */
    virtual ~CorePlugin();
    /**
      * Creates the plugin's GUI and it's elements
      */
    virtual void CreateGUI();
    /**
      * Shows the GUI of the plugin
      */
    virtual void showPlugin();
    /**
     * Hides the GUI of the plugin
     */
    virtual void hidePlugin();
    /**
      * Adds the plugin to the given layout. The plugin will no longer be on it's own window.
      * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
      */
    virtual void addPluginToLayout(QLayout *layout);
    /**
      * Removes the plugin from the given layout. The plugin will be on it's own window.
      * @param layout - Pointer to a QLayout from which the plugin widget (UI) will be removed
      */
    virtual void removePluginFromLayout(QLayout *layout);
    /**
      * Sets the delay of the timer
      * @param tDelay - delay of the timer in milliseconds
      */
    virtual void setTimerDelay(int tDelay);
    /**
      * Sets the timer multiplier of the plugin. Depending of the plugin causes different behaviour.
      * @param multiplier New multipler for the plugin.
      */
    virtual void setTimerMultiplier(int multiplier);
    /**
      * Implement getName.
      * Returns the name of the plugin
      * @return name of the plugin
      */
    virtual QString getName();
    /**
      * Implements getproduceIDs.
      * Returns the message IDs of the messages the plugin produces
      * @return message IDs separated by a semicolong (;).
      */
    virtual QString getProduceIDs();
    /**
      * Implements getParseIDs.
      * Returns the message IDs of the messages the plugin can parse
      * @return message IDs separated by a semicolong (;).
      */
    virtual QString getParseIDs();
    /**
      * Returns the widget which the plugin uses for it's GUI. Can be used to extend the plugin's
      * UI functionality beyond the basic interface.
      */
    PluginQWidget* getWidget();

    //generates a 2-character checksum for the string (no * symbol included)
    /**
      * Generates a 2-character checksum for the string (no * symbol included)
      * @param nString - String for which the checksum is generated
      * @return generated checksum as QString.
      */
    static QString generateChecksum(QString & nString);
    /**
      * Getter method for stopped attribute.
      * @return returns true if the internal timer is stopped.
      */
    virtual bool isStopped();
    /**
      * Implements the start method.
      * Starts the internal timer.
      */
    virtual void start();//todo, move start and stop, show and hide to slot
    /**
      * Implements the stop method.
      * Stops the internal timer.
      */
    virtual void stop();
    /**
      * Validates the string and returns the message ID (3 symbols).
      * @param text - Reference to a QString that contains the NMEA string.
      * @return QString containing the message ID for a valid NMEA string. Returns
      * empty string if the NMEA string is invalid.
      */
    static QString validateNMEA(const QString &text);

    /**
      * Tests if the plugin supports (parses) the message ID.
      * @param m_ID - Reference to the string containing the message id and nothing else.
      * @param excludeProduced - Set to true if you want to exclude the message IDs the plugin produces.
      * @return true if the plugin supports the message id (parsing of the message is supported)
      */
    virtual bool supports(const QString &m_ID, bool excludeProduced);

    /**
      * Parses the message ID from a properly formatted NMEA string.
      * @param nmea_ - Reference to the string that contains the NMEA string
      * @return QString containing the 3 symbols of the message id.
      */
    inline static QString parseMessageID(const QString &nmea_){
        return nmea_.left(6).right(3);
    }

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
protected slots:
    /**
      * Slot that is called by the timer. Implemented in subclass plugins.
      * Will calculate a new NMEA-string based on the available variables.
      */
    virtual void updateNMEAString() = 0;
signals:
    //emitted when timer generates a new string
    /**
      * Emitted when timer generates a new string that does not equal the previous
      * generated string.
      * @param text generated string.
      */
    void newNMEAString( const QString & text);
};


#endif // COREPLUGIN_H
