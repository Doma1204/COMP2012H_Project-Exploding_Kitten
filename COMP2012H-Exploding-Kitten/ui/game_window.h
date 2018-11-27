#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QDialog>

namespace Ui {
class game_window;
}

class game_window : public QDialog
{
    Q_OBJECT

public:
    explicit game_window(QWidget *parent = nullptr);
    ~game_window();
    void update_game_window();

private:
    Ui::game_window *ui;
};

#endif // GAME_WINDOW_H
