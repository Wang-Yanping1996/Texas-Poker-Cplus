#include "TexasPokerServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    testServer w;
    w.show();
    return a.exec();
}
