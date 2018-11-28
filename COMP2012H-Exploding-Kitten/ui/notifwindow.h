#ifndef NOTIFWINDOW_H
#define NOTIFWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
namespace Ui {
class NotifWindow;
}

class NotifWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NotifWindow(QWidget *parent = nullptr, QString notif = "Notif");
    ~NotifWindow();
private:
    Ui::NotifWindow *ui;
    QPushButton *closeNotifBtn;
    QLabel *notifLabel;
};

#endif // NOTIFWINDOW_H
