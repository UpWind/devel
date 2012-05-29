#include "multisimulation.h"
#include "ui_multiwidget.h"
#include "settingsmanager.h"
#include "coreplugin.h"
#include "plugincontroller.h"
#include <QFile>
#include "client.h"
#include <QMessageBox>
#include <QStringList>
#include <QWebFrame>

Multisimulation::Multisimulation():
        ui(new Ui::MultiWidget), server(0), simulationStatus(NOTCONNECTED)
{
    this->pluginName = "NMEA Multisimulation";
    this->produceIDs = "???";
    this->parseIDs = "???";

    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName);
    resetSimulation();
}

Multisimulation::~Multisimulation(){
    delete ui;
    //delete allocated variables
}

void Multisimulation::clientDisconnected(QTcpSocket *socket){
    Client *client = (Client*)socket;
    if(simulationStatus == READY){
        QListWidgetItem *item = 0;
        for(int i=0; i<competitors.count(); i++){
            item = competitors.at(i);
            if(item->text() == client->clientName){
                delete ui->listWidget->takeItem(i);
                competitors.removeOne(item);
                logMessage("Client " + client->clientName + " disconnected");
                break;
            }
        }
    }
}

void Multisimulation::attachGPSPluginToPage()
{
    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("gps", this);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("setMultiplayerMode();");
}

void Multisimulation::setXMLFile(Boat *boat, bool dialog){
    QMessageBox msgBox;
    int fileOpened = boat->openFile(dialog);
    if(fileOpened == 1){
        msgBox.setText("Cannot open XML file.");
        msgBox.exec();
        return;
    }
    else if(fileOpened == 2){
        msgBox.setText("Not a valid XML file.");
        msgBox.exec();
        return;
    }
    else if (fileOpened == 3){
        msgBox.setText("Not a UpWind XML.");
        msgBox.exec();
        return;
    }
    else if (fileOpened != 4){

        int control = 0;
        QDomNode n = boat->element.firstChild();
        QDomNodeList nodeList;
        bool done = false;

        // Get Speed nodes to handle it easier
        while( !n.isNull() && control < 1000){
            control ++;
            QDomElement e = n.toElement();
            if( !e.isNull() ){
                if( e.tagName() == "polar_diagrams" && !done){
                    nodeList = e.childNodes();
                    boat->tabla = nodeList.at(0);
                    if (nodeList.size()>1){
                        boat->tablaSpinnaker = nodeList.at(1);
                        boat->spinnakerAvailable = true;
                        boat->usingSpinnaker = true;
                    }else{
                        boat->spinnakerAvailable = false;
                        boat->usingSpinnaker = false;
                        //boat->tablaSpinnaker = nodeList.at(0);
                    }
                }
            }
            n = n.nextSibling();
        }
        ui->spinnakerBox->setVisible(boat->spinnakerAvailable);
        ui->spinnakerBox->setChecked(boat->usingSpinnaker);
        if(control >= 1000){
            msgBox.setText("XML reading stopped after 1000 nodes");
            msgBox.exec();
            return;
        }
        logMessage("Loaded \"" + boat->fileName + "\"");
    }
}

void Multisimulation::CreateGUI(){
    ui->setupUi(plugin_widget);
    ui->spinnakerBox->setVisible(false);

    connect(ui->startServerButton, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(ui->startSimulationButton, SIGNAL(clicked()), this, SLOT(startSimulation()));
    connect(ui->selectWinnerButton, SIGNAL(clicked()), this, SLOT(selectWinner()));
    connect(ui->resetSimulationButton, SIGNAL(clicked()), this, SLOT(resetSimulation()));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(indexChanged(int)));
    connect(ui->XMLFileButton,SIGNAL(clicked()),this, SLOT(setBoatXML()));
    connect(ui->spinnakerBox, SIGNAL(stateChanged(int)), this, SLOT(spinnakerChanged(int)));

    ui->windFrame->layout()->addWidget(windInstrument.getWidget());
    windInstrument.getWidget()->setStyleSheet(windInstrument.getWidget()->styleSheet() + "background: rgba(225, 225, 225);");
    connect(&windInstrument, SIGNAL(newNMEAString(QString)), this, SLOT(newWindInfo(QString)));

    QString filePath(QApplication::applicationDirPath() +
                   "/plugins/index.html");
    if(QFile::exists(filePath)){
        ui->gemap->setUrl(QUrl("file:///" + filePath));
        logMessage("Loaded index.html successfully");
    }else
        logMessage("Failed to load \"" + filePath + "\"");

}

void Multisimulation::spinnakerChanged(int newStatus){
    int index = ui->listWidget->currentRow();
    if(index <0)
       return;
    if(!server)
        return;
    Client *client = server->getClient(ui->listWidget->item(index)->text());
    if(client){
        bool usingSpinnaker = newStatus == Qt::Checked;
        if(boatFromClient(client)->usingSpinnaker != usingSpinnaker)
            logMessage( (usingSpinnaker? "Spinnaker enabled for boat " : "Spinnaker disabled for boat ")
                    + client->clientName);
        boatFromClient(client)->usingSpinnaker = (newStatus == Qt::Checked);
    }
}

void Multisimulation::doConnected(){
    connect(server, SIGNAL(newConnection(QTcpSocket*)), this, SLOT(clientConnected(QTcpSocket*)));
    connect(server, SIGNAL(newMessage(QString,int,Client*)), this, SLOT(messageReceived(QString,int,Client*)));
    connect(server, SIGNAL(closedConnection(QTcpSocket*)), this, SLOT(clientDisconnected(QTcpSocket*)));
    simulationStatus = READY;
    ui->gemap->show();
    ui->connectionGroupBox->hide();
    ui->controlsBox->show();
    ui->listWidget->show();
    windInstrument.showPlugin();
    plugin_widget->resize(oldWidth,oldHeight);

    attachGPSPluginToPage();
}

void Multisimulation::logMessage(const QString &msg){
    ui->logEdit->append(msg);
}

void Multisimulation::resetSimulation(){

    if(server){
        server->close();
        delete server;
        server = 0;
    }

    simulationStatus = NOTCONNECTED;

    ui->gemap->hide();
    ui->connectionGroupBox->show();

    ui->controlsBox->hide();
    ui->listWidget->hide();
    windInstrument.hidePlugin();

    ui->resetSimulationButton->setEnabled(false);
    ui->startSimulationButton->setEnabled(true);
    ui->XMLFileButton->show();

    ui->listWidget->clear();
    competitors.clear();

    ui->gemap->page()->mainFrame()->evaluateJavaScript("refresh();");

}

void Multisimulation::parseNMEAString(const QString & text, Client* client){
    if(this->parseMessageID(text) == "HDG"){
        QStringList strList = text.split(",");
        ClientInfo *info = ((ClientInfo*)client->extraData);
        info->boat.boatDirection = ((QString)strList.at(1)).toDouble();
        //q.setProperty("boatDirection", boatDirection);
        //gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");
    }else if(this->parseMessageID(text) == "MWD"){
        QStringList pList = text.split(",");
        double windDirection,windSpeed;
        windDirection = pList.at(3).split(".").at(0).toDouble();
        windSpeed = pList.at(5).toDouble();
        ClientInfo *info;
        foreach(Client* c, server->connections){
            info = ((ClientInfo*)c->extraData);
            info->boat.windDirection = windDirection;
            info->boat.windSpeed = windSpeed;
        }

    }
}

void Multisimulation::getCoordsFromMap(){
    int index = ui->listWidget->currentRow();
    if(index <0)
       return;

    Client *client = server->getClient(ui->listWidget->item(index)->text());
    if(!client)
        return;

    Boat *boat = boatFromClient(client);
    QVariant lat = ui->gemap->page()->mainFrame()->evaluateJavaScript("getBoatLat();");
    QVariant lng = ui->gemap->page()->mainFrame()->evaluateJavaScript("getBoatLng();");
    boat->latitude = lat.toDouble();
    boat->longnitude = lng.toDouble();
    boat->positionSelected = true;
}

void Multisimulation::setTimerDelay(int tDelay){
    CorePlugin::setTimerDelay(Boat::updateDelay);
}

void Multisimulation::parseNMEAString(const QString & text){

}

void Multisimulation::selectWinner(){
    int index = ui->listWidget->currentRow();
    if(index<0){
        QMessageBox::warning(this->getWidget(), tr("No winner selected"),
             tr("You must select a winner first."));
        return;
    }

    ui->selectWinnerButton->setEnabled(false);
    ui->resetSimulationButton->setEnabled(true);
    simulationStatus = ENDED;
    QString winnerName = ui->listWidget->item(index)->text();
    server->sendMessage(PacketID::SIMULATION_ENDED, winnerName);
    windInstrument.stop();
    stop();
    foreach(Client* c, server->connections){
        boatFromClient(c)->isMoving = false;
    }
    logMessage("Simulation is over. Winner was " + winnerName);
}

void Multisimulation::startSimulation(){
    if(server->connections.count() == 0){
        QMessageBox::warning(this->getWidget(), tr("No clients connected"),
             tr("You must wait for clients to connect first."));
        return;
    }
    if(xmlBoat.fileName.isEmpty()){
        QMessageBox::warning(this->getWidget(), tr("No polar diagram selected"),
             tr("You must select a polar diagram XML file first. Otherwise the speed of the boat can't be calculated."));
        return;
    }

    foreach(Client* c, server->connections){
        if(!boatFromClient(c)->positionSelected){
            QMessageBox::warning(this->getWidget(), tr("Place the boats"),
                 tr("All boats have not been placed yet."));
            return;
        }
    }

    ui->startSimulationButton->setEnabled(false);
    ui->selectWinnerButton->setEnabled(true);
    ui->XMLFileButton->hide();
    simulationStatus = RUNNING;
    server->sendMessage(PacketID::SIMULATION_STARTED, "");
    windInstrument.start();
    start();
    foreach(Client* c, server->connections){
        boatFromClient(c)->isMoving = true;
    }
    ui->gemap->page()->mainFrame()->evaluateJavaScript("startSimulation();");
}

void Multisimulation::startServer(){
    if(!server)
        server = new Server;
    server->listen(QHostAddress::Any, ui->portEdit->value());
    logMessage("Server started");
    doConnected();
}

void Multisimulation::updateNMEAString(){
    if(simulationStatus != RUNNING){
        stop();
        return;
    }
    ClientInfo *info;
    QString gpsInfo;
    foreach(Client* c, server->connections){
        info = ((ClientInfo*)c->extraData);
        info->boat.calculateEndPoint();
        setBoatMapLocation(c);
        gpsInfo += c->clientName + "," + QString::number(info->boat.latitude) +
                   "," + QString::number(info->boat.longnitude) + "*";
    }
    if(!gpsInfo.isEmpty()){
        gpsInfo.chop(1);
        server->sendMessage(MultiSimuID::GPSLIST, gpsInfo);
    }
}

Boat* Multisimulation::boatFromClient(Client *client){
    return &((ClientInfo *)client->extraData)->boat;
}

void Multisimulation::setBoatMapLocation(Client *client){

    Boat *boat = boatFromClient(client);
    q.setProperty("x", boat->latitude);
    q.setProperty("y", boat->longnitude);
    q.setProperty("index", client->clientID);

    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("q", &q);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("moveBoat();");
}

void Multisimulation::clientConnected(QTcpSocket* client){
    logMessage("Client connected");
    Client *c = ((Client*)client);
    ClientInfo *ci = new ClientInfo();
    c->extraData = ci;
    server->sendMessage(PacketID::CLIENTLIST, getNameList(), c);
    ci->boat.tabla = xmlBoat.tabla.cloneNode();
    ci->boat.tablaSpinnaker = xmlBoat.tablaSpinnaker.cloneNode();
    ci->boat.fileName = xmlBoat.fileName;
    ci->boat.setTimerMultiplier(timerMultiplier);
    ci->boat.usingSpinnaker = xmlBoat.usingSpinnaker;
    ci->boat.spinnakerAvailable = xmlBoat.spinnakerAvailable;
}

void Multisimulation::messageReceived(QString msg, int id, Client* client){

    QListWidgetItem *competitor;
    bool connectOK = true;

    switch(id){
    case PacketID::CLIENT_NAME:
        foreach(Client *c, server->connections){
            if(c->clientName == msg){//already exists
                server->sendMessage(PacketID::NAME_IN_USE, "", client);
                connectOK = false;
                break;
            }
        }
        if(connectOK){
            client->clientName = msg;
            server->sendMessage(PacketID::CLIENTJOINED, msg);

            competitor = new QListWidgetItem(msg);
            competitors.append(competitor);
            ui->listWidget->addItem(competitor);
            break;
        }
    case PacketID::NMEA_STRING:
        parseNMEAString(msg, client);
        break;
    }
}

QString Multisimulation::getNameList(){
    if(!server)
        return "";
    QString result;
    foreach(Client *client, server->connections)
        if(!client->clientName.isEmpty())
            result += client->clientName + ";";
    if(!result.isEmpty())
        result.chop(1);
    return result;
}

void Multisimulation::indexChanged(int index){
    if(index<0)
        return;
    Client *client = server->getClient(ui->listWidget->item(index)->text());
    if(client){
        q.setProperty("index", client->clientID);
        q.setProperty("boatName", client->clientName);
        ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("q", &q);

        ui->spinnakerBox->setVisible(boatFromClient(client)->spinnakerAvailable);
        ui->spinnakerBox->setChecked(boatFromClient(client)->usingSpinnaker);
    }
}

void Multisimulation::newWindInfo(QString msg){
    server->sendMessage(PacketID::NMEA_STRING, msg);
    parseNMEAString(msg, 0);
}

void Multisimulation::saveTo(SettingsManager &sm){
    CorePlugin::saveTo(sm);
    if(simulationStatus == NOTCONNECTED){
        sm.saveProperty("width2", QString::number(oldWidth));
        sm.saveProperty("height2", QString::number(oldHeight));
    }
    sm.saveProperty("polarXml", xmlBoat.fileName);
}

QString Multisimulation::loadFrom(SettingsManager &sm){
    QString fileVer = CorePlugin::loadFrom(sm);
    oldWidth = sm.loadProperty("width2", QString::number(this->getWidget()->width())).toInt();
    oldHeight = sm.loadProperty("height2", QString::number(this->getWidget()->height())).toInt();
    plugin_widget->resize(1,1);
    xmlBoat.fileName = sm.loadProperty("polarXml", "");
    QFile file(xmlBoat.fileName);
    if(file.exists())
        setXMLFile(&xmlBoat, false);
    else
        logMessage("Failed to load a polar diagram xml file. Please select a polar diagram.");
    return fileVer;
}

void Multisimulation::setBoatXML(){
    setXMLFile(&xmlBoat, true);
    if(!server)
        return;
    int index = ui->listWidget->currentRow();
    if(index <0)
       return;

    Client *client = server->getClient(ui->listWidget->item(index)->text());
    if(!client)
        return;

    Boat *boat = boatFromClient(client);
    boat->tabla = xmlBoat.tabla.cloneNode();
    boat->tablaSpinnaker = xmlBoat.tablaSpinnaker.cloneNode();
    if(!boat->fileName.isEmpty())
        logMessage("Polar diagram \"" + boat->fileName + "\" selected for boat " + client->clientName);
    else
        logMessage("No polar diagram selected for boat " + client->clientName);
    boat->usingSpinnaker = xmlBoat.usingSpinnaker;
    boat->spinnakerAvailable = xmlBoat.spinnakerAvailable;
}

void Multisimulation::setTimerMultiplier(int multiplier){
    CorePlugin::setTimerMultiplier(multiplier);
    if(server)
        foreach(Client *client, server->connections)
            boatFromClient(client)->setTimerMultiplier(multiplier);
}
