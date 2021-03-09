#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/qlineedit.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qpixmap.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qheaderview.h>
#include "game.h"
#include "tcpCommand.h"
#include "tcpPackageAnalyzer.h"

#include <string>
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

QString getCardFileName(card const& c);
vector<card> QByteArrayToCards(QByteArray const& qArray);
//class testUi : public QMainWindow
//{
//    Q_OBJECT
//
//public:
//	testUi(QWidget *parent = Q_NULLPTR);
//
//private:
//	QTcpSocket *m_tcpClient;
//	QPushButton *m_connect;
//	QLineEdit *m_address;
//
//	QMenuBar *menuBar;
//	QToolBar *mainToolBar;
//	QWidget *centralWidget;
//	QStatusBar *statusBar;
//public slots:
//	void connectTcp();
//
//};

class playerClient : public QWidget {
	Q_OBJECT

public:
	int handCardPosition[2][2] = { { 30,70 },{ 120,70 } };

	QLabel *playerHandCards[player::numOfHandCards];

	QLabel *playerName;
	QLabel *playerChip;
	QLabel *sidePot;

	QLabel *actionMessage;

	playerClient(QWidget *centralWidget, int x, int y, int playerIndex);

	void showHandCards(vector<card> const& c)const;
	void hideHandCards()const;
	void showPlayerName(string const& name)const;
	void hidePlayerName()const;
	void showPlayerChip(const int chip)const;
	void hidePlayerChip()const;
	void showPlayerActionMessage(QString const& actionMessage)const;
	void hidePlayerActionMessage()const;
	void showSidePot(const int money)const;
	void hideSidePot()const;

	////玩家行为
	//void showRaise(const int minRaiseMoney, const int maxRaiseMoney)const;
	//void hideRaise()const;
	//void showAllin(const int allMoney)const;
	//void hideAllin()const;
	//void showCheck()const;
	//void hideCheck()const;
	//void showCall(const int callMoney)const;
	//void hideCall()const;
	//void showFold()const;
	//void hideFold()const;
	//void hideAllAction()const;
	////组合行为
	//void allinFoldAction(const int allmoney)const;
	//void callRaiseFoldAction(const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const;
	//void checkRaiseFoldAction(const int minRaiseMoney, const int maxRaiseMoney)const;

	//void hideAll()const;
};

class TexasPokerClientUI : public QMainWindow
{
	Q_OBJECT

public:
	TexasPokerClientUI(QWidget *parent = Q_NULLPTR);

private:
	int commonCardsPosition[game::maxNumOfCommonCards][2] = { { 300,260 },{ 390,260 },{ 480,260 },{ 590,260 },{ 700,260 } };
	int playerPosition[game::maxNumOfPlayers][2] = { { 470,430 },{ 750,430 },{ 940,190 },{ 750,0 },{ 470,0 },{ 170,0 },{ 0,190 },{ 170,430 } };
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;	
	//player相关
	playerClient *players[game::maxNumOfPlayers];
	//client版本，只需要一个行动键
	QPushButton *raise;
	QPushButton *allin;		//和raise相同位置，名字不一样
	QPushButton *check;
	QPushButton *call;		//和check相同位置，名字不一样
	QPushButton *fold;
	QSpinBox *raiseMoney;
	//table相关
	QLabel *commonCards[game::maxNumOfCommonCards];
	QLabel *flop;
	QLabel *turn;
	QLabel *river;
	QLabel *pot;
	QPushButton *begin;			//有吗
	QLabel *dealer;
	//连接相关
	QLineEdit *m_name;
	QTcpSocket *m_tcpClient;
	QPushButton *m_connect;
	QLineEdit *m_address;
	QLineEdit *m_port;
	//防止粘包
	tcpPackageAnalyzer m_tcpPackageAnalyzer;
	/*QByteArray m_buffer;
	int m_headLen;*/
	//记录当前client玩家编号
	int m_clientPlayerIndex;
	
	string macAddress;			//本机第一个mac地址
public:
	//获取第一个以太网卡mac地址
	bool getMacByGetAdaptersInfo();

	//tcp命令解析
	void analyzeCommand(QByteArray received);
	void sendCommandAndDataToServer(commandAndDataToServer toSend)const;

	//桌子
	void showCommonCards(vector<card> const& commonCards)const;
	void hideCommonCards()const;
	void showRound(gameRound nowRound)const;
	void hideRound()const;
	void showPot(const int potNum)const;
	void hidePot()const;
	void showBegin()const { this->begin->show(); };
	void hideBegin()const { this->begin->hide(); };

	//玩家相关
	void setClientPlayerIndex(const int index) { this->m_clientPlayerIndex = index; };

	void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const;
	void showPlayerName(const int playerIndex, string const& playerName)const;
	void showPlayerChip(const int playerIndex, const int chip)const;
	void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const;
	void showPlayerSidePot(const int playerIndex, const int money)const;
	void showPlayerDealer(const int playerIndex)const;

	void hidePlayerHandCards(const int playerIndex)const;
	void hidePlayerName(const int playerIndex)const;
	void hidePlayerChip(const int playerIndex)const;
	void hidePlayerActionMessage(const int playerIndex)const;
	void hidePlayerSidePot(const int playerIndex)const;
	//只显示当前client的行动
	virtual void showClientPlayerRaiseAction(const int minRaiseMoney, const int maxRaiseMoney)const;
	virtual void showClientPlayerAllinAction(const int allinMoney)const;
	virtual void showClientPlayerCheckAction()const;
	virtual void showClientPlayerCallAction(const int callMoney)const;
	virtual void showClientPlayerFoldAction()const;

	virtual void hideClientPlayerRaiseAction()const;
	virtual void hideClientPlayerAllinAction()const;
	virtual void hideClientPlayerCheckAction()const;
	virtual void hideClientPlayerCallAction()const;
	virtual void hideClientPlayerFoldAction()const;
	//防止重复按键
	void hideClientPlayerAllAction()const;
public slots:
	void connectTcp();
	void nowPlayerRaise();
	void nowPlayerAllin();
	void nowPlayerCheck();
	void nowPlayerCall();
	void nowPlayerFold();
	void clientReady();
	void readData();
	void disconnectTcp();
	void aboutSlot();
	void aboutCardTypeSlot();
	void showScoreChartSlot();
};