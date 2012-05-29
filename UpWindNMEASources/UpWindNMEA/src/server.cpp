#include <QtNetwork>
#include <QTcpSocket>
#include "server.h"
#include "client.h"

Server::Server(QObject *parent)
                :QTcpServer(parent), nextClientID(0)
{

}

Server::~Server()
{
    foreach(QTcpSocket *clientSock, connections){
        clientSock->flush();
        clientSock->deleteLater();
    }
}

void Server::broadcastMessage(const QString &msg)
{
    //if(msg.length() == 0)
    //    return;
    QByteArray stringData;
    QDataStream stringDataStream(&stringData, QIODevice::ReadWrite);
    QByteArray output;
    QDataStream outputStream(&output, QIODevice::WriteOnly);

    stringDataStream << msg;
    outputStream << ((qint32)(PacketID::NMEA_STRING));
    outputStream << (qint32)(stringData.size());
    output.append(stringData);

    foreach(QTcpSocket *clientSock, connections)
        clientSock->write(output);
}

Client *Server::getClient(const QString &name){
    foreach(Client *client, connections)
        if(client->clientName == name)
            return client;
    return 0;
}

void Server::clientDisconnected()
{
    Client *client = (Client *)(sender());
    disconnect(client, SIGNAL(newMessage(QString,int,Client*)), this, SIGNAL(newMessage(QString,int,Client*)));
    connections.removeAt(connections.indexOf(client));
    emit closedConnection(client);

    client->deleteLater();
}

void Server::incomingConnection(int socketDescriptor)
{
    Client *client = new Client(this);
    client->setSocketDescriptor(socketDescriptor);
    connections.append(client);
    connect(client, SIGNAL(newMessage(QString,int,Client*)), this, SIGNAL(newMessage(QString,int,Client*)));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    client->clientID = nextClientID;
    nextClientID++;
    emit newConnection(client);
}

void Server::sendMessage(const int id, const QString &msg, Client *client){
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

    foreach(Client *clientSock, connections)
        if(clientSock->socketDescriptor() == client->socketDescriptor())
            clientSock->write(output);
}

void Server::sendMessage(const int id, const QString &msg){
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

    foreach(Client *clientSock, connections)
        clientSock->write(output);
}

void Server::broadcastMessageExclude(const int id, const QString &msg, Client *client){
    foreach(Client *clientSock, connections)
        if(clientSock->socketDescriptor() != client->socketDescriptor())
            sendMessage(id, msg, clientSock);
}

void Server::messageArrived(const QString &msg, int id, Client *client){
    emit newMessage(msg, id, client);
    qDebug() << "This is never called";
}
