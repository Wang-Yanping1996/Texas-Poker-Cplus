#pragma once
#include <QtWidgets/qmainwindow.h>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qnetworkinterface.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include "tcpCommand.h"
class testServer : public QMainWindow {
	Q_OBJECT
public:
	testServer(QWidget *parent = Q_NULLPTR);
private:
	QTcpServer *m_tcpServer;
	QList<QTcpSocket*> m_arrayClient;
private slots:
	void newConnectionSlot();
};




