#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include "gamelogic.h"
#include "game_window.h"
class QLabel;
class QTcpServer;
class QNetworkSession;

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr, game_window* game_win = nullptr);

private slots:
    void sessionOpened();
    void sendGameInfo();
    GameLogic* game;
    game_window* game_win;

private:
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;
};

//class Server : public QDialog
//{
//    Q_OBJECT

//public:
//    explicit Server(QWidget *parent = nullptr);

//private slots:
//    void sessionOpened();
//    void sendFortune();

//private:
//    QLabel *statusLabel = nullptr;
//    QTcpServer *tcpServer = nullptr;
//    QVector<QString> fortunes;
//    QNetworkSession *networkSession = nullptr;
//};

#endif
