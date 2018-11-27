#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server(nullptr),
    client(nullptr)
{
    ui->setupUi(this);
    connect(ui->createRoom,&QPushButton::clicked, this,&MainWindow::create_room_handler);
    connect(ui->joinRoom,&QPushButton::clicked, this,&MainWindow::join_room_handler);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_room_handler(){

    server = new Server();
    server->show();
    this->hide();
}
void MainWindow::join_room_handler(){
    game = new GameLogic(false);
    this->hide();
}
