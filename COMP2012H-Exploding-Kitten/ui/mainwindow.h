#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamelogic.h"
#include "server.h"
#include "client.h"

#include "gamelogic.h"
#include "game_window.h"

#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    game_window* gameWindow;
private:
    // UI
    enum Window {NoWindow = 0, RequestRoom, CreateRoom, JoinRoom, Room};
    Ui::MainWindow *ui;
    QPushButton *createRoomBtn;
    QPushButton *joinRoomBtn;
    QPushButton *backBtn;
    QPushButton *leaveBtn;
    QPushButton *startBtn;

    QLabel *serverNameLabel;
    QLabel *serverPortLabel;
    QLabel *nameLabel;

    QLineEdit *serverNameLineEdit;
    QLineEdit *serverPortLineEdit;
    QLineEdit *nameLineEdit;

    QListWidget *playerList;

    GameLogic* game;
    Server* server;
    Client* client;
    QString playerName;
    bool isHost;

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
    void joinRoom(QString ip, quint16 port);
    void addPlayer(const QString &playerName);
    void removePlayer(const QString &playerName);

private slots:
    void create_room_handler();
    void join_room_handler();

    void sendPlayerName();
    void clientJsonReceived(const QJsonObject &json);
    void jsonReceived(ServerWorker *sender, const QJsonObject& json);

};

#endif // MAINWINDOW_H
