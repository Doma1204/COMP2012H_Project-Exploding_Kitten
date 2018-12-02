#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamelogic.h"
#include "server.h"
#include "client.h"

#include "gamelogic.h"
#include "gamewindow.h"

#include <QListWidget>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GameWindow* gameWindow;
private:
    // UI
    enum Window {NoWindow = 0, RequestRoom, CreateRoom, JoinRoom, Room};
    Ui::MainWindow *ui;

    GameLogic* game;
    Server* server;
    Client* client;

    QString playerName;
    QString ip;
    quint16 port;

    bool isHost;
    bool isConnect;

    // Window Setting
    void setRequestRoomWindow();
    void deleteRequestRoomWindow();
    void setCreateRoomWindow();
    void deleteCreateRoomWindow();
    void setJoinRoomWindow();
    void deleteJoinRoomWindow();
    void setRoomWindow();
    void deleteRoomWindow();

    void destroyRoom();
    void joinRoom();
    void leaveRoom();
    void forceLeaveRoom();
    void addPlayer(const QString &playerName);
    void removePlayer(const QString &playerName);

    //Start Game
    void startGame();

private slots:
    void create_room_handler();
    void join_room_handler();

    void sendPlayerName();
    void clientJsonReceived(const QJsonObject &json);
};

#endif // MAINWINDOW_H
