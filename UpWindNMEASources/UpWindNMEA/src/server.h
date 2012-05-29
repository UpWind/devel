#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Client;

/**
  * Server class implements the necessary methods for broadcasting NMEA strings.
  */
class Server : public QTcpServer
{
    Q_OBJECT

public:
    //list of connected clients
    QList<Client *> connections;
    /**
      * Constructor of the Server class.
      * @param parent QObject pointer to the parent (optional).
      */
    Server(QObject *parent = 0);
    /**
      * Virtual destructor of the Server class.
      */
    virtual ~Server();
    /**
      * Broadcasts the message to all clients.
      * @param msg Reference to the message.
      */
    void broadcastMessage(const QString &msg);
    /**
      * Sends an NMEA message to a client with a custom ID value.
      * @param id ID number of the message to be sent
      * @param msg Message to be sent
      * @param client Client who will receive the message
      */
    void sendMessage(const int id, const QString &msg, Client *client);
    /**
      * Sends an NMEA message to all clients with a custom ID value.
      * @param id ID number of the message to be sent
      * @param msg Message to be sent
      */
    void sendMessage(const int id, const QString &msg);
    /**
      * Helper method to sends a message to everyone but one client.
      * @param id ID number of the message to be sent
      * @param msg Message to be sent
      * @param client Client who will not receive the message
      */
    void broadcastMessageExclude(const int id, const QString &msg, Client *client);
    /**
      * Tries to read a string from the socket. Must be called multiple times if
      * connection can send multiple packets in one segment. Otherwise only
      * the first string is read.
      *
      * @return Returns proper string if read is successful.
      Returns an empty string if socket did not contain enough data.
      */
    QString tryReadString(int *id, Client *client);
    Client *getClient(const QString &name);

signals:
    /**
      * Emitted when a client connects to the server.
      * @param socket Pointer to the local socket.
      */
    void newConnection(QTcpSocket *socket);
    /**
      * Emitted when a client disconnects from the server.
      * @param socket Pointer to the local socket.
      */
    void closedConnection(QTcpSocket *socket);
    /**
      * Emitted when a new string has been properly received from a client.
      * @param msg Message that is received
      * @param id ID of the received message
      * @param socketDescriptor Unique descriptor value for the socket which was used.
      */
    void newMessage(const QString &msg, int id, Client *client);

protected:
    int nextClientID;
    /**
      * Overridden incomingConnection. Handles the initialization procedures of a connecting client.
      * @param socketDescriptor value describing the socket.
      */
    void incomingConnection(int socketDescriptor);

private slots:
    /**
      * Slot that will handle the finalization procedures of a disconnecting client.
      * Sender is the local client socket.
      */
    void clientDisconnected();
    /**
      * Slot that will handle reading of data and emit newMessage signals for all read data.
      */
    void messageArrived(const QString &msg, int id, Client *client);
};

#endif // SERVER_H
