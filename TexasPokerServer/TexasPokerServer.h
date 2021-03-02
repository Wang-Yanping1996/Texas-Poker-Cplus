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

#include "tcpCommand.h"
//�մ���ui
class emptyServerUI : public QMainWindow, public virUI {
	Q_OBJECT
public:
	emptyServerUI(QWidget *parent = Q_NULLPTR, game* g = nullptr);
	//gameָ������
	void setGamePoint(game* g) { this->m_game = g; };
	game* getGamePoint()const { return this->m_game; };

	void analyzeCommand(QByteArray received, const int fromPlayerIndex);		//���յ�������
	void sendCommandAndDataToPlayer(const int playerIndex, commandAndDataToClient toSend)const;	//�ײ㲻һ��
	void sendCommandAndDataToAll(commandAndDataToClient toSend)const;							//������ʵ�ֲ�һ��

	//�̳еĴ��麯��
	virtual void showCommonCards(vector<card> const& commonCards)const;
	virtual void hideCommonCards()const;
	virtual void showRound(gameRound nowRound)const;
	virtual void hideRound()const;
	virtual void showPot(const int potNum)const;
	virtual void hidePot()const;
	virtual void showBegin(const int playerIndex)const;
	virtual void hideBegin(const int playerIndex)const;
	virtual void showDealer() const;

	//������
	virtual void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const;	//��ʾ����������
	virtual void showPlayerName(const int playerIndex, string const& playerName)const;				//��ʾ������֣������е�client�ϣ����ظ���������д
	virtual void showPlayerChip(const int playerIndex, const int chip)const;		//��ʾchip������client��
	virtual void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const;	//��ʾmessage������client��	
	virtual void showPlayerSidePot(const int playerIndex, const int money)const;	//��ʾsidepot������client��	

	virtual void showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const;	//��5��ֻ��ʾ��nowPlayer client
	virtual void showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney)const;	
	virtual void showPlayerCheckAction(const int nowPlayerIndex)const;
	virtual void showPlayerCallAction(const int nowPlayerIndex, const int callMoney)const;
	virtual void showPlayerFoldAction(const int nowPlayerIndex)const;

	virtual void hidePlayerHandCards(const int playerIndex)const;	//��5������������client������player��card
	virtual void hidePlayerName(const int playerIndex)const;
	virtual void hidePlayerChip(const int playerIndex)const;
	virtual void hidePlayerActionMessage(const int playerIndex)const;
	virtual void hidePlayerSidePot(const int playerIndex)const;

	virtual void hidePlayerRaiseAction(const int nowPlayerIndex)const;	//��5��ֻ����playerIndex�Ľ���
	virtual void hidePlayerAllinAction(const int nowPlayerIndex)const;							
	virtual void hidePlayerCheckAction(const int nowPlayerIndex)const;
	virtual void hidePlayerCallAction(const int nowPlayerIndex)const;
	virtual void hidePlayerFoldAction(const int nowPlayerIndex)const;
	
	//����ӵ�show�Ʒ����������2��client��show���1����
	virtual void showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const& handCards)const;

private:
	game *m_game;		//gameʵ��

	vector<QTcpSocket*> m_arrayClient;
	QTcpServer *m_tcpServer;
	
	//��ճ��
	vector<QByteArray> m_buffers;
	vector<int> m_headLens;
	//���벻ͬclient����Ϣ
	map<QTcpSocket*, int> m_SocketMap;
	//�趨�����˿ں�
	QPushButton *m_setPort;
	QLineEdit *m_port;
	QLabel *m_portDisplay;
	//ԭ�����е�
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




