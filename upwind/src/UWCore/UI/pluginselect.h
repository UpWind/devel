#ifndef PLUGINSELECT_H
#define PLUGINSELECT_H

#include <QWidget>
#include <QComboBox>
#include <QTreeWidgetItem>

namespace Ui {
class PluginSelect;
}

class PluginSelect : public QWidget
{
    Q_OBJECT

public:
    explicit PluginSelect(QWidget *parent = 0);
    ~PluginSelect();
    void setupUI();
    void newPluginSelected(const QString &pluginName);
    void saveSettings();
    void updateTextBox();
    void updateTreeView(const QString &pluginName);
    void constructTreeView();

private:
    Ui::PluginSelect *ui;
    QComboBox* rendererBox;
    QComboBox* nmeaReaderBox;
    QComboBox* chartProviderBox;
    QComboBox* routeManagerBox;
    QComboBox* loggerBox;
    QComboBox* upwindSceneBox;
    QIcon* selectedIcon;
    QIcon* notSelectedIcon;

    QTreeWidgetItem *chartProviderItem;
    QTreeWidgetItem *rendererItem;
    QTreeWidgetItem *nmeaReaderItem;
    QTreeWidgetItem *routeManagerItem;
    QTreeWidgetItem *loggerItem;
    QTreeWidgetItem *upwindSceneItem;
    QTreeWidgetItem *instrumentItem;

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_routeManagerBox_activated(QString );
    void on_loggerBox_activated(QString );
    void on_upwindSceneBox_activated(QString );
    void on_chartProviderBox_activated(QString );
    void on_rendererBox_activated(QString );
    void on_nmeaReaderBox_activated(QString );
    void on_nmeaReaderBox_currentIndexChanged(QString );
    void on_routeManagerBox_textChanged(QString );
    void on_loggerBox_textChanged(QString );
    void on_upwindSceneBox_textChanged(QString );
    void on_chartProviderBox_textChanged(QString );
    void on_nmeaReaderBox_textChanged(QString );
    void on_rendererBox_textChanged(QString );
};

#endif // PLUGINSELECT_H
