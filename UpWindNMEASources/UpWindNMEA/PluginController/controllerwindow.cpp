#include "controllerwindow.h"
#include "ui_controllerwindow.h"
#include "qeffects_p.h"
#include "upwindhighlighter.h"
#include "environment.h"
#include "instrumentpanel.h"
#include "plugindelegate.h"

#include <QInputDialog>
#include <QShortcut>
#include <QMessageBox>

const int OUTPUTDELAYTIME = 300;
const int OUTPUTMAXLINES = 100;

ControllerWindow::ControllerWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ControllerWindow),
        currentPlugin(0),
        xmlStatus(xmlDemoStopped),
        xmlRecording(false),
        panel(new InstrumentPanel(this)),
        minimizeIcon(QIcon(":/images/minimize.png")),
        maximizeIcon(QIcon(":/images/maximize.png")){

        ui->setupUi(this);

        controller.loadPlugins();

        this->setWindowIcon(QIcon(":/upwind/icons/upwindicon.ico"));
        ui->logProgressBar->hide();
        ui->tableView->setFocusPolicy(Qt::NoFocus);

        //Load program settings
        manager.load(Environment::getSettingsFilepath());
        manager.loadHeader("App");
        move(manager.loadProperty("x", "0").toInt(), manager.loadProperty("y", "0").toInt());
        resize(manager.loadProperty("width", "640").toInt(), manager.loadProperty("height", "480").toInt());
        ui->serialPortSpinBox->setValue(manager.loadProperty("serialport", "1").toInt());
        ui->serverPortSpinBox->setValue(manager.loadProperty("serverport", "9999").toInt());

        //Load and add all XML log files to a list widget
        controller.loadXmlLogFiles();
        foreach(QString logName, (*controller.getXmlLogFiles())) {
            ui->xmlListWidget->addItem(new QListWidgetItem((logName), ui->xmlListWidget));
        }

        //Add all plugins to a list widget
        foreach(QString pluginName, (*controller.getPluginNames())) {
            pluginTableModel.addString(pluginName);
            controller.getPlugin(pluginName)->loadFrom(manager);
        }

        ui->tableView->setModel(&pluginTableModel);
        ui->tableView->setItemDelegate(new PluginDelegate(controller));
        ui->pluginFrame->layout()->addWidget(panel);

        connect(&outputDelay, SIGNAL(timeout()), this, SLOT(flushOutput()));
        connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(showPlugin(QModelIndex)));

        connect(&controller, SIGNAL(newPluginString(QString)), this, SLOT(displayNMEAString(QString)));
        connect(&controller, SIGNAL(newXmlLogString(QString)), this, SLOT(displayNMEAString(QString)));
        connect(&controller, SIGNAL(newXmlLogFile()), this, SLOT(loadXmlLogFiles()));
        connect(&controller, SIGNAL(xmlLogEnd()), this, SLOT(stopXmlDemo()));

        connect(ui->serialCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleControllerSerial(bool)));
        connect(ui->serverCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleControllerServer(bool)));

        connect(ui->xmlStartPauseButton, SIGNAL(clicked()), this, SLOT(startXmlDemo()));
        connect(ui->xmlStopButton, SIGNAL(clicked()), this, SLOT(stopXmlDemo()));
        connect(ui->xmlRecordButton, SIGNAL(clicked()), this, SLOT(startRecording()));

        connect(ui->stopPluginsButton, SIGNAL(clicked()), this, SLOT(stopPlugins()));
        connect(ui->startPluginsButton, SIGNAL(clicked()), this, SLOT(startPlugins()));
        connect(ui->simulationSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(setSimulationSpeed(int)));
        connect(ui->clearButton, SIGNAL(clicked()), ui->textEdit, SLOT(clear()));

        connect(ui->serverMinMaxButton, SIGNAL(clicked()), this, SLOT(maximizeServer()));
        connect(ui->xmlMinMaxButton, SIGNAL(clicked()), this, SLOT(maximizeXml()));
        connect(ui->serialMinMaxButton, SIGNAL(clicked()), this, SLOT(maximizeSerial()));

        connect(&controller, SIGNAL(xmlLogProgress(int)), ui->logProgressBar, SLOT(setValue(int)));

        NMEAConsole = new QShortcut(this);
        NMEAConsole->setKey(QKeySequence(Qt::CTRL + Qt::Key_Return));
        connect(NMEAConsole, SIGNAL(activated()), this, SLOT(showNMEAConsole()));
        highlighter = new UpwindHighlighter(ui->textEdit->document());
        ui->textEdit->document()->setMaximumBlockCount(OUTPUTMAXLINES);
}

ControllerWindow::~ControllerWindow() {

    //Save plugin widget positions
    foreach(QString plugin, (*controller.getPluginNames())) {
        controller.getPlugin(plugin)->saveTo(manager);
    }
    //Save main window position and size
    manager.saveHeader("App", "1.0");
    manager.saveProperty("x", QString::number(this->x()));
    manager.saveProperty("y", QString::number(this->y()));
    manager.saveProperty("width", QString::number(this->width()));
    manager.saveProperty("height", QString::number(this->height()));

    //Save ports
    manager.saveProperty("serverport", QString::number(ui->serverPortSpinBox->value()));
    manager.saveProperty("serialport", QString::number(ui->serialPortSpinBox->value()));

    manager.save(Environment::getSettingsFilepath());
    delete ui;
    delete NMEAConsole;
    delete highlighter;
}

void ControllerWindow::showPlugin(QModelIndex pluginListItem) {

    if(pluginListItem.column() == 0){
        pluginTableModel.toggleCheck(pluginListItem);
        return;
    }


    currentPlugin = controller.getPlugin(pluginTableModel.getString(pluginListItem));
    if(currentPlugin) {
        if(!currentPlugin->getWidget()->isHidden()) {
            //currentPlugin->hidePlugin();
            fadePlugin(currentPlugin->getWidget(), true);
        }
        else {
            QPoint pos = currentPlugin->getWidget()->pos();
            currentPlugin->getWidget()->setParent(this, Qt::Tool); //To keep the plugin widgets always on top
            currentPlugin->getWidget()->move(pos);
            fadePlugin(currentPlugin->getWidget(), false);
        }
    }

}

void ControllerWindow::displayNMEAString(const QString &nmeaString) {

    //todo: filter based on plugins, check ID string
    QString pluginName;
    QList<QString> *names = controller.getPluginNames();
    bool filterAllow = true;
    QString messageID = CorePlugin::parseMessageID(nmeaString);
    for(int i=0; i< names->count(); i++) {
        pluginName = names->at(i);
        if(controller.getPlugin(pluginName)->getProduceIDs().indexOf(messageID) > -1){
            filterAllow = pluginTableModel.isChecked(i);
            break;
        }
    }
    if(filterAllow)
        appendOutput(nmeaString);

    if(ui->serverCheckBox->isChecked()) {
        controller.broadcastToServer(nmeaString);
    }
    if(ui->serialCheckBox->isChecked()) {
        controller.broadcastToSerial(nmeaString);
    }
    if(xmlRecording) {
        controller.broadcastToXml(nmeaString);
    }

    panel->parseNmeaString(nmeaString);

}

void ControllerWindow::toggleControllerServer(bool toggled) {
    if(toggled) {
        controller.startServer(ui->serverPortSpinBox->value());
    }
    else {
        controller.stopServer();
    }
}

void ControllerWindow::toggleControllerSerial(bool toggled) {
    if(toggled) {
        controller.startSerial(ui->serialPortSpinBox->value() - 1);
        if(!controller.isSerialOpen()) {
            ui->serialCheckBox->toggle();
            QMessageBox::warning(this, "Serial port information", "Failed to open serial port!");
        }
    }
    else {
        controller.stopSerial();
    }
}

void ControllerWindow::loadXmlLogFiles() {

    controller.loadXmlLogFiles();
    ui->xmlListWidget->clear();
    foreach(QString logName, (*controller.getXmlLogFiles())) {
        ui->xmlListWidget->addItem(new QListWidgetItem((logName), ui->xmlListWidget));
    }

}

void ControllerWindow::startXmlDemo() {

    if(ui->xmlListWidget->currentRow() != -1) {

        if(xmlStatus == xmlDemoStopped) {

            ui->logProgressBar->reset();
            ui->logProgressBar->show();

            xmlStatus = xmlDemoPlaying;
            appendOutput("XML file: " + ui->xmlListWidget->currentItem()->text());
            ui->xmlStartPauseButton->setText("Pause");

            //Disable plugin use when playing back logs
            ui->startPluginsButton->setEnabled(false);
            stopPlugins();
            flushOutput();
            outputDelay.start(OUTPUTDELAYTIME);
            controller.startXmlDemo(ui->xmlListWidget->currentItem()->text());
        }
        else if(xmlStatus == xmlDemoPaused) {
            xmlStatus = xmlDemoPlaying;
            controller.resumeXmlDemo();
            ui->xmlStartPauseButton->setText("Pause");
        }
        else if(xmlStatus == xmlDemoPlaying){
            xmlStatus = xmlDemoPaused;
            controller.pauseXmlDemo();
            ui->xmlStartPauseButton->setText("Resume");
        }
    }

}

void ControllerWindow::stopXmlDemo() {

    if(xmlStatus == xmlDemoPlaying || xmlStatus == xmlDemoPaused) {

        ui->logProgressBar->hide();

        xmlStatus = xmlDemoStopped;
        ui->xmlStartPauseButton->setText("Start");
        controller.stopXmlDemo();

        //Enable plugin use
        ui->startPluginsButton->setEnabled(true);
        appendOutput("NMEA log playback end.");
        outputDelay.stop();
        flushOutput();
    }

}

void ControllerWindow::showNMEAConsole() {
    bool ok;
    QString text = QInputDialog::getText(this, "Sending custom NMEA sentence",
        "NMEA String", QLineEdit::Normal, QString(), &ok);
        if (ok && !text.isEmpty()){
            if(text.indexOf("*")>=0 &&
                text.indexOf("*")+3 == text.length()){

                text = text.left(text.length()-3);
                text += "*" + CorePlugin::generateChecksum(text);
            }else{
                text += "*" + CorePlugin::generateChecksum(text);
            }
        QString textId = CorePlugin::validateNMEA(text);
        if(textId.length()>0){

            displayNMEAString(text);
            foreach(QString pluginName, (*controller.getPluginNames())) {

                CorePlugin *plugin = controller.getPlugin(pluginName);
                if(plugin) {
                    if(plugin->supports(textId, false)) {
                        plugin->parseNMEAString(text);
                    }
                }
            }
        }else
            qDebug() << "Invalid NMEA String entered: " << text;
    }
}

void ControllerWindow::startRecording() {

    if(!xmlRecording) {
        xmlRecording = true;
        ui->xmlRecordButton->setText("Stop recording");
    }
    else {
        ui->xmlRecordButton->setText("Record");
        xmlRecording = false;
        stopPlugins();
        controller.saveXml();
        ui->xmlRecordButton->setEnabled(true);
    }

}

void ControllerWindow::stopPlugins() {
    foreach(QString pluginName, (*controller.getPluginNames())) {
        controller.getPlugin(pluginName)->stop();
    }
    outputDelay.stop();
    flushOutput();
}

void ControllerWindow::startPlugins() {
    foreach(QString pluginName, (*controller.getPluginNames())) {
        controller.getPlugin(pluginName)->start();
    }
    outputDelay.start(OUTPUTDELAYTIME);
}

void ControllerWindow::setSimulationSpeed(int speed) {

    ui->simulationSpeedValueLabel->setText(QString::number(speed) + "x");

    foreach(QString pluginName, (*controller.getPluginNames())) {
        CorePlugin *plugin = controller.getPlugin(pluginName);
        if(plugin) {
            plugin->setTimerDelay(1000 / speed);
            plugin->setTimerMultiplier(speed);
        }
    }
}

void ControllerWindow::flushOutput(){
    ui->textEdit->setUpdatesEnabled(false);
    foreach(QString line, waitingOutput)
        ui->textEdit->append(line);
    ui->textEdit->setUpdatesEnabled(true);
    ui->textEdit->update();
    waitingOutput.clear();
}

void ControllerWindow::appendOutput(const QString &line){
    waitingOutput.append(line);
}

void ControllerWindow::maximizeSerial() {

    if(ui->serialContainer->isHidden()) {
        ui->serialMinMaxButton->setIcon(minimizeIcon);
        ui->serialContainer->show();
    }
    else {
        ui->serialMinMaxButton->setIcon(maximizeIcon);
        ui->serialContainer->hide();
    }
}

void ControllerWindow::maximizeServer() {

    if(ui->serverContainer->isHidden()) {
        ui->serverMinMaxButton->setIcon(minimizeIcon);
        ui->serverContainer->show();
    }
    else {
        ui->serverMinMaxButton->setIcon(maximizeIcon);
        ui->serverContainer->hide();
    }
}

void ControllerWindow::maximizeXml() {

    if(ui->xmlContainer->isHidden()) {
        ui->xmlMinMaxButton->setIcon(minimizeIcon);
        ui->xmlContainer->show();
    }
    else {
        ui->xmlMinMaxButton->setIcon(maximizeIcon);
        ui->xmlContainer->hide();
    }

}
