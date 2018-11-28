#ifndef SERVER_H
#define SERVER_H


#include <QTcpServer>
#include <QVector>

class ServerWorker;
class Server : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
public:
    explicit Server(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
signals:
//    void logMessage(const QString &msg);
public slots:
//    void stopServer();
private slots:
//    void broadcast(const QJsonObject &message, ServerWorker *exclude);
//    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
//    void userDisconnected(ServerWorker *sender);
//    void userError(ServerWorker *sender);
private:
//    void jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &doc);
//    void jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &doc);
//    void sendJson(ServerWorker *destination, const QJsonObject &message);
    QVector<ServerWorker *> clients;
};

#endif // SERVER_H
