#include "mainwindow.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    std::srand(unsigned (std::time(nullptr)));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
