#include "tcpCommand.h"
//int转QByteArray
QByteArray intTo4Bytes(int i)
{
	QByteArray abyte0;
	abyte0.resize(4);
	abyte0[0] = (uchar)(0x000000ff & i);
	abyte0[1] = (uchar)((0x0000ff00 & i) >> 8);
	abyte0[2] = (uchar)((0x00ff0000 & i) >> 16);
	abyte0[3] = (uchar)((0xff000000 & i) >> 24);
	return abyte0;
}

//QByteArray 转int
int bytes4ToInt(QByteArray bytes)
{
	int addr = bytes[0] & 0x000000FF;
	addr |= ((bytes[1] << 8) & 0x0000FF00);
	addr |= ((bytes[2] << 16) & 0x00FF0000);
	addr |= ((bytes[3] << 24) & 0xFF000000);
	return addr;
}

commandAndDataToClient::commandAndDataToClient()
{
	m_command.clear(); 
	m_data.clear(); 
	m_command = intTo4Bytes((int)tcpCommandToClient::noCommandToClient);
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, string data)
{
	//待测试
	m_command = intTo4Bytes((int)command);
	//m_data = QByteArray(data.c_str(), data.length());
	m_data = QByteArray::fromStdString(data);
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, vector<card> cards)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
	for (auto const& card : cards) {
		m_data = m_data.append(intTo4Bytes((int)card.getColor()));
		m_data = m_data.append(intTo4Bytes((int)card.getNumber()));
	}
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, int num)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
	m_data = m_data.append(intTo4Bytes(num));
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, int num1, int num2)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
	m_data = m_data.append(intTo4Bytes(num1));
	m_data = m_data.append(intTo4Bytes(num2));
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, int num, string data)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
	m_data = m_data.append(intTo4Bytes(num));
	m_data = m_data.append(QByteArray::fromStdString(data));
}

commandAndDataToClient::commandAndDataToClient(tcpCommandToClient command, int num, vector<card> cards)
{
	m_command = intTo4Bytes((int)command);
	m_data = intTo4Bytes(num);
	for (auto const& card : cards) {
		m_data = m_data.append(intTo4Bytes((int)card.getColor()));
		m_data = m_data.append(intTo4Bytes((int)card.getNumber()));
	}
}

QByteArray commandAndDataToClient::getTcpSend() const
{
	QByteArray body = m_command;
	body.append(m_data);

	const int len = body.length();			//用的int
	QByteArray head = intTo4Bytes(len);
	QByteArray tcpSend = body.prepend(head);
	return tcpSend;
}

commandAndDataToServer::commandAndDataToServer()
{
	m_command.clear();
	m_data.clear();
	m_command = intTo4Bytes((int)tcpCommandToServer::noCommandToServer);
}

commandAndDataToServer::commandAndDataToServer(tcpCommandToServer command)
{
	m_command.clear();
	m_data.clear();
	m_command = intTo4Bytes((int)command);
}

commandAndDataToServer::commandAndDataToServer(tcpCommandToServer command, int num)
{
	m_command = intTo4Bytes((int)command);
	m_data.clear();
	m_data = m_data.append(intTo4Bytes(num));
}

QByteArray commandAndDataToServer::getTcpSend() const
{
	QByteArray body = m_command;
	body.append(m_data);

	const int len = body.length();			//用的int
	QByteArray head = intTo4Bytes(len);
	QByteArray tcpSend = body.prepend(head);
	return tcpSend;
}
