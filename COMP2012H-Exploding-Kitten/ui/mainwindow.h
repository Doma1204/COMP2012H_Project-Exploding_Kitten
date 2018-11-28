#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListView"
#include "server.h"
#include "client.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // UI
    enum Window {NoWindow = 0, RequestRoom, CreateRoom, JoinRoom, Room};
    Ui::MainWindow *ui;
    QPushButton *createRoomBtn;
    QPushButton *joinRoomBtn;
    QPushButton *backBtn;

    QLabel *serverNameLabel;
    QLabel *serverPortLabel;
    QLabel *nameLabel;

    QLineEdit *serverNameLineEdit;
    QLineEdit *serverPortLineEdit;
    QLineEdit *nameLineEdit;

    GameLogic* game;
    Server* server;
    Client* client;



    // Window Setting
    void setRequestRoomWindow();
    void deleteRequestRoomWindow();
    void setCreateRoomWindow();
    void deleteCreateRoomWindow();
    void setJoinRoomWindow();
    void deleteJoinRoomWindow();
    void setRoomWindow();
    void deleteRoomWindow();

private slots:
    void create_room_handler();
    void join_room_handler();
};

#endif // MAINWINDOW_H
