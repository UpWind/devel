#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"

class SerialPortReader;

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

    void initialize(const QStringList &serialPortNames,
                    const QStringList &baudRates,
                    const QStringList &dataBitsOptions,
                    const QStringList &parityOptions,
                    const QStringList &stopBitsOptions);

    /** Setup the UI settings
      * @param s - The settings from the serialPort Settings User Interface
      */
    void setupSettings(Settings* s);

    /** Save the settings with new parameters that the user has chosen
      */
    void updateSettings();
    void serialPortNotConnected();

public slots:
    void serialPortOpened();
    void serialPortClosed();
    void serialPortError(const QString &errorMsg);
    void serialPortSettingsApplied();

    /** Append information to log view
      */
    void appendDataToLogView(const QString &data);

    void serialPortBaudRateChanged(const QString &baudRate);
    void serialPortParityChanged(const QString &parity);
    void serialPortStopBitsChanged(const QString &stopBits);
    void serialPortDataBitsChanged(const QString &dataBits);



signals:
    void toggleSerialPortRequested(const QString &text);
    void serialPortSettingsChanged(const QString &portName,
        const QString &baudRate,
        const QString &dataBits,
        const QString &parity,
        const QString &stopBits);

private:
    Ui::SettingsUI *ui;

private slots:
    /** Try to open serial port or close it if it's already open
      */
    void toggleSerialPortConnection();

    /**
     * @brief applySerialPortSettings
     * Emits serialPortSettingsChanged() signal with current
     * serial port settings.
     */
    void applySerialPortSettings();

    void on_checkBox_autoconnect_clicked();
    void on_comboBox_dataBits_activated(QString);
    void on_comboBox_stopBits_activated(QString);
    void on_comboBox_parity_activated(QString);
    void on_comboBox_baudRate_activated(QString);
    void on_comboBox_portName_activated(QString);
};

#endif // SETTINGSUI_H
