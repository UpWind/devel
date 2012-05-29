#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

/**
  * General packet ids used to identify packets. Use id CUSTOM for your own packets.
  */
namespace PacketID {
    const int NMEA_STRING = 1;
    const int CLIENT_NAME = 2;
    const int CLIENTJOINED = 3;
    const int CLIENTLEFT = 4;
    const int SIMULATION_ENDED = 5;
    const int CLIENTLIST = 6;
    const int SIMULATION_STARTED = 7;
    const int NAME_IN_USE = 8;
    const int CUSTOM = 1000;
}

/**
  * Client class contains the methods necessary to read information from the server.
  */
class Client : public QTcpSocket
{
    Q_OBJECT
protected:
    /**
      * Tries to read a string from the socket. Must be called multiple times if
      * connection can send multiple packets in one segment. Otherwise only
      * the first string is read.
      *
      * @return Returns proper string if read is successful. Returns an empty string if socket did not contain enough data.
      */
    QString tryReadString(int *id = 0);
public:
    virtual ~Client();
    QObject *extraData;
    qint32 stringSize;
    //type of the incoming packet, 0 means no type (invalid type)
    qint32 typeID;
    QString clientName;
    int clientID;
    /**
      * Constructor of the Client class.
      * @param parent QObject pointer to the parent (optional).
      */
    Client(QObject *parent = 0);
    /**
      * Sends an NMEA message to server with a custom ID value.
      * @param id ID number of the message to be sent
      * @param msg Message to be sent
      */
    void sendMessage(const int id, const QString &msg);
signals:
    /**
      * Emitted when a new string has been properly received from a client.
      * @param msg Message that is received
      * @param id ID of the received message
      * @param socketDescriptor Unique descriptor value for the socket which was used.
      */
    void newMessage(const QString &msg, const int id, Client *client);
private slots:
    /**
      * Slot that will handle reading of data and emit newMessage signals for all read data.
      */
    void dataAvailable();
};

#endif // CLIENT_H
