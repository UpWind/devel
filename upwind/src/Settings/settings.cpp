#include "settings.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QMapIterator>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

Settings::Settings(){
    ownerName = "Default";
    fileName = "test.xml";
}

Settings::Settings(const QString &name){
    ownerName = name;
    fileName = name + ".xml";
}

QString Settings::getSetting(const QString &settingName){
    if(settings.contains(settingName))
        return settings.value(settingName);
    return 0;
}

void Settings::setSetting(const QString &settingName, const QString &settingValue){
    settings.insert(settingName, settingValue);
}

void Settings::displaySettings(){
    QString settingString = "";
    QMapIterator<QString, QString> i(settings);
    while(i.hasNext()){
        i.next();
        settingString += i.key() + " " + i.value() + "\n";
    }
    QMessageBox::about(0,"Settings", settingString);
}

QMap<QString, QString> Settings::getSettings(){
    return settings;
}

void Settings::setOwnerName(const QString &name){
    ownerName = name;
}

void Settings::setFileName(const QString &name){
    fileName = name;
}

void Settings::loadSettings(){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        exit(-1);

    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        reader.readNext();
        if(!reader.isWhitespace()){
            foreach(QXmlStreamAttribute attribute, reader.attributes()){
                QString name = attribute.name().toString();
                QString value = attribute.value().toString();
                this->setSetting(name, value);
            }
        }
    }
    file.close();
}

void Settings::saveSettings(){
    QDomDocument doc("Settings_for_" + ownerName);
    QDomElement root = doc.createElement("settings");
    doc.appendChild(root);

    QMapIterator<QString, QString> i(settings);
    while(i.hasNext()) {
        i.next();
        QDomElement settingNode = doc.createElement("setting");
        settingNode.setAttribute(i.key(), i.value());
        root.appendChild(settingNode);
    }
    QFile file(fileName);
    file.open( QIODevice::WriteOnly);
    QTextStream ts(&file);
    ts << doc.toString();
    file.close();
}
