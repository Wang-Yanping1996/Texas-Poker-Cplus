#pragma once
#include "game.h"
#include "qbytearray.h"
#include "qvariant.h"
//#pragma pack(push, 1)		//1×Ö½Ú¶ÔÆë

QByteArray intTo4Bytes(int i);
int bytes4ToInt(QByteArray bytes);

enum tcpCommand {
	noCommand = 0x00000000,

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

class commandAndData {
public:
	static const int byteOfInt = 4;
	static const int byteOfCommand = byteOfInt;

	commandAndData();
	commandAndData(tcpCommand command, string data);
	commandAndData(tcpCommand command, vector<card> cards);
	commandAndData(tcpCommand command, int num);
	commandAndData(tcpCommand command, int num1, int num2);
	commandAndData(tcpCommand command, int num, string data);

	/*tcpCommand getCommand()const { return this->m_command; };
	QVariant getData()const { return this->m_data; };

	void setCommand(tcpCommand command = noCommand) { this->m_command = command; };
	void setDate(QVariant data = QVariant()) { this->m_data = data; };*/
	QByteArray getTcpSend()const;
private:
	QByteArray m_command;
	QByteArray m_data;
};
Q_DECLARE_METATYPE(tcpCommand)
Q_DECLARE_METATYPE(commandAndData)
Q_DECLARE_METATYPE(card)
Q_DECLARE_METATYPE(vector<card>)
Q_DECLARE_METATYPE(gameRound)
Q_DECLARE_METATYPE(string)
//Q_DECLARE_METATYPE(int)

//class wcommandAndData {
//	static const int commandByte = 8;
//	static const int dataLenByte = 1;
//	static const int dataByte = 40;
//public:
//	wcommandAndData();
//	wcommandAndData(tcpCommand command, int dataLen, string data);
//	wcommandAndData(tcpCommand command, int dataLen, vector<card> cards);
//	wcommandAndData(tcpCommand command, int dataLen, int num1, int num2);
//
//
//	tcpCommand getCommand()const { return this->m_command; };
//	QVariant getData()const { return this->m_data; };
//
//	void setCommand(tcpCommand command = noCommand) { this->m_command = command; };
//	void setDate(QVariant data = QVariant()) { this->m_data = data; };
//private:
//	QByteArray m_command;
//	QByteArray m_dataLen;
//	QByteArray m_data;
//};


