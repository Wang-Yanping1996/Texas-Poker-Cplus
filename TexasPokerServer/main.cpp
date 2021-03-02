#include "TexasPokerServer.h"
#include <QtWidgets/QApplication>
#include <QtCore/qsharedmemory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QSharedMemory singleton(a.applicationName());
	if (!singleton.create(1))
	{
		QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("程序已在运行中！"));
		return false;
	}

	game myGame("myGame");
    emptyServerUI w;

	//连接
	myGame.setVirUIPoint(&w);
	w.setGamePoint(&myGame);

    w.show();
    return a.exec();
}
