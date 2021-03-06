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
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qbuttongroup.h>
#include <qvalidator.h>

#include "tcpCommand.h"
#include "tcpPackageAnalyzer.h"

#include <unordered_set>
//空窗口ui
class emptyServerUI : public QMainWindow, public virUI {
	Q_OBJECT
public:
	emptyServerUI(QWidget *parent = Q_NULLPTR, game* g = nullptr);
	//game指针设置
	void setGamePoint(game* g) { this->m_game = g; };
	game* getGamePoint()const { return this->m_game; };

	void analyzeCommand(QByteArray received, const int fromPlayerIndex);		//接收到的命令
	void sendCommandAndDataToPlayer(const int playerIndex, commandAndDataToClient toSend)const;		//底层不一样
	void sendCommandAndDataToAll(commandAndDataToClient toSend)const;								//和toPlayer实现不一样
	void sendCommandAndDataToAllExcept(const int exceptPlayerIndex, commandAndDataToClient toSend);	//和toAll类似

	//继承的纯虚函数
	virtual void showCommonCards(vector<card> const& commonCards)const;
	virtual void hideCommonCards()const;
	virtual void showRound(gameRound nowRound)const;
	virtual void hideRound()const;
	virtual void showPot(const int potNum)const;
	virtual void hidePot()const;
	virtual void showBegin(const int playerIndex)const;
	virtual void hideBegin(const int playerIndex)const;
	virtual void showDealer() const;

	//玩家相关
	virtual void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const;	//显示玩家自身的牌
	virtual void showPlayerName(const int playerIndex, string const& playerName)const;				//显示玩家名字，在所有的client上，有重复，可以重写
	virtual void showPlayerChip(const int playerIndex, const int chip)const;		//显示chip在所有client上
	virtual void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const;	//显示message在所有client上	
	virtual void showPlayerSidePot(const int playerIndex, const int money)const;	//显示sidepot在所有client上	

	virtual void showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const;	//这5个只显示给nowPlayer client
	virtual void showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney)const;	
	virtual void showPlayerCheckAction(const int nowPlayerIndex)const;
	virtual void showPlayerCallAction(const int nowPlayerIndex, const int callMoney)const;
	virtual void showPlayerFoldAction(const int nowPlayerIndex)const;

	virtual void hidePlayerHandCards(const int playerIndex)const;	//这5个，隐藏所有client界面下player的card
	virtual void hidePlayerName(const int playerIndex)const;
	virtual void hidePlayerChip(const int playerIndex)const;
	virtual void hidePlayerActionMessage(const int playerIndex)const;
	virtual void hidePlayerSidePot(const int playerIndex)const;

	virtual void hidePlayerRaiseAction(const int nowPlayerIndex)const;	//这5个只隐藏playerIndex的界面
	virtual void hidePlayerAllinAction(const int nowPlayerIndex)const;							
	virtual void hidePlayerCheckAction(const int nowPlayerIndex)const;
	virtual void hidePlayerCallAction(const int nowPlayerIndex)const;
	virtual void hidePlayerFoldAction(const int nowPlayerIndex)const;
	
	//后添加的show牌方法，在玩家2的client上show玩家1的牌
	virtual void showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const& handCards)const;

	//后添加的，带时间的行动信息显示
	virtual void showPlayerActionMessageWithTime(const int playerIndex)const;
	virtual void stopAllClientTimer()const;

	//analyze部分抽出的函数
	void setPlayerUniqueName(const int playerIndex, const string& playerName);
	void setPlayerUniqueMacAddress(const int playerIndex, const string& macAddress);
private:
	game *m_game;		//game实体

	vector<QTcpSocket*> m_arrayClient;
	QTcpServer *m_tcpServer;
	
	//防粘包
	vector<tcpPackageAnalyzer> m_tcpPackageAnalyzers; 
	//vector<QByteArray> m_buffers;
	//vector<int> m_headLens;
	//分离不同client的消息
	map<QTcpSocket*, int> m_SocketMap;
	//界面显示控件
	QPushButton *m_setPort;		//设定监听端口号
	QLineEdit *m_port;
	QLabel *m_portDisplay;

	QButtonGroup *m_gameMode;	//游戏模式，目前为正常和短牌（排除2~5）
	QRadioButton *nomalMode;
	QRadioButton *shortDeckMode;
	QLabel *m_gameModeDisplay;

	int m_timePerAction;					//思考时间sec，默认-1
	QLineEdit *m_timePerActionInput;		//输入每次思考时间
	QLabel *m_timePerActionShow;			//显示每次思考时间
	//原来就有的
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;

private slots:
	void newConnectionSlot();
	void readData();
	void disconnectionSlot();
	void setPort();
	void closeEvent(QCloseEvent * e);
};




