#include "client.h"

Client::Client(QObject *parent)
                :QTcpSocket(parent), extraData(0), stringSize(-1), typeID(0), clientID(-1)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
}

Client::~Client(){
    if(extraData)
        delete extraData;
}

void Client::sendMessage(const int id, const QString &msg){
    //if(msg.length() == 0)
    //    return;
    QByteArray stringData;
    QDataStream stringDataStream(&stringData, QIODevice::ReadWrite);
    QByteArray output;
    QDataStream outputStream(&output, QIODevice::WriteOnly);

    stringDataStream << msg;
    outputStream << ((qint32)(id));
    outputStream << (qint32)(stringData.size());
    output.append(stringData);

    this->write(output);
}

QString Client::tryReadString(int *id)
{
    if(typeID==0){
        if(bytesAvailable() < sizeof(typeID))
        {
            return "";
        }else{
            QByteArray data = read(sizeof(typeID));
            QDataStream dStream(data);
            dStream >> typeID;
        }
    }
    if(stringSize<0){
        if(bytesAvailable() < sizeof(stringSize))
        {
            return "";
        }else{
            QByteArray data = read(sizeof(stringSize));
            QDataStream dStream(data);
            dStream >> stringSize;
        }
    }
    if(bytesAvailable() < stringSize)
        return "";

    QByteArray data = read(stringSize);
    QDataStream dStream(data);
    QString result;
    dStream >> result;

    *id = typeID;

    stringSize = -1;
    typeID = 0;

    return result;
}

void Client::dataAvailable(){
    QString result;
    int id;
    do{
        id = -1;
        result = tryReadString(&id);
        if(id > -1)
            emit newMessage(result, id, this);
    }while(id > -1);
}
