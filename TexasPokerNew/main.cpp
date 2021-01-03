#include "TexasPokerNew.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TexasPokerNew w;
    w.show();
    return a.exec();
}
