#ifndef NOTIFYWINDOW_H
#define NOTIFYWINDOW_H

#include <QWidget>
#include "gamewindow.h"
#include <QHBoxLayout>

namespace Ui {
class NotifyWindow;
}

class NotifyWindow : public QWidget
{
    Q_OBJECT

public:
    enum NotifyType {
        SHOW_SEE_THE_FUTURE,
        SHOW_EXPLODING_KITTEN
    };

    explicit NotifyWindow(QWidget *parent = nullptr, NotifyType type = SHOW_SEE_THE_FUTURE, QJsonArray *cards = nullptr);
    ~NotifyWindow();

private:
    Ui::NotifyWindow *ui;
//    QHBoxLayout *layout;
    GameWindow *parentWindow;
    NotifyType type;

};

#endif // NOTIFYWINDOW_H
