#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include "notifwindow.h"
#include "client.h"
#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
namespace Ui {
class game_window;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr, Client* client = nullptr, int playerNum = 2, QString name = "Player", QMap<QString,int> playerNames = QMap<QString,int>());
    ~GameWindow();
    NotifWindow* notifWindow;

private:
    Ui::game_window *ui;
    //Game UI
    QPushButton *endTurnBtn;
    QPushButton *playCardBtn;

    QLabel *deckLabel;
    QLabel *currentPlayerLabel;
    QVector<QLabel*> playerLabel;

    QListWidget *handList;
    Client* client;
    QMap<QString,int> playerOrder;
    QString playerName;


private slots:
    void endTurnBtnHandler();
    void playCardBtnHandler();
    void clientJsonReceived(const QJsonObject &json);
};

#endif // GAME_WINDOW_H
