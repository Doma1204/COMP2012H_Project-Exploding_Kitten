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
//    void login(const QString &userName);
//    void sendMessage(const QString &text);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void connected();
//    void loggedIn();
//    void loginError(const QString &reason);
    void disconnected();
    void receiveJson(const QJsonObject &json);
//    void messageReceived(const QString &sender, const QString &text);
//    void error(QAbstractSocket::SocketError socketError);
//    void userJoined(const QString &username);
//    void userLeft(const QString &username);
private:
    QTcpSocket *clientSocket;
    void jsonReceived(const QJsonObject &doc);
};

#endif // CLIENT_H
