#include "plugincontroller.h"

#include <QtCore>
#include <QFileDialog>

#include "coreinterface.h"
#include "coreplugin.h"
#include "xmlcontroller.h"
#include "serialsrc/serialport.h"
#include "qeffects_p.h"

#include "environment.h"

#define QT_NO_EFFECTS

int PluginController::logIndex = 0;

PluginController::PluginController():
    server(0), serialPort(0), xmlController(new XMLController), xmlLog(0) {

    connect(&delayTimer, SIGNAL(timeout()), this, SLOT(getXmlLogString()));

    time.start();
}

PluginController::~PluginController() {

    if(server) {
        delete server;
    }
    if(serialPort) {
        delete serialPort;
    }

    delete xmlController;

}

void PluginController::loadPlugins(const QString &plugDir, const QStringList &filter) {

    QDir pluginsDir(plugDir);

    qDebug() << Q_FUNC_INFO << qApp->applicationDirPath();
    qDebug() << "plugin dirrr" << plugDir;

    qDebug()<< "plug filter: " << pluginsDir.entryList();


    //Load all plugins found in directory
    foreach(QString fileName, pluginsDir.entryList()) {           //     foreach(QString fileName, pluginsDir.entryList(filter)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *loadedPlugin = loader.instance();

        qDebug()<< "plug name: " << fileName;

        if(loadedPlugin) {
            CoreInterface *interface = qobject_cast<CoreInterface *>(loadedPlugin);
            CorePlugin *plugin = dynamic_cast<CorePlugin *>(interface);

            if(plugin) {

                connect(plugin->getWidget(), SIGNAL(onClose()), this, SLOT(pluginHackFade()), Qt::DirectConnection);
                pluginNames.append(plugin->getName());
                plugins.insert(plugin->getName(), plugin);

            }
        }
    }

    foreach(CorePlugin *plugin, plugins) {
        connect(plugin, SIGNAL(newNMEAString(QString)), this, SLOT(getPluginString(QString)));
    }
}

QList<QString> *PluginController::getPluginNames() {

    return &pluginNames;

}

CorePlugin *PluginController::getPlugin(const QString &pluginName) {

    if(plugins.contains(pluginName)) {

        return plugins.value(pluginName);

    }
    else {
        return 0;
    }
}

void PluginController::startServer(unsigned int portNumber) {
    if(!server) {
        server = new Server;
        server->listen(QHostAddress::Any, portNumber);
    }
}

void PluginController::stopServer() {
    if(server) {
        server->close();
        delete server;
        server = 0;
    }
}

void PluginController::startSerial(unsigned int portNumber, unsigned int baudRate) {
    if(!serialPort) {
        serialPort = new SerialPort(portNumber, baudRate);

        if(!serialPort->isOpen()) {
            delete serialPort;
            serialPort = 0;
        }
    }
}

void PluginController::stopSerial() {
    if(serialPort) {
        serialPort->close();
        delete serialPort;
        serialPort = 0;
    }
}

bool PluginController::isSerialOpen() {
    if(serialPort) {
        return serialPort->isOpen();
    }
    else {
        return false;
    }
}

void PluginController::saveXml() {

    //Save generated XML log to a file
    QDir logsDir(qApp->applicationDirPath());
    logsDir.cd("logs");
    QString saveFileName = QFileDialog::getSaveFileName(0, "Save log file", Environment::getLogDirectory() + "log" +
                                                        QDateTime::currentDateTime().toString("dd.MM.yy.hh.mm") +
                                                        ".xml", "XML files (*.xml)");

    //A valid save was made, emit signal to inform the view of a new log
    if(saveFileName.size() != 0) {
        xmlController->save(saveFileName);
        xmlController->emptyToBeSaved();
        emit newXmlLogFile();
    }

}

void PluginController::broadcastToServer(const QString &nmeaString) {
    if(server) {
        server->broadcastMessage(nmeaString);
    }
}

void PluginController::broadcastToSerial(const QString &nmeaString) {
    if(serialPort) {

        serialPort->writeString(nmeaString.toStdString());
    }
}

void PluginController::broadcastToXml(const QString &nmeaString) {

    xmlController->addToBeSaved(nmeaString, QString::number(time.elapsed()));

}

void PluginController::loadXmlLogFiles() {

    xmlLogs.clear();

    QDir xmlDir(qApp->applicationDirPath());
    xmlDir.setNameFilters(QStringList("*.xml"));
    bool folderExists = xmlDir.cd("logs");

    if(folderExists) {
        QStringList list = xmlDir.entryList(QDir::Files);
        foreach(QString xmlFile, list) {
            xmlLogs.append(xmlFile);
        }
    }
    else {
        xmlDir.mkdir("logs");
    }

}

QList<QString> *PluginController::getXmlLogFiles() {

    return &xmlLogs;

}

void PluginController::startXmlDemo(const QString &xmlFileName) {

    logIndex = 0;

    xmlSimulationStartTime.start();
    xmlLog = xmlController->getFileData(Environment::getLogDirectory() + xmlFileName);

    //XML log exists, start timer and set initial delay time
    if(xmlLog && xmlLog->size() > 0) {
        offsetAmount = -xmlLog->front().second;
        delayTimer.start(0);
    }
    else if(xmlLog->size() == 0) {
        emit xmlLogEnd();
    }

}

void PluginController::stopXmlDemo() {

    delayTimer.stop();

}

void PluginController::pauseXmlDemo() {

    delayTimer.pause();

}

void PluginController::resumeXmlDemo() {

    if(logIndex < xmlLog->size()) {

        if(logIndex == 0) {
            offsetAmount = -xmlLog->front().second;
        }
        else {
            offsetAmount = -xmlLog->at(logIndex - 1).second;
        }

        int resumeTime = xmlLog->at(logIndex).second + offsetAmount;

        xmlSimulationStartTime.start();
        delayTimer.resume(resumeTime);

    }

}

void PluginController::getPluginString(const QString &nmeaString) {

    emit newPluginString(nmeaString);

    //Send NMEA string to all plugins that need it
    foreach(CorePlugin *plugin, plugins) {
        if(sender() != plugin &&  plugin->supports(CorePlugin::parseMessageID(nmeaString), true)) {
            plugin->parseNMEAString(nmeaString);
        }
    }
}

void PluginController::getXmlLogString() {

    if(xmlLog && xmlLog->size() == 0) {
        emit xmlLogEnd();
        delayTimer.stop();
        return;
    }

    if(xmlLog && xmlLog->size() > 0) {

        //Get and emit current NMEA string from XML log
        QString nmeaString = xmlLog->at(logIndex).first;
        emit newXmlLogString(nmeaString);

        //Calculate log playback percentage
        double temp = static_cast<double>(logIndex + 1) / static_cast<double>(xmlLog->size());
        emit xmlLogProgress(static_cast<int>(temp * 100));

        logIndex++;

        if(logIndex < xmlLog->size()) {

            //Set new delay time
            int tempInterval = xmlLog->at(logIndex).second - xmlSimulationStartTime.elapsed() + offsetAmount;
            if(tempInterval > 0) {
                delayTimer.setInterval(tempInterval);
            }
            else {
                delayTimer.setInterval(0);
            }

            //Send NMEA string to all plugins that need it
            foreach(CorePlugin *plugin, plugins) {
                if(plugin->supports(CorePlugin::parseMessageID(nmeaString), false)) {
                    plugin->parseNMEAString(nmeaString);
                }
            }

        }
        else {
            emit xmlLogEnd();
            delayTimer.stop();
        }

    }

}

Server* PluginController::getServer(){
    return server;
}

void PluginController::pluginHackFade() {

    QWidget *widget = qobject_cast<QWidget *>(sender());
    fadePlugin(widget, true);

}
