#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>

#include "upwindhighlighter.h"
#include "serialport.h"

const int OUTPUTDELAYTIME = 300;
const int OUTPUTMAXLINES = 100;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serialport(0)
{
    ui->setupUi(this);
    connect(&client, SIGNAL(newMessage(QString,int,Client*)), this, SLOT(messageArrived(QString,int,Client*)));
    connect(&client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(ui->serialCheckBox, SIGNAL(toggled(bool)), this, SLOT(openSerial(bool)));
    connect(ui->clearButton, SIGNAL(clicked()), ui->textOutput, SLOT(clear()));
    connect(&outputDelay, SIGNAL(timeout()), this, SLOT(flushOutput()));
    loadSettings(XML_FILE);
    this->setWindowIcon(QIcon(":/upwind/icons/upwindicon.ico"));

    highlighter = new UpwindHighlighter(ui->textOutput->document());
    ui->textOutput->document()->setMaximumBlockCount(OUTPUTMAXLINES);
}

MainWindow::~MainWindow()
{
    if(serialport) {
        serialport->close();
        delete serialport;
    }
    delete ui;
    delete highlighter;
}

void MainWindow::messageArrived(const QString &msg, int id, Client *client){
    appendOutput(msg);

    if(serialport) {
        serialport->writeString(msg);
    }
}

void MainWindow::disconnected()
{
    ui->connectBtn->setText("Connect");
    disconnect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(breakConnection()));
    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(makeConnection()));

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
    outputDelay.start(OUTPUTDELAYTIME);

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
