#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QDialog>
#include <QHBoxLayout>
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
    struct Player {
        QVBoxLayout *layout;
        QLabel *icon;
        QLabel *name;
        QLabel *card;
    };

    Ui::game_window *ui;
    //Game UI
    QHBoxLayout *handLayout;
    QSizePolicy *cardSizePolicy;
    QFont *cardFont;
    QFont *textFont;



    QVector<Player*> playerLabel;

    QLabel *recentMove;
    Client* client;
    QMap<QString,int> playerOrder;
    QString playerName;

    Player* createNewPlayer(QString name);
    void setPlayerCard(Player *player, int cardNum);
    void setPlayerExploding(Player *player);
    void setPlayerDead(Player *player);

    void newCard(QString cardType);
    void setCardStyle(QWidget *widget, QString cardType);
    void clearLayout(QLayout *layout);

private slots:
    void endTurnBtnHandler();
    void playCardBtnHandler(QPushButton *card, QString currentCard);
    void clientJsonReceived(const QJsonObject &json);
};

#endif // GAME_WINDOW_H