#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include "notifwindow.h"
#include <QtDebug>
namespace Ui {
class game_window;
}

class game_window : public QDialog
{
    Q_OBJECT

public:
    explicit game_window(QWidget *parent = nullptr);
    ~game_window();
    NotifWindow* notifWindow;

private:
    Ui::game_window *ui;
    //Game UI
    QPushButton *endTurnBtn;
    QPushButton *playCardBtn;

    QLabel *deckLabel;
    QLabel *currentPlayerLabel;
    QLabel *leftPlayerLabel;
    QLabel *rightPlayerLabel;
    QLabel *topPlayerLabel;
    QLabel *botPlayerLabel;

    QListView *handList;

    //Game UI
    void setGameWindow();
    void deleteGameWindow();
private slots:
    void endTurnBtnHandler();
    void playCardBtnHandler();
};

#endif // GAME_WINDOW_H
