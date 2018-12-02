#ifndef NOTIFYWINDOW_H
#define NOTIFYWINDOW_H

#include <QDialog>
#include "gamewindow.h"

namespace Ui {
class NotifyWindow;
}

class NotifyWindow : public QDialog
{
    Q_OBJECT

public:
    enum NotifyType {
        SHOW_SEE_THE_FUTURE,
        SHOW_EXPLODING_KITTEN
    };

    explicit NotifyWindow(QWidget *parent = nullptr, NotifyType type = SHOW_SEE_THE_FUTURE, const QJsonArray* const cards = nullptr);
    ~NotifyWindow();

private:
    Ui::NotifyWindow *ui;
    GameWindow *parentWindow;
    NotifyType type;
};

#endif // NOTIFYWINDOW_H
