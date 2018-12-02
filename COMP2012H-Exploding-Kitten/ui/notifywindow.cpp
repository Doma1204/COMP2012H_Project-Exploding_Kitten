#include "notifywindow.h"
#include "gamewindow.h"
#include "ui_notifywindow.h"

#include <QSizePolicy>
#include <QJsonArray>
#include <QJsonValue>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>

NotifyWindow::NotifyWindow(QWidget *parent, NotifyType type, QJsonArray *cards) :
    QWidget(parent),
    ui(new Ui::NotifyWindow),
    parentWindow(dynamic_cast<GameWindow*>(parent)),
    type(type)
{
    ui->setupUi(this);

    switch (type) {
        case SHOW_SEE_THE_FUTURE: {
            QSizePolicy *policy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            QFont *cardFont = new QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Twisted System.otf")).at(0));
            this->setGeometry(0, 0, 425, 170);
            layout = new QHBoxLayout(this);
            for (int i = 0; i < cards->count(); ++i) {
                QString cardStr = cards->takeAt(i).toString();
                QLabel *newCard = new QLabel(cardStr);
                newCard->setSizePolicy(*policy);
                newCard->setFixedWidth(125);
                newCard->setFixedHeight(150);
                newCard->setFont(*cardFont);
                parentWindow->setCardStyle(newCard, cardStr);
                layout->addWidget(newCard);
            }
        }
        break;

        case SHOW_EXPLODING_KITTEN:
            break;
    }
}

NotifyWindow::~NotifyWindow()
{
    delete ui;
}
