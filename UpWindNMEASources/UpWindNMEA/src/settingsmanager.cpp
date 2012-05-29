#include "settingsmanager.h"

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent), loadDoc(0), next(0)
{
    this->setDomBasic();
}

SettingsManager::~SettingsManager(){
    delete doc;
    if(loadDoc)
        delete loadDoc;
}

bool SettingsManager::save(const QString &filename){
    QFile file(filename);
    if(!file.open( QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream ts(&file);
    ts << doc->toString();

    file.close();

    return true;
}

void SettingsManager::saveHeader(const QString &name, const QString &version){
    cn = doc->createElement("Object");
    cn.setAttribute("Name", name);
    cn.setAttribute("Version", version);
    root.appendChild(cn);
}

void SettingsManager::saveProperty(const QString &name, const QString &value){
    QDomElement setting = doc->createElement(name);
    setting.setAttribute("Value", value);
    cn.appendChild(setting);
}

bool SettingsManager::load(const QString &filename){
    loadDoc = new QDomDocument("SettingsML");
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    if(!loadDoc->setContent(&file)){
        file.close();
        return false;
    }

    file.close();
    return true;
}

QString SettingsManager::loadHeader(const QString &name){
    if(!loadDoc)
        return 0;
    if(entrySettings.size() > 0)
        entrySettings.clear();

    QDomElement first = loadDoc->documentElement();
    if(first.tagName() != "Settings")
        return 0;

    QDomNode n = first.firstChild();
    while(!n.isNull()){
        QDomElement e = n.toElement();
        if(!e.isNull()){
            if(e.attribute("Name", "") == name){
                QDomNode node = e.firstChild();
                while(!node.isNull()){
                    QDomElement elem = node.toElement();
                    if(!elem.isNull()){
                        entrySettings.append(qMakePair(elem.tagName(), elem.attribute("Value", "")));
                    }
                    node = node.nextSibling();
                }
                return e.attribute("Version", "");
            }
        }
        n = n.nextSibling();
    }

    return 0;
}

QString SettingsManager::loadProperty(const QString &name, const QString &defValue){
    int i;
    for(i=0; i<entrySettings.size(); ++i){
        if(entrySettings.at(i).first == name){
            return entrySettings.at(i).second;
        }

    }
    return defValue;
}

void SettingsManager::setDomBasic(){
    doc = new QDomDocument("SettingsML");
    root = doc->createElement("Settings");
    doc->appendChild(root);
}
