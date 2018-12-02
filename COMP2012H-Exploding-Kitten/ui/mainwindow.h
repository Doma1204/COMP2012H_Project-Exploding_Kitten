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
    GameWindow* gameWindow; //gameWindow will be the main window for the game, will be created after the game has been started
private:
    // UI
    Ui::MainWindow *ui;

    GameLogic* game; //this will handle the game logic, only one instance will be created (whoever hosts the game)
    Server* server; //the person who hosts the game will also be the one who creates this server object
    Client* client; //each player will create a client object, which will communicate with the server to handle the GUI of the game

    QString playerName; //the player name of the window
    QString ip;  //for connecting to the server
    quint16 port;

    bool isHost; //will be true for the host, false for everyone else
    bool isConnect; //will be true if the client has connected to the server

    // Window Setting: Each set/delete pair will show/hide the corresponding elements
    void setRequestRoomWindow();
    void deleteRequestRoomWindow();
    void setCreateRoomWindow();
    void deleteCreateRoomWindow();
    void setJoinRoomWindow();
    void deleteJoinRoomWindow();
    void setRoomWindow();
    void deleteRoomWindow();
    void setInstructionWindow();
    void deleteInstructionWindow();

    //handling the room creation and player handling
    void destroyRoom();
    void joinRoom();
    void leaveRoom();
    void leaveRoomNoPrompt();
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
