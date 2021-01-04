#pragma once
//#ifndef TEXASPOKERSINGLEUI_H
//#define TEXASPOKERSINGLEUI_H

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

#include "player.h"
#include "game.h"
QT_BEGIN_NAMESPACE

QString getCardFileName(card const& c);
class Ui_playerWindow {
	int handCardPosition[2][2] = { { 10,70 },{ 100,70 } };
public:
	QLabel *playerHandCards[player::numOfHandCards];
	QPushButton *raise;
	QPushButton *check;
	QPushButton *call;		//和check相同位置，名字不一样
	QPushButton *fold;
	QSpinBox *raiseMoney;
	QLabel *playerName;
	QLabel *playerChip;

	QLabel *actionMessage;

	Ui_playerWindow(QWidget *centralWidget, int x, int y, int playerIndex) {
		for (int i = 0; i < player::numOfHandCards; ++i) {
			playerHandCards[i] = new QLabel(centralWidget);
			playerHandCards[i]->setObjectName(QStringLiteral("playerHandCard1"));
			playerHandCards[i]->setGeometry(QRect(handCardPosition[i][0] + x, handCardPosition[i][1] + y, 70, 100));
			playerHandCards[i]->setPixmap(QPixmap(QString::fromUtf8("image/0_00.jpg")));
			playerHandCards[i]->raise();
			playerHandCards[i]->hide();
		}
		raise = new QPushButton(centralWidget);
		raise->setObjectName(QStringLiteral("raise"));
		raise->setGeometry(QRect(70 + x, 40 + y, 41, 23));
		check = new QPushButton(centralWidget);
		check->setObjectName(QStringLiteral("check"));
		check->setGeometry(QRect(20 + x, 40 + y, 41, 23));
		call = new QPushButton(centralWidget);
		call->setObjectName(QStringLiteral("check"));
		call->setGeometry(QRect(20 + x, 40 + y, 41, 23));
		fold = new QPushButton(centralWidget);
		fold->setObjectName(QStringLiteral("fold"));
		fold->setGeometry(QRect(120 + x, 40 + y, 41, 23));
		raiseMoney = new QSpinBox(centralWidget);
		raiseMoney->setObjectName(QStringLiteral("spinBox"));
		raiseMoney->setGeometry(QRect(70 + x, 10 + y, 42, 22));
		
		QFont font1;
		font1.setPointSize(12);
		font1.setBold(true);
		font1.setWeight(75);
		playerName = new QLabel(centralWidget);
		playerName->setObjectName(QStringLiteral("label_4"));
		playerName->setGeometry(QRect(0 + x, 0 + y, 61, 31));
		playerName->setFont(font1);
		playerName->setLayoutDirection(Qt::LeftToRight);
		playerName->setAlignment(Qt::AlignCenter);

		playerChip = new QLabel(centralWidget);
		playerChip->setObjectName(QStringLiteral("chip"));
		playerChip->setGeometry(QRect(120 + x, 0 + y, 86, 41));
		playerChip->setFont(font1);
		playerChip->setLayoutDirection(Qt::LeftToRight);
		playerChip->setAlignment(Qt::AlignCenter);

		actionMessage = new QLabel(centralWidget);
		actionMessage->setObjectName(QStringLiteral("actionMessage"));
		actionMessage->setGeometry(QRect(30 + x, 30 + y, 121, 31));
		actionMessage->setFont(font1);
		actionMessage->setLayoutDirection(Qt::LeftToRight);
		actionMessage->setAlignment(Qt::AlignCenter);

		raise->raise();
		check->raise();
		fold->raise();
		call->raise();
		raiseMoney->raise();
		playerName->raise();
		playerChip->raise();
		actionMessage->raise();

		raise->hide();
		check->hide();
		fold->hide();
		call->hide();
		raiseMoney->hide();
		playerName->hide();
		playerChip->hide();
		actionMessage->hide();

		raise->setText(QApplication::translate("TexasPokerNewClass", "\345\212\240\346\263\250", Q_NULLPTR));
		check->setText(QApplication::translate("TexasPokerNewClass", "\347\234\213\347\211\214", Q_NULLPTR));
		fold->setText(QApplication::translate("TexasPokerNewClass", "\345\274\203\347\211\214", Q_NULLPTR));
		call->setText(QStringLiteral("跟注"));
		playerName->setText(QStringLiteral("玩家1"));
		playerChip->setText(QStringLiteral("筹码："));
		actionMessage->setText(QStringLiteral("玩家行为："));
	}

	void showHandCards(vector<card> const& c)const;
	void hideHandCards()const;
	void showPlayerName(string const& name)const;
	void hidePlayerName()const;
	void showPlayerChip(const int chip)const;
	void hidePlayerChip()const;
	void showPlayerActionMessage(QString const& actionMessage)const;
	void hidePlayerActionMessage()const;
	//玩家行为
	void showRaise(const int minRaiseMoney, const int maxRaiseMoney)const;
	void hideRaise()const;
	void showAllin(const int allMoney)const;
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

class Ui_singleGameWindow : public virUI {

public:
	int commonCardsPosition[game::maxNumOfCommonCards][2] = { { 300,260 },{ 390,260 },{ 480,260 },{ 590,260 },{ 700,260 } };
	int playerPosition[game::maxNumOfPlayers][2] = { { 470,430 },{ 750,430 },{ 940,190 },{ 750,0 },{ 470,0 },{ 170,0 },{ 0,190 },{ 170,430 } };

	game *m_game;

	Ui_playerWindow *players[game::maxNumOfPlayers];
	QLabel *commonCards[game::maxNumOfCommonCards];
	QLabel *flop;
	QLabel *turn;
	QLabel *river;
	QLabel *pot;
	QPushButton *begin;

	Ui_singleGameWindow(QWidget *centralWidget, game* g = nullptr) {
		this->m_game = g;

		for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
			commonCards[i_card] = new QLabel(centralWidget);
			commonCards[i_card]->setObjectName(QStringLiteral("playerHandCard1_3"));
			commonCards[i_card]->setGeometry(QRect(commonCardsPosition[i_card][0], commonCardsPosition[i_card][1], 70, 100));
			QString cardFileName = getCardFileName(card(Club, Three));
			commonCards[i_card]->setPixmap(QPixmap(cardFileName));
			commonCards[i_card]->raise();
			commonCards[i_card]->hide();
		}

		QFont font;
		font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
		font.setPointSize(12);
		font.setBold(true);
		font.setWeight(75);

		flop = new QLabel(centralWidget);
		flop->setObjectName(QStringLiteral("flop"));
		flop->setGeometry(QRect(390, 230, 71, 31));
		flop->setFont(font);
		flop->setTextFormat(Qt::AutoText);
		flop->setScaledContents(false);
		flop->setAlignment(Qt::AlignCenter);
		flop->raise();
		flop->hide();

		turn = new QLabel(centralWidget);
		turn->setObjectName(QStringLiteral("turn"));
		turn->setGeometry(QRect(590, 230, 71, 31));
		turn->setFont(font);
		turn->setTextFormat(Qt::AutoText);
		turn->setScaledContents(false);
		turn->setAlignment(Qt::AlignCenter);
		turn->raise();
		turn->hide();

		river = new QLabel(centralWidget);
		river->setObjectName(QStringLiteral("river"));
		river->setGeometry(QRect(700, 230, 71, 31));
		river->setFont(font);
		river->setTextFormat(Qt::AutoText);
		river->setScaledContents(false);
		river->setAlignment(Qt::AlignCenter);
		river->raise();
		river->hide();

		pot = new QLabel(centralWidget);
		pot->setObjectName(QStringLiteral("底池"));
		pot->setGeometry(QRect(480, 370, 70, 30));
		pot->setFont(font);
		pot->setTextFormat(Qt::AutoText);
		pot->setScaledContents(false);
		pot->setAlignment(Qt::AlignCenter);
		pot->raise();
		pot->hide();

		begin = new QPushButton(centralWidget);
		begin->setObjectName(QStringLiteral("begin"));
		begin->setGeometry(QRect(500, 300, 50, 50));
		begin->raise();
		begin->show();

		flop->setText(QApplication::translate("QtWidgetsApplication1Class", "\347\277\273\347\211\214", Q_NULLPTR));
		turn->setText(QApplication::translate("QtWidgetsApplication1Class", "\350\275\254\347\211\214", Q_NULLPTR));
		river->setText(QApplication::translate("QtWidgetsApplication1Class", "\346\262\263\347\211\214", Q_NULLPTR));
		pot->setText(QStringLiteral("底池：") + QString::number(10));
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			players[i_player] = new Ui_playerWindow(centralWidget, playerPosition[i_player][0], playerPosition[i_player][1], i_player);
		}
	}

	void showCommonCards(vector<card> const& commonCards)const;
	void hideCommonCards()const;

	void showRound(gameRound nowRound)const;
	void hideRound()const;
	void showPot(const int potNum)const;
	void hidePot()const;

	//玩家相关
	//void showPlayerAction(int playerIndex, player const& needShowPlayer)const;
	//void showPlayer(const int playerIndex, player const& needShowPlayer)const;
	void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const;
	void showPlayerName(const int playerIndex, string const& playerName)const;
	void showPlayerChip(const int playerIndex, const int chip)const;
	void showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer)const;
	void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const;

	void playerCheckRaiseFoldAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const;
	void playerAllinFoldAction(const int nowPlayerIndex, const int allmoney)const;
	void playerCallRaiseFoldAction(const int nowPlayerIndex, const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const;


	void hidePlayerHandCards(const int playerIndex)const;
	void hidePlayerName(const int playerIndex)const;
	void hidePlayerChip(const int playerIndex)const;
	void hidePlayerNameCardsChip(const int playerIndex)const;
	void hidePlayerActionMessage(const int playerIndex)const;
	void hidePlayerAction(const int playerIndex)const;
	void hidePlayer(const int playerIndex)const;
};

class Ui_TexasPokerNewClass
{
public:
	Ui_singleGameWindow *table;

	//QPushButton *begin;

	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *TexasPokerNewClass)
	{
		if (TexasPokerNewClass->objectName().isEmpty())
			TexasPokerNewClass->setObjectName(QStringLiteral("TexasPokerNewClass"));
		TexasPokerNewClass->resize(1118, 668);
		menuBar = new QMenuBar(TexasPokerNewClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		TexasPokerNewClass->setMenuBar(menuBar);
		mainToolBar = new QToolBar(TexasPokerNewClass);
		mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		TexasPokerNewClass->addToolBar(mainToolBar);
		centralWidget = new QWidget(TexasPokerNewClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		TexasPokerNewClass->setCentralWidget(centralWidget);
		statusBar = new QStatusBar(TexasPokerNewClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		TexasPokerNewClass->setStatusBar(statusBar);

		//这里添加
		//begin = new QPushButton(TexasPokerNewClass);
		//begin->setObjectName(QStringLiteral("begin"));
		//begin->setGeometry(QRect(500, 300, 50, 50));
		//begin->raise();
		//begin->show();

		table = new Ui_singleGameWindow(centralWidget);

		retranslateUi(TexasPokerNewClass);

		QMetaObject::connectSlotsByName(TexasPokerNewClass);
	} // setupUi

	void retranslateUi(QMainWindow *TexasPokerNewClass)
	{
		TexasPokerNewClass->setWindowTitle(QApplication::translate("TexasPokerNewClass", "TexasPokerNew", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class TexasPokerNewClass : public Ui_TexasPokerNewClass {};
} // namespace Ui

QT_END_NAMESPACE

//#endif 
