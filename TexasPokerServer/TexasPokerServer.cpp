#include "TexasPokerServer.h"

testServer::testServer(QWidget *parent)
	:QMainWindow(parent)
{

	m_tcpServer = new QTcpServer(this);
	QString hostAddress = QNetworkInterface().allAddresses().at(1).toString();
	quint16 port = 6060;
	m_tcpServer->listen(QHostAddress::Any, port);
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));

}

void testServer::newConnectionSlot() {
	QString testMessage = "连接成功";
	QTcpSocket *currentClient = m_tcpServer->nextPendingConnection();
	m_arrayClient.append(currentClient);		//加入链表中

	//返回数据给client
	//此处告知玩家在本局游戏中的序号
	commandAndData setClientIndex(tcpCommand::setClientPlayerIndex, 0);
	currentClient->write(setClientIndex.getTcpSend());

	commandAndData toSend(tcpCommand::showPlayerActionMessageCommand, 1, "我是你爹");
	QByteArray tcpSend = toSend.getTcpSend();
	currentClient->write(tcpSend);
}
