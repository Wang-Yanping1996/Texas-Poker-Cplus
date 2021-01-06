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
	QString testMessage = "���ӳɹ�";
	QTcpSocket *currentClient = m_tcpServer->nextPendingConnection();
	m_arrayClient.append(currentClient);		//����������

	//�������ݸ�client
	//�˴���֪����ڱ�����Ϸ�е����
	commandAndData setClientIndex(tcpCommand::setClientPlayerIndex, 0);
	currentClient->write(setClientIndex.getTcpSend());

	commandAndData toSend(tcpCommand::showPlayerActionMessageCommand, 1, "�������");
	QByteArray tcpSend = toSend.getTcpSend();
	currentClient->write(tcpSend);
}
