#include "notifywindow.h"
#include "ui_notifywindow.h"
#include "gamewindow.h"

#include <QSizePolicy>
#include <QJsonArray>
#include <QJsonValue>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>

NotifyWindow::NotifyWindow(QWidget *parent, NotifyType type, const QJsonArray* const cards):
    QDialog(parent),
    ui(new Ui::NotifyWindow),
    parentWindow(dynamic_cast<GameWindow*>(parent)),
    type(type)
{
    ui->setupUi(this);

    switch (type) {
        case SHOW_SEE_THE_FUTURE: {
            this->setWindowTitle("See The Future");
            QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            QFont cardFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Twisted System.otf")).at(0));
            cardFont.setPointSize(30);
            this->setFixedWidth(425);
            this->setFixedHeight(230);
            for (int i = 0; i < cards->size(); ++i) {
                QString cardStr = (*cards)[i].toString();
                if (cardStr == "No Card")
                    continue;

                QLabel *cardOrder = new QLabel(QString::number(i + 1));
                cardOrder->setFont(cardFont);
                cardOrder->setStyleSheet("color: white;");
                cardOrder->setAlignment(Qt::AlignCenter);

                QLabel *newCard = new QLabel(cardStr == "SEE_THE_FUTURE" ? "SEE\nTHE\nFUTURE" : cardStr);
                newCard->setSizePolicy(policy);
                newCard->setFixedWidth(125);
                newCard->setFixedHeight(150);
                newCard->setFont(cardFont);
                newCard->setAlignment(Qt::AlignCenter);

                if (cardStr == "EXPLODING_KITTEN") {
                    newCard->setText("EXPLODING\nKITTEN");
                    newCard->setStyleSheet("color: black; background-color: white; border: 7px solid black; border-radius: 20px");
                } else {
                    parentWindow->setCardStyle(newCard, cardStr);
                }
                ui->layout->addWidget(cardOrder, 0, i);
                ui->layout->addWidget(newCard, 1, i);
//                ui->layout->addWidget(newCard);
            }
        }
        break;

        case SHOW_EXPLODING_KITTEN:
            this->setWindowTitle("Exploded");
            this->setFixedWidth(589);
            this->setFixedHeight(330);
            this->setStyleSheet("background-image:url(\":/resource/resource/image/Exploded_Image.png\"); background-position: center;");
            break;
    }

    this->show();
}

NotifyWindow::~NotifyWindow()
{
    parentWindow->clearLayout(ui->layout);
    delete ui;
}
