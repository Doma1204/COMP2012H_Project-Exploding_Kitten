#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverNameLabel(new QLabel(QString("Server Name:"), this)),
    serverPortLabel(new QLabel(QString("Server Port:"), this)),
    nameLabel(new QLabel(QString("Name:"), this)),
    server(nullptr),
    client(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Exploding Kittens"));
    createRoomBtn = new QPushButton(QString("Create Room") ,this);
    joinRoomBtn = new QPushButton(QString("Join Room"), this);
    backBtn = new QPushButton(QString("Back"), this);

    serverNameLineEdit = new QLineEdit(this);
    serverPortLineEdit = new QLineEdit(this);
    nameLineEdit = new QLineEdit(this);

    createRoomBtn->hide();
    joinRoomBtn->hide();
    backBtn->hide();
    serverNameLabel->hide();
    serverPortLabel->hide();
    nameLabel->hide();
    serverNameLineEdit->hide();
    serverPortLineEdit->hide();
    nameLineEdit->hide();


    setRequestRoomWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRequestRoomWindow() {
    createRoomBtn->setGeometry(125, 170, 150, 30);
    joinRoomBtn->setGeometry(125, 200, 150, 30);

    //    createRoomBtn->setStyleSheet(QString("QPushButton{background:white}"));
    //    joinRoomBtn->setStyleSheet(QString("QPushButton{background:white}"));

    createRoomBtn->show();
    joinRoomBtn->show();

    createRoomBtn->disconnect();
    joinRoomBtn->disconnect();
    connect(createRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setCreateRoomWindow();});
    connect(joinRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setJoinRoomWindow();});
}

void MainWindow::deleteRequestRoomWindow() {
    createRoomBtn->hide();
    joinRoomBtn->hide();
}

void MainWindow::setCreateRoomWindow() {
    nameLabel->setGeometry(75, 85, 50, 50);
    nameLineEdit->setGeometry(125, 100, 200, 20);
    createRoomBtn->setGeometry(275, 270, 125, 30);
    backBtn->setGeometry(150, 270, 125, 30);

    nameLabel->show();
    nameLineEdit->show();
    createRoomBtn->show();
    backBtn->show();

    createRoomBtn->disconnect();
    backBtn->disconnect();
    connect(backBtn, &QPushButton::clicked, this, [this]() {deleteCreateRoomWindow(); setRequestRoomWindow();});
    connect(createRoomBtn, &QPushButton::clicked, this, &MainWindow::create_room_handler);
}

void MainWindow::deleteCreateRoomWindow() {
    nameLabel->hide();
    nameLineEdit->hide();
    createRoomBtn->hide();
    backBtn->hide();
}

void MainWindow::setJoinRoomWindow() {
    serverNameLabel->setGeometry(50, 85, 100, 50);
    serverPortLabel->setGeometry(60, 105, 100, 50);
    nameLabel->setGeometry(95, 125, 50, 50);
    serverNameLineEdit->setGeometry(140, 100, 200, 20);
    serverPortLineEdit->setGeometry(140, 120, 200, 20);
    nameLineEdit->setGeometry(140, 140, 200, 20);
    joinRoomBtn->setGeometry(275, 270, 125, 30);
    backBtn->setGeometry(150, 270, 125, 30);

    serverNameLabel->show();
    serverNameLineEdit->show();
    serverPortLabel->show();
    serverPortLineEdit->show();
    nameLabel->show();
    nameLineEdit->show();
    joinRoomBtn->show();
    backBtn->show();

    joinRoomBtn->disconnect();
    backBtn->disconnect();
    connect(backBtn, &QPushButton::clicked, this, [this]() {deleteJoinRoomWindow(); setRequestRoomWindow();});
    connect(joinRoomBtn, &QPushButton::clicked, this, &MainWindow::join_room_handler);
}

void MainWindow::deleteJoinRoomWindow() {
    serverNameLabel->hide();
    serverNameLineEdit->hide();
    serverPortLabel->hide();
    serverPortLineEdit->hide();
    nameLabel->hide();
    nameLineEdit->hide();
    joinRoomBtn->hide();
    backBtn->hide();
}

void MainWindow::setRoomWindow() {

}



void MainWindow::create_room_handler(){
    qDebug("Create Room");
    QString userName = nameLineEdit->text();
}
void MainWindow::join_room_handler(){

    qDebug("Join Room");
}
