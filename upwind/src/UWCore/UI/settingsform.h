#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QTabWidget>

namespace Ui{
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();
    void setPluginSettings();

signals:
    void goBack();

public slots:
    void back();
    void showSettings();

private:
    Ui::SettingsForm *ui;
    QTabWidget *tabWidget;

    QWidget* loggerWidget;
    QWidget* chartProviderWidget;
    QWidget* viewRendererWidget;
    QWidget* nmeaReaderWidget;
    QWidget* upWindSceneWidget;
    QWidget* routeManagerWidget;

private slots:
    void on_pushButton_clicked();
};

#endif // SETTINGSFORM_H
