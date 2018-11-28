#include "notifwindow.h"
#include "ui_notifwindow.h"

NotifWindow::NotifWindow(QWidget *parent, QString notif) :
    QWidget(parent),
    ui(new Ui::NotifWindow)
{
    ui->setupUi(this);
    closeNotifBtn = new QPushButton(QString("Close"),this);
    notifLabel = new QLabel(notif,this);
    closeNotifBtn->setGeometry(240,160,114,31);
    notifLabel->setGeometry(80,50,161,21);
    notifLabel->setAlignment(Qt::AlignCenter);
    closeNotifBtn->show();
    notifLabel->show();
    this->show();
    connect(closeNotifBtn,&QPushButton::clicked,this,&NotifWindow::close);
}



NotifWindow::~NotifWindow()
{
    delete ui;
}
