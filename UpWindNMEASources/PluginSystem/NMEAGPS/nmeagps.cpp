#include "nmeagps.h"
#include <QWebView>
#include <QWebFrame>
#include <QWidget>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QToolTip>

#include <QDebug>

#include "dms.h"
#include "nmeagps.h"
#include "time.h"
#include <math.h>
#include "settingsmanager.h"
#include "ui_simulator.h"
#include "boat.h"

QString invalidCoordinatesMessage =  "Use the correct format: \n\n"
                                     "Latitude: \n DDMM.MMM \n\n"
                                     "Longnitude: \n DDDMM.MMM \n\n"
                                     "D = degrees, M = minutes";

int latCursorPos = 0;
int longCursorPos = 0;

NMEAGPS::NMEAGPS(): ui(new Ui::NMEAGPS)
{
    CreateGUI();
    setTimerDelay(boat.updateDelay);
    this->pluginName = "NMEA Simulator";
    this->produceIDs = "RMC";
    this->parseIDs = "RMC;HDG;MWD";
    plugin_widget->setWindowTitle(pluginName);
}

NMEAGPS::~NMEAGPS(){
    delete ui;
}

void NMEAGPS::CreateGUI(){
    ui->setupUi(this->getWidget());

    ui->gemap->setUrl(QUrl("file:///" +
                       QApplication::applicationDirPath() +
                       "/plugins/index.html"));

    nsbutts = new QButtonGroup();
    nsbutts->addButton(ui->northButton);
    nsbutts->addButton(ui->southButton);

    webutts = new QButtonGroup();
    webutts->addButton(ui->westButton);
    webutts->addButton(ui->eastButton);

    QWebPage *page = ui->gemap->page();
    frame = page->mainFrame();

    connect(ui->simulatorButton, SIGNAL(clicked()), this, SLOT(toggleSimulation()));
    connect(ui->selectXMLFileButton,SIGNAL(clicked()),this, SLOT(setXMLFile()));
    connect(ui->latLineEdit,SIGNAL(textEdited(const QString &)),this, SLOT(setCoordinates()));
    connect(ui->longLineEdit,SIGNAL(textEdited(const QString &)),this, SLOT(setCoordinates()));
    connect(ui->northButton,SIGNAL(clicked()),this, SLOT(setCoordinates()));
    connect(ui->southButton,SIGNAL(clicked()),this, SLOT(setCoordinates()));
    connect(ui->westButton,SIGNAL(clicked()),this, SLOT(setCoordinates()));
    connect(ui->eastButton,SIGNAL(clicked()),this, SLOT(setCoordinates()));

    //ui->setupUi(this);
    connect(frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachGPSPluginToPage()) );

    //take away the spinnaker check box
    connect(ui->spinnaker,SIGNAL(stateChanged(int)),this,SLOT(spinnakerChanged(int)));

    ui->latLineEdit->setInputMask("9999.999");
    ui->longLineEdit->setInputMask("99999.999");

}

void NMEAGPS::attachGPSPluginToPage()
{
    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("gps", this );
}

void NMEAGPS::toggleSimulation() {
    if (boat.isMoving){
        stopMoving();

    } else {
        startMoving();
    }
}

void NMEAGPS::startMoving() {
    if(boat.fileName != ""){
        boat.isMoving = true;
        ui->latLineEdit->setEnabled(false);
        ui->longLineEdit->setEnabled(false);
        ui->northButton->setEnabled(false);
        ui->southButton->setEnabled(false);
        ui->westButton->setEnabled(false);
        ui->eastButton->setEnabled(false);
        ui->simulatorButton->setText("Stop simulation");
    }
    else{
        boat.isMoving = false;
        QMessageBox msgBox;
        msgBox.setText("Select a Boat Speed XML file.");
        msgBox.exec();
    }
}

void NMEAGPS::setTimerDelay(int tDelay){
    CorePlugin::setTimerDelay(updateDelay);
}

void NMEAGPS::stopMoving() {
    boat.isMoving = false;
    ui->latLineEdit->setEnabled(true);
    ui->longLineEdit->setEnabled(true);
    ui->northButton->setEnabled(true);
    ui->southButton->setEnabled(true);
    ui->westButton->setEnabled(true);
    ui->eastButton->setEnabled(true);
    ui->simulatorButton->setText("Start simulation");
}

void NMEAGPS::updateNMEAString(){
    if(boat.calculateEndPoint() != -1)
        updateEndPointUI();
    else{
        boat.isMoving = false;
        ui->simulatorButton->setText("Start simulation");
    }

}

void NMEAGPS::disableControls(){
    ui->controlsFrame->hide();
}

void NMEAGPS::setCoordinates() {

    if (validateCoordinates()){
        boat.latitude = Boat::latDecimal(ui->latLineEdit->text());
        if (ui->southButton->isChecked())
            boat.latitude = boat.latitude * -1;
        boat.longnitude = Boat::longDecimal(ui->longLineEdit->text());
        if (ui->westButton->isChecked())
            boat.longnitude = boat.longnitude * -1;
        coordinatesChanged();
        setBoatMapLocation(boat.latitude, boat.longnitude);

        ui->latLineEdit->setCursorPosition(latCursorPos);
        ui->longLineEdit->setCursorPosition(longCursorPos);
    }
}

void NMEAGPS::setBoatDirection(int bearing){

    boat.boatDirection = bearing;
    q.setProperty("boatDirection", boat.boatDirection);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");

    QString newString = "$IIHDG,"+ QString::number(bearing) +",,,7.1,W";
    newString = newString + "*" + generateChecksum(newString);
    //if(newString != NMEAString){
        NMEAString = newString;
        emit newNMEAString(NMEAString);
    //}
}

bool NMEAGPS::validateCoordinates() {

    QString lat = ui->latLineEdit->text();
    QString lng = ui->longLineEdit->text();

    if (lat.length() != 8) {
        QToolTip::showText(ui->latLineEdit->mapToGlobal(QPoint()), invalidCoordinatesMessage);
        return false;
    }

    if (lng.length() != 9) {
        QToolTip::showText(ui->longLineEdit->mapToGlobal(QPoint()), invalidCoordinatesMessage);
        return false;
    }

    int latdeg = lat.left(2).toInt();
    int latmin = lat.mid(2, 2).toInt();
    int latsec = lat.right(3).toInt();

    if (latdeg > 89 || latdeg < 0 || latmin > 59 || latmin < 0|| latsec > 999 || latsec < 0) {
        QToolTip::showText(ui->latLineEdit->mapToGlobal(QPoint()), invalidCoordinatesMessage);
        //QMessageBox::warning(this->getWidget(), "Invalid coordinates", invalidCoordinatesMessage);
        return false;
    }

    int lngdeg = lng.left(3).toInt();
    int lngmin = lng.mid(3, 2).toInt();
    int lngsec = lng.right(3).toInt();

    if (lngdeg > 179 || lngdeg < 0 || lngmin > 59 || lngdeg < 0 || lngsec > 999 || lngdeg < 0) {
        QToolTip::showText(ui->longLineEdit->mapToGlobal(QPoint()), invalidCoordinatesMessage);
        //QMessageBox::warning(this->getWidget(), "Invalid coordinates", invalidCoordinatesMessage);
        return false;
    }

    latCursorPos = ui->latLineEdit->cursorPosition();
    longCursorPos = ui->longLineEdit->cursorPosition();

    QToolTip::hideText();
    return true;
}

//gets coordinates from marker on the map
void NMEAGPS::getCoordsFromMap() {

    if(!boat.isMoving) {

        QVariant lat = ui->gemap->page()->mainFrame()->evaluateJavaScript("getLat();");
        QVariant lng = ui->gemap->page()->mainFrame()->evaluateJavaScript("getLng();");
        boat.latitude = lat.toDouble();
        boat.longnitude = lng.toDouble();

        QString latstr = QString::fromStdString(DegreesMinutesSecondsLat(lat.toDouble(), 0));
        QString lngstr = QString::fromStdString(DegreesMinutesSecondsLng(lng.toDouble(), 0));
        QStringList latstrlist = latstr.split(",");
        QStringList lngstrlist = lngstr.split(",");

        ui->latLineEdit->setText(latstrlist.at(0));
        ui->longLineEdit->setText(lngstrlist.at(0));

        if(latstrlist.at(1) == "N")
            ui->northButton->setChecked(true);
        else
            ui->southButton->setChecked(true);

        if(lngstrlist.at(1) == "W")
            ui->westButton->setChecked(true);
        else
            ui->eastButton->setChecked(true);
    }
}

void NMEAGPS::coordinatesChanged() {

    QString str = "$IIRMC,";
    str += ",A,";
    str += ui->latLineEdit->text() + ",";

    if(boat.latitude>=0)
        str+= "N,";
    else{
        str+= "S,";
    }
    str += ui->longLineEdit->text() + ",";

    if(boat.longnitude<0){
        str+= "W,";
    }
    else
        str+= "E,";

    str +=QString::number(boat.boatSpeed)+",";
    str +="0.0,"; //track made good ???
    str +=","; //date
    str = str + "*" + generateChecksum(str);

    if(NMEAString!=""){
        emit newNMEAString(NMEAString);
    }

    if(str != NMEAString){
        NMEAString = str;
        emit newNMEAString(NMEAString);
    }

    //setBoatMapLocation(lat, lng);
}

//called by controller in demo mode (after loading XML-file)
//update the UI to match the new NMEA string
void NMEAGPS::parseNMEAString( const QString & text){

    if(this->parseMessageID(text) == "HDG"){
        QStringList strList = text.split(",");
        boat.boatDirection = ((QString)strList.at(1)).toDouble();
        q.setProperty("boatDirection", boat.boatDirection);
        ui->gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");
    }

    /*$IIMWD,316.00,T,316.00,M,11.2967,N,5.81152,M*44*/
    if(this->parseMessageID(text) == "MWD"){
        QStringList pList = text.split(",");
        QString angle = pList.at(3).split(".").at(0);
        QString speed = pList.at(5);

        boat.windDirection = angle.toDouble();
        boat.windSpeed = speed.toDouble();

        q.setProperty("windDirection", boat.windDirection);
        ui->gemap->page()->mainFrame()->evaluateJavaScript("setWindAngle();");
    }

    //$IIRMC,,A,2908.38,N,16438.18,E,0.0,0.0,251210,,*06
    if (this->parseMessageID(text) == "RMC"){
        QStringList strList = text.split(",");

        double latdecimal = boat.latDecimal((QString)strList.at(3));
        double lngdecimal = boat.longDecimal((QString)strList.at(5));

        ui->latLineEdit->setText((QString)strList.at(3));

        ui->longLineEdit->setText((QString)strList.at(5));

        if (strList.at(4) == "S") {
            latdecimal = latdecimal * -1;
            ui->southButton->setChecked(true);
        } else {
            ui->northButton->setChecked(true);
        }

        if (strList.at(6) == "W") {
            lngdecimal = lngdecimal * -1;
            ui->westButton->setChecked(true);
        } else {
            ui->eastButton->setChecked(true);
        }

        ui->resultLabel->setText(QString::number(strList.at(7).toDouble(), 'f', 2) + " knots");
        setBoatMapLocation(latdecimal, lngdecimal);
    }
}

void NMEAGPS::setBoatMapLocation(double lat, double lng){

    q.setProperty("x", lat);
    q.setProperty("y", lng);
    //q.setProperty("boatDirection", boatDirection);
    //q.setProperty("windDirection", windDirection);

    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("q", &q);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("setPos();");
    //gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");
    //gemap->page()->mainFrame()->evaluateJavaScript("setWindAngle();");
}


void NMEAGPS::updateEndPointUI(){

    if (boat.isMoving){

      ui->resultLabel->setText(QString::number(boat.boatSpeed, 'f', 2) + " knots");

      if (boat.latitude < 0)
          ui->southButton->setChecked(true);
      else
          ui->northButton->setChecked(true);

      if (boat.longnitude < 0) {
          ui->westButton->setChecked(true);
      } else {
          ui->eastButton->setChecked(true);
      }

      QString latstr = QString::fromStdString(DegreesMinutesSecondsLat(boat.latitude, 0));
      QString lngstr = QString::fromStdString(DegreesMinutesSecondsLng(boat.longnitude, 0));
      ui->latLineEdit->setText(latstr.left(8));
      ui->longLineEdit->setText(lngstr.left(9));
      coordinatesChanged();
      setBoatMapLocation(boat.latitude, boat.longnitude);

  }
}

void NMEAGPS::spinnakerChanged(int state){
    boat.usingSpinnaker = Qt::Checked == state;
}

void NMEAGPS::setXMLFile(bool dialog){
    QMessageBox msgBox;
    int fileOpened = boat.openFile(dialog);
    ui->selFileLabel->setText(boat.fileName);
    if(fileOpened == 1){
        msgBox.setText("Cannot open XML file.");
        msgBox.exec();
        ui->boatNameLabelValue->setText("");
        ui->regNumLabelValue->setText("");
        ui->selFileLabel->setText("");
        return;
    }
    else if(fileOpened == 2){
        msgBox.setText("Not a valid XML file.");
        msgBox.exec();
        ui->boatNameLabelValue->setText("");
        ui->regNumLabelValue->setText("");
        ui->selFileLabel->setText("");
        return;
    }
    else if (fileOpened == 3){
        msgBox.setText("Not a UpWind XML.");
        msgBox.exec();
        ui->boatNameLabelValue->setText("");
        ui->regNumLabelValue->setText("");
        ui->selFileLabel->setText("");
        return;
    }
    else if (fileOpened != 4){
        // This method takes all the information we could need to know
        // of the boat, and set it into the simulation plugin.
        boat.tabla.clear();
        boat.tablaSpinnaker.clear();

        getBoatInfo();

        int control = 0;
        QDomNode n = boat.element.firstChild();
        QDomNodeList nodeList;
        bool done = false;

        // Get Speed nodes to handle it easier
        while( !n.isNull() && control < 1000){
            control ++;
            QDomElement e = n.toElement();
            if( !e.isNull() ){
                if( e.tagName() == "polar_diagrams" && !done){
                    nodeList = e.childNodes();
                    boat.tabla = nodeList.at(0);
                    if (nodeList.size()>1){
                        ui->spinnaker->show();
                        boat.tablaSpinnaker = nodeList.at(1);
                    }else{
                        ui->spinnaker->setChecked(false);
                        ui->spinnaker->hide();
                    }
                }
            }
           n = n.nextSibling();
       }
        if(control >= 1000){
            msgBox.setText("XML reading stopped after 1000 nodes");
            msgBox.exec();
            return;
        }
    }
}

void NMEAGPS::getBoatInfo(){
    QDomNode n = boat.element.firstChild();
    int control = 0;
    while( !n.isNull() && control < 1000){
        control ++;
        QDomElement e = n.toElement();
        if( !e.isNull() ){
            if(e.tagName() == "name")
                ui->boatNameLabelValue->setText(e.text());
            if(e.tagName() == "register_number")
                ui->regNumLabelValue->setText(e.text());
//            if(e.tagName() == "model")
//                ui->modelLabel->setText(e.text());
//            if(e.tagName() == "owner")
//                ui->ownerLabel->setText(e.text());
            //more boat info...
        }
        n=n.nextSibling();
    }
    if(control >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes");
        msgBox.exec();
        return;
    }
}

void NMEAGPS::saveTo(SettingsManager &sm){
    CorePlugin::saveTo(sm);
    sm.saveProperty("polarXml", boat.fileName);
}

QString NMEAGPS::loadFrom(SettingsManager &sm){
    QString fileVer = CorePlugin::loadFrom(sm);
    boat.fileName = sm.loadProperty("polarXml", "");
    QFile file(boat.fileName);
    if(file.exists())
        setXMLFile(false);
    return fileVer;
}

void NMEAGPS::setTimerMultiplier(int multiplier){
    CorePlugin::setTimerMultiplier(multiplier);
    boat.setTimerMultiplier(multiplier);
}
