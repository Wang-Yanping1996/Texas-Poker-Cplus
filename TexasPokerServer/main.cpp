#include "TexasPokerServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
	//game myGame("myGame");
    emptyServerUI w;

	//����
	//myGame.setVirUIPoint(&w);
	//w.setGamePoint(&myGame);

    w.show();
    return a.exec();
}
