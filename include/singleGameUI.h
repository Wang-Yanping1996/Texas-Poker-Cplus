#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include "game.h"

//QString getCardFileName(card const& c);

class playerWindow : public QWidget {
	Q_OBJECT
signals:
	void beginClicked();
public slots:
	void playerReady();
public:
	int handCardPosition[2][2] = { { 30,70 },{ 120,70 } };

	QLabel *playerHandCards[player::numOfHandCards];
	QPushButton *raise;
	QPushButton *allin;		//和raise相同位置，名字不一样
	QPushButton *check;
	QPushButton *call;		//和check相同位置，名字不一样
	QPushButton *fold;
	QSpinBox *raiseMoney;
	QLabel *playerName;
	QLabel *playerChip;
	QLabel *sidePot;

	QLabel *actionMessage;
	QPushButton *begin;

	playerWindow(QWidget *centralWidget, int x, int y, int playerIndex);

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
	//玩家行为
	void showRaise(const int minRaiseMoney, const int maxRaiseMoney)const;
	void hideRaise()const;
	void showAllin(const int allMoney)const;
	void hideAllin()const;
	void showCheck()const;
	void hideCheck()const;
	void showCall(const int callMoney)const;
	void hideCall()const;
	void showFold()const;
	void hideFold()const;
	void hideAllAction()const;
	//组合行为
	void allinFoldAction(const int allmoney)const;
	void callRaiseFoldAction(const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const;
	void checkRaiseFoldAction(const int minRaiseMoney, const int maxRaiseMoney)const;

	void hideAll()const;
};

class singleGameWindow: public QMainWindow, public virUI {
	Q_OBJECT
public slots:
	void about();
	//void testShow();
	void nowPlayerRaise();
	void nowPlayerAllin();
	void nowPlayerCheck();
	void nowPlayerCall();
	void nowPlayerFold();
	void gameBegin();
public:
	int commonCardsPosition[game::maxNumOfCommonCards][2] = { { 300,260 },{ 390,260 },{ 480,260 },{ 590,260 },{ 700,260 } };
	int playerPosition[game::maxNumOfPlayers][2] = { { 470,430 },{ 750,430 },{ 940,190 },{ 750,0 },{ 470,0 },{ 170,0 },{ 0,190 },{ 170,430 } };

	game *m_game;

	playerWindow *players[game::maxNumOfPlayers];
	QLabel *commonCards[game::maxNumOfCommonCards];
	QLabel *flop;
	QLabel *turn;
	QLabel *river;
	QLabel *pot;
	//QPushButton *begin;

	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;

	singleGameWindow(QWidget *parent = Q_NULLPTR, game* g = nullptr);

	//game指针设置
	void setGamePoint(game* g) { this->m_game = g; };
	game* getGamePoint()const { return this->m_game; };

	//桌子
	void showCommonCards(vector<card> const& commonCards)const;
	void hideCommonCards()const;
	void showRound(gameRound nowRound)const;
	void hideRound()const;
	void showPot(const int potNum)const;
	void hidePot()const;
	void showBegin(const int playerIndex)const { this->players[playerIndex]->begin->show(); };
	void hideBegin(const int playerIndex)const { this->players[playerIndex]->begin->hide(); };
	void showDealer()const {};
	//玩家相关
	void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const;
	void showPlayerName(const int playerIndex, string const& playerName)const;
	void showPlayerChip(const int playerIndex, const int chip)const;
	void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const;
	void showPlayerSidePot(const int playerIndex, const int money)const;
	//void showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer)const;

	//void playerCheckRaiseFoldAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const;
	//void playerAllinFoldAction(const int nowPlayerIndex, const int allmoney)const;
	//void playerCallRaiseFoldAction(const int nowPlayerIndex, const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const;
	virtual void showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const;
	virtual void showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney)const;
	virtual void showPlayerCheckAction(const int nowPlayerIndex)const;
	virtual void showPlayerCallAction(const int nowPlayerIndex, const int callMoney)const;
	virtual void showPlayerFoldAction(const int nowPlayerIndex)const;

	void hidePlayerHandCards(const int playerIndex)const;
	void hidePlayerName(const int playerIndex)const;
	void hidePlayerChip(const int playerIndex)const;
	void hidePlayerActionMessage(const int playerIndex)const;
	void hidePlayerSidePot(const int playerIndex)const;
	//void hidePlayerNameCardsChip(const int playerIndex)const;
	//void hidePlayerAction(const int playerIndex)const;
	//void hidePlayer(const int playerIndex)const;

	virtual void hidePlayerRaiseAction(const int nowPlayerIndex)const;
	virtual void hidePlayerAllinAction(const int nowPlayerIndex)const;
	virtual void hidePlayerCheckAction(const int nowPlayerIndex)const;
	virtual void hidePlayerCallAction(const int nowPlayerIndex)const;
	virtual void hidePlayerFoldAction(const int nowPlayerIndex)const;

	//不需要在singleGame中实现
	virtual void showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const& handCards)const {};
	virtual void showPlayer1CardBackOnPlayer2(const int player1Index, const int player2Index)const {};

	virtual void showPlayerActionMessageWithTime(const int playerIndex)const {};
	virtual void stopAllClientTimer()const {};
};



