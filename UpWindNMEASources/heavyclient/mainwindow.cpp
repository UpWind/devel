#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringList>
#include "upwindhighlighter.h"
#include "serialport.h"
#include <QStringList>
#include <QWebFrame>

const int OUTPUTDELAYTIME = 300;
const int OUTPUTMAXLINES = 100;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serialport(0), nextID(0), windDirection(0)
{
    ui->setupUi(this);
    connect(&client, SIGNAL(newMessage(QString,int,Client*)), this, SLOT(messageArrived(QString,int,Client*)));
    connect(&client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(ui->serialCheckBox, SIGNAL(toggled(bool)), this, SLOT(openSerial(bool)));
    connect(ui->clearButton, SIGNAL(clicked()), ui->textOutput, SLOT(clear()));
    connect(&outputDelay, SIGNAL(timeout()), this, SLOT(flushOutput()));
    outputDelay.start(OUTPUTDELAYTIME);
    loadSettings(XML_FILE);
    this->setWindowIcon(QIcon(":/upwind/icons/upwindicon.ico"));

    highlighter = new UpwindHighlighter(ui->textOutput->document());
    ui->textOutput->document()->setMaximumBlockCount(OUTPUTMAXLINES);
    ui->pluginFrame->hide();
    ui->pluginFrame->layout()->addWidget(compassInstrument.getWidget());
    compassInstrument.showPlugin();
    connect(&compassInstrument, SIGNAL(newNMEAString(QString)), this, SLOT(compassInfo(QString)));

    QString filePath(QApplication::applicationDirPath() +
                   "/plugins/index.html");
    if(QFile::exists(filePath)){
        ui->gemap->setUrl(QUrl("file:///" + filePath));
        appendOutput("Loaded index.html successfully");
    }else
        appendOutput("Failed to load \"" + filePath + "\"");
}

void MainWindow::attachGPSPluginToPage()
{
    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("gps", this);
    ui->gemap->page()->mainFrame()->addToJavaScriptWindowObject("q", &q);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("setHeavyClientMode();");
}

MainWindow::~MainWindow()
{
    if(client.state() != QAbstractSocket::UnconnectedState){
        breakConnection();
    }
    if(serialport) {
        serialport->close();
        delete serialport;
    }
    delete ui;
    delete highlighter;
}

void MainWindow::messageArrived(const QString &msg, int id, Client *client){

    appendOutput(QString::number(id) + ":" + msg);

    QStringList pList;
    QString angle;

    QStringList boats;    
    QStringList data;//boatName, latitude, longitude;
    switch(id){
    case PacketID::CLIENTJOINED:
        ui->pluginFrame->show();
        addBoat(msg);
        break;
    case PacketID::CLIENTLEFT:
        break;
    case PacketID::NAME_IN_USE:
        client->disconnectFromHost();
        QMessageBox::warning(this, "Unique name required", "The name you selected is already in use. Please select a new name.");
        if(client->state() != QAbstractSocket::UnconnectedState)
            client->waitForDisconnected();
        break;
    case PacketID::CLIENTLIST:
         boats = msg.split(";");
        foreach(QString boatName, boats)
            addBoat(boatName);
        break;
    case PacketID::SIMULATION_STARTED:
        appendOutput("Simulation started");
        compassInstrument.start();
        break;
    case PacketID::SIMULATION_ENDED:
        if(this->client.clientName == msg)
            appendOutput("Simulation ended. You were the winner.");
        else
            appendOutput("Simulation ended. Winner was " + msg);
        compassInstrument.stop();
        break;
    case PacketID::NMEA_STRING:
        pList = msg.split(",");
        angle = pList.at(3).split(".").at(0);
        windDirection = angle.toInt();
        q.setProperty("windDirection", windDirection);
        ui->gemap->page()->mainFrame()->evaluateJavaScript("setWindAngle();");

        if(serialport) {
            serialport->writeString(msg);
        }
        break;
    case MultiSimuID::GPSLIST:
        boats = msg.split("*");
        foreach(QString boat, boats){
            data = boat.split(",");
            setBoatMapLocation(data.at(0), data.at(1), data.at(2));
        }
    break;
    }
}

void MainWindow::setBoatMapLocation(const QString &boatName, const QString &latitude, const QString &longitude){

    MapObject* object = 0;

    for(int i=0; i<boatList.count(); i++){
        if( boatList.at(i)->name == boatName){
            object = boatList.at(i);
            object->latitude = latitude.toDouble();
            object->longnitude = longitude.toDouble();
            break;
        }
    }
    if(!object){
        qDebug() << "No object named " + boatName;
        return;
    }
    q.setProperty("x", object->latitude);
    q.setProperty("y", object->longnitude);
    q.setProperty("index", object->ID);
    q.setProperty("boatDirection", compassInstrument.getHeading());
    q.setProperty("boatName", boatName);

    if(client.clientName == boatName) {
        ui->gemap->page()->mainFrame()->evaluateJavaScript("setPos();");
        ui->gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");
    }  else
        ui->gemap->page()->mainFrame()->evaluateJavaScript("moveBoat();");
    //logMessage("Boat position set for " + client->clientName);
}


void MainWindow::setBoatDirection(int bearing){

    compassInstrument.setHeading(bearing);
    q.setProperty("boatDirection", bearing);
    ui->gemap->page()->mainFrame()->evaluateJavaScript("setBoatAngle();");

}

void MainWindow::addBoat(const QString &boatName){
    boatList.append(new MapObject(boatName, nextID));
    nextID++;
}

void MainWindow::disconnected()
{
    ui->connectBtn->setText("Connect");
    disconnect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(breakConnection()));
    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(makeConnection()));

    ui->pluginFrame->hide();
    ui->serialBox->show();
    ui->connectionBox->show();

    ui->gemap->page()->mainFrame()->evaluateJavaScript("refresh();");
    //delete serialport;
}

void MainWindow::breakConnection()
{
    client.disconnectFromHost();
    if(client.state() != QAbstractSocket::UnconnectedState)
        client.waitForDisconnected();
}
void MainWindow::openSerial(bool toggled) {
    if(toggled) {
        serialport = new SerialPort(ui->serialSpinBox->value() - 1);
        if(!serialport->isOpen()) {
            ui->serialCheckBox->toggle();
            QMessageBox::warning(this, "Serial port information", "Failed to open serial port!");
        }
    }
    else {
        serialport->close();
        delete serialport;
        serialport = 0;
    }
}

void MainWindow::makeConnection()
{
    saveSettings(XML_FILE);
    client.connectToHost(ui->ipEdit->text(),ui->portEdit->text().toUShort());

    if(!client.waitForConnected(5000)){
        QMessageBox::warning(this, tr("Connection error"),
             tr("Connection could not be established."));
        return;
    }

    ui->connectBtn->setText("Disconnect");
    disconnect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(makeConnection()));
    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(breakConnection()));
    //serialport = new SerialPort();
    client.clientName = ui->nameEdit->text();
    client.sendMessage(PacketID::CLIENT_NAME, client.clientName);
    ui->connectionBox->hide();
    ui->serialBox->hide();

    attachGPSPluginToPage();

}

void MainWindow::loadSettings(const QString& fileName){
    QFile::QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QXmlStreamReader reader(&file);

    reader.readNext();
    reader.readNext();
    if(!reader.isWhitespace()){
        ui->ipEdit->setText(reader.attributes().value("IP").toString().trimmed());
        ui->portEdit->setText(reader.attributes().value("Port").toString().trimmed());
        ui->nameEdit->setText(reader.attributes().value("Name").toString().trimmed());
    }
    file.close();
}

void MainWindow::saveSettings(const QString& fileName){
    QFile::QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
    stream.writeStartElement("Settings");
    stream.writeAttribute("IP", ui->ipEdit->text());
    stream.writeAttribute("Port", ui->portEdit->text());
    stream.writeAttribute("Name", ui->nameEdit->text());
    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
}

void MainWindow::flushOutput(){
    ui->textOutput->setUpdatesEnabled(false);
    foreach(QString line, waitingOutput)
        ui->textOutput->append(line);
    ui->textOutput->setUpdatesEnabled(true);
    ui->textOutput->update();
    waitingOutput.clear();
}

void MainWindow::appendOutput(const QString &line){
    waitingOutput.append(line);
}

void MainWindow::compassInfo(const QString &msg){
    client.sendMessage(PacketID::NMEA_STRING, msg);
}
