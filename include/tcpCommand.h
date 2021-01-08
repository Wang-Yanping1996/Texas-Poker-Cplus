#pragma once
#include "game.h"
#include "qbytearray.h"
//#pragma pack(push, 1)		//1×Ö½Ú¶ÔÆë

QByteArray intTo4Bytes(int i);
int bytes4ToInt(QByteArray bytes);

enum tcpCommandToClient {
	noCommandToClient = 0x00000000,

	showCommonCardsCommand = 0x00000001,
	hideCommonCardsCommand = 0x00000002,
	showRoundCommand = 0x00000004,
	hideRoundCommand = 0x00000008,

	showPotCommand = 0x00000010,
	hidePotCommand = 0x00000020,
	showBeginCommand = 0x00000040,
	hideBeginCommand = 0x00000080,

	showPlayerHandCardsCommand = 0x00000100,
	showPlayerNameCommand = 0x00000200,
	showPlayerChipCommand = 0x00000400,
	showPlayerActionMessageCommand = 0x00000800,
	
	showPlayerSidePotCommand = 0x00001000,
	hidePlayerHandCardsCommand = 0x00002000,
	hidePlayerNameCommand = 0x00004000,
	hidePlayerChipCommand = 0x00008000,

	hidePlayerActionMessageCommand = 0x00010000,
	hidePlayerSidePotCommand = 0x00020000,
	showClientPlayerRaiseActionCommand = 0x00040000,
	showClientPlayerAllinActionCommand = 0x00080000,

	showClientPlayerCheckActionCommand = 0x00100000,
	showClientPlayerCallActionCommand = 0x00200000,
	showClientPlayerFoldActionCommand = 0x00400000,
	hideClientPlayerRaiseActionCommand = 0x00800000,

	hideClientPlayerAllinActionCommand = 0x01000000,
	hideClientPlayerCheckActionCommand = 0x02000000,
	hideClientPlayerCallActionCommand = 0x04000000,
	hideClientPlayerFoldActionCommand = 0x08000000,

	setClientPlayerIndex = 0x10000000
};

enum tcpCommandToServer {
	noCommandToServer = 0x00000000,

	nowPlayerRaiseCommand = 0x00000001,
	nowPlayerAllinCommand = 0x00000002,
	nowPlayerCheckCommand = 0x00000004,
	nowPlayerCallCommand = 0x00000008,

	nowPlayerFoldCommand = 0x00000010,
	playerReadyCommand = 0x00000020,
	setPlayerNameCommand = 0x00000040
};
class commandAndDataToClient {
public:
	static const int byteOfInt = 4;
	static const int byteOfCommand = byteOfInt;

	commandAndDataToClient();
	commandAndDataToClient(tcpCommandToClient command);
	commandAndDataToClient(tcpCommandToClient command, string data);
	commandAndDataToClient(tcpCommandToClient command, vector<card> cards);
	commandAndDataToClient(tcpCommandToClient command, int num);
	commandAndDataToClient(tcpCommandToClient command, int num1, int num2);
	commandAndDataToClient(tcpCommandToClient command, int num, string data);
	commandAndDataToClient(tcpCommandToClient command, int num, vector<card> cards);

	/*tcpCommand getCommand()const { return this->m_command; };
	QVariant getData()const { return this->m_data; };

	void setCommand(tcpCommand command = noCommand) { this->m_command = command; };
	void setDate(QVariant data = QVariant()) { this->m_data = data; };*/
	QByteArray getTcpSend()const;
private:
	QByteArray m_command;
	QByteArray m_data;
};

class commandAndDataToServer {
public:
	static const int byteOfInt = 4;
	static const int byteOfCommand = byteOfInt;
	commandAndDataToServer();
	commandAndDataToServer(tcpCommandToServer command);
	commandAndDataToServer(tcpCommandToServer command, int num);
	commandAndDataToServer(tcpCommandToServer command, string const data);

	QByteArray getTcpSend()const;

private:
	QByteArray m_command;
	QByteArray m_data;
};


