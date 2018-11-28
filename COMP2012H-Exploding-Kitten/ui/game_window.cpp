#include "game_window.h"
#include "ui_game_window.h"

game_window::game_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::game_window)
{
    ui->setupUi(this);
    //Game UI
    this->setWindowTitle(QString("Exploding Kittens"));
    endTurnBtn = new QPushButton(QString("End Turn"),this);
    playCardBtn = new QPushButton(QString("Play Card"),this);
    deckLabel = new QLabel(QString("Deck Size: 30 Cards"),this);
    currentPlayerLabel = new QLabel(QString("Current Player: Player 1"),this);
    botPlayerLabel = new QLabel(QString("Player 1: 5 cards"),this);
    leftPlayerLabel = new QLabel(QString("Player 2: 5 Cards"),this);
    topPlayerLabel = new QLabel(QString("Player 3: 5 Cards"),this);
    rightPlayerLabel = new QLabel(QString("Player 4: 5 Cards"),this);
    handList = new QListView(this);

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
    this->show();
    connect(endTurnBtn, &QPushButton::clicked,this, &game_window::endTurnBtnHandler);
    connect(playCardBtn, &QPushButton::clicked,this,&game_window::playCardBtnHandler);
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
    endTurnBtn->setAutoDefault(false);
    playCardBtn->show();
    deckLabel->show();
    currentPlayerLabel->show();
    botPlayerLabel->show();
    leftPlayerLabel->show();
    topPlayerLabel->show();
    rightPlayerLabel->show();
    handList->show();
    this->show();
}

void game_window::deleteGameWindow(){

}

void game_window::endTurnBtnHandler(){
    notifWindow = new NotifWindow(nullptr,"asdf");
}
void game_window::playCardBtnHandler(){
    notifWindow = new NotifWindow(nullptr,"asdf");
}
