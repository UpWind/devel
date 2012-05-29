#include "xmlcontroller.h"
#include <QFile>

XMLController::XMLController(QObject *parent):
    FileController(parent), readFileDataWithName(0)
{
}

XMLController::~XMLController(){
   if(readFileDataWithName)
       delete readFileDataWithName;
}

int XMLController::save(const QString &filepath, bool ask){
    int i = 0;
    QString s;
    SavingType sav("", "");
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
    stream.writeStartElement("Simulation");
    stream.writeAttribute("date", getDate());

    if(toBeSaved.size() > 0) {
        sav = toBeSaved.at(0);
        s = sav.getLine().left(3);
        s = s.right(2);
        stream.writeAttribute("type", s);
    }else
        stream.writeAttribute("type", "II");

    stream.writeAttribute("swversion", "0.1");

    for(i = 0; i < toBeSaved.size(); ++i){
        SavingType savet = toBeSaved.at(i);
        stream.writeStartElement("entry");

        QString temp = savet.getLine().left(6);
        temp = temp.right(3);

        stream.writeAttribute("id", temp);

        stream.writeAttribute("value", savet.getLine());
        stream.writeAttribute("time", savet.getTime());
        if(!ask)
            stream.writeAttribute("name", savet.getName());
        stream.writeEndElement();

    }
    stream.writeEndElement();
    stream.writeEndDocument();

    file.flush();

    if(file.size() == 0){
        file.close();
        return -1;
    }

    file.close();

    return 0;
}

int XMLController::read(const QString &filepath) {
    filepathChangeble = false;
    QList<SavingType> readData;
    int basicms = 0;
    int previous = 0;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        filepathChangeble = true;
        return -1;
    }

    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        reader.readNext();
        if(!reader.isWhitespace()){
            QString line = reader.attributes().value("value").toString().trimmed();
            QString time = reader.attributes().value("time").toString().trimmed();

            if(line != ""){
                SavingType sav(line, time);
                readData << sav;
            }

        }
    }
    file.close();
    filepathChangeble = true;

    if(readData.size() == 0)
        return -1;

    SavingType basic = readData.at(0);
    previous = basic.getTime().toInt();

    cannotRun = false;
    int i;
    for(i=0; i<readData.size(); ++i) {

        if(cannotRun == true){
            break;
        }

        SavingType send = readData.at(i);
        int ms = send.getTime().toInt();
        QString line = send.getLine();

        basicms = ms - previous;
        previous = ms;

        msleep(basicms);
        emit giveLine(line);

    }

    readData.clear();
    cannotRun = false;

    return 0;

}

void XMLController::run(){
    read(filename);

}

QList<QPair<QString, int> >* XMLController::getFileData(const QString &filepath, bool ask){
    if(readFileData == 0){
        readFileData = new QList<QPair<QString, int> >;
    }else{
        readFileData->clear();
        //delete readFileData;
        //readFileData = 0;
    }

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return 0;
    }

    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        reader.readNext();
        if(!reader.isWhitespace()){
            QString line = reader.attributes().value("value").toString().trimmed();
            QString time = reader.attributes().value("time").toString().trimmed();

            if(line != ""){
                readFileData->append(qMakePair(line, time.toInt()));
                if(!ask){
                    QString name = reader.attributes().value("name").toString().trimmed();
                    SavingType type(line, time);
                    type.setName(name);
                    readFileDataWithName->append(type);
                }
            }

        }
    }
    file.close();

    return readFileData;
}

QList<SavingType>* XMLController::getFileDataWithName(const QString &filepath){
    if(!readFileDataWithName)
        readFileDataWithName = new QList<SavingType>;
    else{
        readFileDataWithName->clear();
    }

    getFileData(filepath, false);

    return readFileDataWithName;
}

int XMLController::saveWithName(const QString &filepath){
    return save(filepath, false);
}





