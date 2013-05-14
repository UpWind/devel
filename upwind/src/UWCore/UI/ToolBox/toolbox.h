#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <QtGui/QWidget>
#include <QtGui/QToolBox>
#include <QtGui/QtGui>
#include <QListIterator>
#include <QtGui/QAbstractButton>

#include "../../ui_toolBox.h"
#include "tab.h"

#define MAX_CHAR 8

class ToolBox : public QToolBox
{
    Q_OBJECT

public:
    ToolBox(QWidget *parent = 0);
    ~ToolBox();
    void loadStyleSheet();
    void hideRotateButtons();
    void initializeInstruments();
    void initializeLayerBoxes();
    void add_profile(QString *profile, QString * path);
    bool remove_profile(QString *profile);

private:
    bool debug;
    Ui::ToolBox ui;

    QList<Tab *> list; // It is a list formed by means of Tabs items
    QString * actual_profile;

signals:
    // View & navigation
    void zoomInS();
    void zoomOutS();
    void rotateLeftS();
    void rotateRightS();
    void changeCursor();
    void zoomChart();
    void zoomBoat();
    void unsetDestiny();
    void zoomToolActivated(bool);
    void followBoat(bool);

    // Instruments
    void compassStateChangedS(int);
    void windStateChangedS(int);
    void gpsCoordsStateChangedS(int);
    void boatStateChangedS(int);
    void angleToolStateChangedS(int);

    // Simulator
    void simModeChanged(bool);
    void settingSimulatedRoute(bool);
    void settingSimulatedPos(bool);
    void simHeadChanged(float);
    void simWindSpeedChanged(float);
    void simTWDChanged(float);
    void compassSpinBoxMovedS(int);
    void requestUpdate();
    void drawRoute(bool);

    void brightnessChange(int bright);

    void compassCheckChange(bool checked);
    void angleCheckChange(bool checked);
    void gpsCheckChange(bool checked);
    void windCheckChange(bool checked);

    void load_profile(QString profile);

public slots:
    void loadPolarDiagramList( int to_load = -1);
    void compassCBChanged(int state);
    void windCBChanged(int state);
    void gpsCoordsCBChanged(int state);
    void boatCBChanged(int state);
    void angleToolCBChanged(int state);

    void obstacles_CBChanged(int state);
    void generarea_rCBChanged(int state);
    void deptharea_rCBChanged(int state);
    void lake_rCBChanged(int state);
    void spoilgrnd_rCBChanged(int state);
    void limit_rCBChanged(int state);
    void builtupare_rCBChanged(int state);
    void fwayarea_rCBChanged(int state);
    void survqual_rCBChanged(int state);
    void building_rCBChanged(int state);
    void dock_rCBChanged(int state);
    void depthcont_lCBChanged(int state);
    void coastline_lCBChanged(int state);
    void closinglne_lCBChanged(int state);
    void navigline_lCBChanged(int state);
    void ferry_lCBChanged(int state);
    void limit_lCBChanged(int state);
    void heightcont_lCBChanged(int state);
    void leadinglne_lCBChanged(int state);
    void lockndam_lCBChanged(int state);
    void pipeline_lCBChanged(int state);
    void submcable_lCBChanged(int state);
    void roadnrailw_lCBChanged(int state);
    void bridge_lCBChanged(int state);
    void transmline_lCBChanged(int state);
    void river_lCBChanged(int state);
    void shlnecon_lCBChanged(int state);
    void navaid_pCBChanged(int state);
    void limit_pCBChanged(int state);
    void towernmast_pCBChanged(int state);
    void signsound_pCBChanged(int state);
    void ctext_pCBChanged(int state);
    void vegetation_pCBChanged(int state);
    void moorfacil_pCBChanged(int state);
    void natseabed_pCBChanged(int state);
    void obstruct_pCBChanged(int state);
    void harbour_pCBChanged(int state);
    void marservice_pCBChanged(int state);
    void wreck_pCBChanged(int state);
    void building_pCBChanged(int state);
    void rock_pCBChanged(int state);
    void checkpoint_pCBChanged(int state);
    void fortstruct_pCBChanged(int state);
    void lightsCBChanged(int state);
    void brightnessChanged(int bright);
    void setCompassChecked(bool checked);
    void setAngleChecked(bool checked);
    void setgpsChecked(bool checked);
    void setWindChecked(bool checked);

    void next_tab();
    void previous_tab();
    void change_profile_menu(QAbstractButton*);

private slots:
    void instrument_Clicked(bool value);
    void on_routeButton_clicked();
    void on_routeComboBox_currentIndexChanged(int index);
    void on_destinyButton_clicked();

    void on_polarDiagramNamesComboBox_currentIndexChanged(int index);
    void on_obstaclesButton_clicked();
    void on_lockButton_clicked();
    void on_simCompassSpinBox_valueChanged(int );
    void on_simGeneralButton_clicked();
    void on_simRouteButton_clicked();
    void on_simPosButton_clicked();
    void on_zoomToolButton_clicked();
};

#endif /* TOOLBOX_H_ */
