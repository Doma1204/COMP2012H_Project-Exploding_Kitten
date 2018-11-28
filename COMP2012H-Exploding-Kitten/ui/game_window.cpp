#include "game_window.h"
#include "ui_game_window.h"

game_window::game_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::game_window)
{
    ui->setupUi(this);
    //Game UI
    endTurnBtn = new QPushButton(QString("End Turn"));
    playCardBtn = new QPushButton(QString("Play Card"));
    deckLabel = new QLabel(QString("Deck Size: 30 Cards"));
    currentPlayerLabel = new QLabel(QString("Current Player: Player 1"));
    botPlayerLabel = new QLabel(QString("Player 1: 5 cards"));
    leftPlayerLabel = new QLabel(QString("Player 2: 5 Cards"));
    topPlayerLabel = new QLabel(QString("Player 3: 5 Cards"));
    rightPlayerLabel = new QLabel(QString("Player 4: 5 Cards"));
    handList = new QListView();

    endTurnBtn->hide();
    playCardBtn->hide();
    deckLabel->hide();
    currentPlayerLabel->hide();
    botPlayerLabel->hide();
    leftPlayerLabel->hide();
    topPlayerLabel->hide();
    rightPlayerLabel->hide();
    handList->hide();

    setGameWindow();
}

game_window::~game_window()
{
    delete ui;
}

void game_window::setGameWindow(){
    endTurnBtn->setGeometry(661,470,91,31);
    playCardBtn->setGeometry(661,510,91,31);
    deckLabel->setGeometry(320,280,161,31);
    currentPlayerLabel->setGeometry(320,200,161,31);
    botPlayerLabel->setGeometry(330,430,51,41);
    leftPlayerLabel->setGeometry(50,30,121,41);
    topPlayerLabel->setGeometry(320,30,121,41);
    rightPlayerLabel->setGeometry(580,30,131,41);
    handList->setGeometry(60,460,561,91);
    handList->show();
    endTurnBtn->show();
    playCardBtn->show();
    deckLabel->show();
    currentPlayerLabel->show();
    botPlayerLabel->show();
    leftPlayerLabel->show();
    topPlayerLabel->show();
    rightPlayerLabel->show();
    handList->show();
}

void game_window::deleteGameWindow(){

}
void game_window::createGameWindow(){

}
