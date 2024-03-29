#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
class QHostAddress;
class QJsonDocument;
class Client : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
public:
    explicit Client(QObject *parent = nullptr);
    void sendJson(const QJsonObject &json);
public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void connected();
    void disconnected();
    void receiveJson(const QJsonObject &json);
private:
    QTcpSocket *clientSocket;
    void jsonReceived(const QJsonObject &doc);
};

#endif // CLIENT_H
