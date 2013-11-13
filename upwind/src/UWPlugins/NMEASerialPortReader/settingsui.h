#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"

class serialPort;

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

    /** Load the computer Settings
      */
    void loadSettings();
    /** Set the serialPort vinculated with this SettingsUI class
      */
    void setReader(serialPort* reader);
    /** Setup the UI settings
      * @param s - The settings from the serialPort Settings User Interface
      */
    void setupSettings(Settings* s);

    /** Save the settings with new parameters that the user has chosen
      */
    void updateSettings();
    void serialPortNotConnected();
    void serialPortConnected();
    void isSerialPortConnected(bool connected);
    bool readWhenStarted();
    void setSettings();
    void closePort();



private:
    Ui::SettingsUI *ui;
    serialPort* reader;
    Settings *settings;
    QStringList baudRates;
    QStringList parityList;
    QStringList stopBitsList;
    QStringList byteSizeList;
    QStringList portNames;

private slots:
    /** Try to open the reader Port with the UISettings preferences
      */
    void on_checkBox_autoconnect_clicked();
    void on_comboBox_byteSize_activated(QString);
    void on_comboBox_stopBits_activated(QString);
    void on_comboBox_parity_activated(QString);
    void on_comboBox_baudRate_activated(QString);
    void on_comboBox_portName_activated(QString);
    void openPort();

    /** Start the reader thread
      */
    void startReading();

    /** Stop displaying the Settings Log
      */
    void endTest();

    /** Save the settings
      */
    void save();

    /** Show the NMEAStrings in the Settings log
      */
    void readData(const QString & data);

signals:
    void canRead();
    void stopReading();

};

#endif // SETTINGSUI_H
