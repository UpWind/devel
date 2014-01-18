#include "coreplugin.h"
#include <QStringList>
#include <QDateTime>
#include <QtWidgets>
#include "../../UpWindNMEA/src/settingsmanager.h"

CorePlugin::CorePlugin()
    : stopped(true), timerDelay(1000), timerMultiplier(1)
{
    pluginName = "CorePlugin";
    parseIDs = "---"; //use ZDA etc, the first one MUST be the plugin's own message type
    produceIDs = "---";
    version = "1.0.0";
    CreateGUI();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateNMEAString()));
}

CorePlugin::~CorePlugin(){
    delete timer;
    delete plugin_widget;
}

void CorePlugin::CreateGUI(){
    plugin_widget = new PluginQWidget(0, Qt::Tool);
    //plugin_widget->hide();
    //plugin_widget->show();
}

void CorePlugin::showPlugin(){
    plugin_widget->show();
}

void CorePlugin::hidePlugin(){
    plugin_widget->hide();
}

void CorePlugin::addPluginToLayout(QLayout *layout){
    layout->addWidget(plugin_widget);
}

void CorePlugin::removePluginFromLayout(QLayout *layout){
    layout->removeWidget(plugin_widget);
}

void CorePlugin::setTimerDelay(int tDelay){
    timerDelay = tDelay;
    if(!stopped)
        timer->start(timerDelay);
}

void CorePlugin::setTimerMultiplier(int multiplier){
    timerMultiplier = multiplier;
}


void CorePlugin::start(){
    stopped = false;
    timer->start(timerDelay);
}

void CorePlugin::stop(){
    stopped = true;
    timer->stop();
}

bool CorePlugin::isStopped(){
    return stopped;
}

QString CorePlugin::getName(){
    return pluginName;
}

QString CorePlugin::getProduceIDs(){
    return produceIDs;
}

QString CorePlugin::getParseIDs(){
    return parseIDs;
}

//checksum is calculated for the string between characters $ and *
//a checksum does not have to exist already in the nString
QString CorePlugin::generateChecksum(QString & nString){
    int i;
    quint8 xorRes = 0;
    int endI =nString.indexOf("*");
    if(endI<0)
        endI = nString.length();
    for(i = 1; i< endI; i++)
        xorRes ^= nString.at(i).toLatin1();

    QString resString;
    resString.setNum(xorRes, 16);
    if(resString.length()==1)
        resString = "0" + resString;
    return resString;
}

bool CorePlugin::supports(const QString &m_ID, bool excludeProduced){
    if(excludeProduced)
        if(produceIDs.indexOf(m_ID)>=0)
            return false;
        else
            return parseIDs.indexOf(m_ID)>=0;
    else
        return parseIDs.indexOf(m_ID)>=0;
}

PluginQWidget* CorePlugin::getWidget(){
    return plugin_widget;
}

QString CorePlugin::validateNMEA(const QString &text){
    QString msgID_;
    bool ok;

    if(text.indexOf("$") == 0){
        QStringList params = text.split(",");
        if(params.count()>1)
            if(((QString)params.at(0)).length()==6){
                msgID_ = ((QString)params.at(0)).right(3);

                //todo, if more optimizations are needed, return the parsed parameters as a list of
                //QVariant objects so parseNMEA does not need to parse them again

                if(msgID_ == "ZDA"){//Clock
                    if(params.count() == 7){
                        QString pDateTime = (QString)((QString)params.at(1)).split(".").at(0) +
                            params.at(2) + params.at(3) + params.at(4);
                        QDateTime dateTime = QDateTime::fromString(pDateTime, "hhmmssddMMyyyy");
                        if(dateTime.isValid()){
                            /*int hours = */((QString)params.at(5)).toInt(&ok);
                            if(ok)
                                /*int minutes = */((QString)params.at(6)).toInt(&ok);
                                if(ok)
                                    return msgID_; //valid
                        }
                    }
                }


                if(msgID_ == "MWV"){//Anemometer MWV (perhaps used at some point)
                    if(params.count() == 6){
                        double angle = ((QString)params.at(1)).toDouble();
                        if(angle <= 360 && angle >=0){
                            QString type = (QString)params.at(2);
                            if(type == "R" || type == "T"){
                                QString speed = (QString)params.at(4);
                                if(speed == "K" || speed == "M" || speed == "N"){
                                    QString valid = (QString)params.at(5);
                                    valid = valid.left(1);
                                    if(valid == "A"){
                                        return msgID_;
                                    }
                                }
                            }
                        }

                    }
                }

                if(msgID_ == "MWD"){//Anemometer MWD $IIMWD,348.49,T,348.49,M,00.00,N,00.00,M*44
                    if(params.count() == 9){
                        double angle = ((QString)params.at(1)).toDouble();
                        if(angle <= 360 && angle >=0){
                            QString type = (QString)params.at(2);
                            if(type == "T"){
                                angle = ((QString)params.at(3)).toDouble();
                                if(angle <= 360 && angle >=0){
                                    type = (QString)params.at(4);
                                    if(type == "M"){
                                        double speed = ((QString)params.at(5)).toDouble();
                                        if(speed >= 0){
                                            type = (QString)params.at(6);
                                            if(type == "N"){
                                                speed = ((QString)params.at(7)).toDouble();
                                                if(speed >= 0){
                                                    type = (QString)params.at(8);
                                                    type = type.left(1);
                                                    if(type == "M"){
                                                        return msgID_;
                                                    }

                                                }
                                            }
                                        }
                                    }
                                }

                            }
                        }

                    }
                }




                if(msgID_ == "RMC"){//GPS $IIRMC,,A,6503.300,N,02527.600,E,0,0.0,,*3f
                    if(params.count() == 11){
                        QString type = (QString)params.at(2);
                        if(type == "A"){
                            type = (QString)params.at(4);
                            if(type == "N" || type == "S"){
                                type = (QString)params.at(6);
                                if(type == "E" || type == "W"){
                                    return msgID_;
                                }
                            }
                        }
                    }
                }



                if(msgID_ == "HDG"){//Compass
                    if(params.count() == 6){
                        double angle = ((QString)params.at(1)).toDouble();
                        if(angle <= 360 && angle >=0){
                            QString direc = (QString)params.at(5);
                            direc = direc.left(1);
                            if(direc == "W" || direc == "E"){
                                return msgID_;
                            }
                        }
                    }
                }


        }
    }

    return ""; //invalid
}

void CorePlugin::saveTo(SettingsManager &sm){
    sm.saveHeader(pluginName, version);
    sm.saveProperty("x", QString::number(plugin_widget->x()));
    sm.saveProperty("y", QString::number(plugin_widget->y()));
    sm.saveProperty("width", QString::number(plugin_widget->width()));
    sm.saveProperty("height", QString::number(plugin_widget->height()));
}

QString CorePlugin::loadFrom(SettingsManager &sm){
    QString fileVer = sm.loadHeader(pluginName);
    plugin_widget->move( sm.loadProperty("x", "0").toInt(), sm.loadProperty("y", "0").toInt() );
    plugin_widget->resize( sm.loadProperty("width", "150").toInt(), sm.loadProperty("height", "150").toInt() );
    return fileVer;
}
