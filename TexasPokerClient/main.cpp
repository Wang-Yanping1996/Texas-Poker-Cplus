#include "TexasPokerClientUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TexasPokerClientUI w;
    w.show();
    return a.exec();
}
