#include "TexasPokerClientUI.h"
#include <QtWidgets/QApplication>
#include <QtCore/qsharedmemory.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QSharedMemory singleton(a.applicationName());
	if (!singleton.create(1))
	{
		QMessageBox::warning(NULL, QStringLiteral("����"), QStringLiteral("�������������У�"));
		return false;
	}
    TexasPokerClientUI w;
    w.show();
    return a.exec();
}
