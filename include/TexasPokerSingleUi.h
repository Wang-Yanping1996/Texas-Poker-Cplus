
#ifndef TEXASPOKERSINGLEUI_H
#define TEXASPOKERSINGLEUI_H

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

class Ui_playerWindow {
	int handCardPosition[2][2] = { { 10,70 },{ 100,70 } };
public:
	QLabel *playerHandCards[player::numOfHandCards];
	QPushButton *raise;
	QPushButton *check;
	QPushButton *fold;
	QSpinBox *raiseMoney;
	QLabel *playerName;

	Ui_playerWindow(QWidget *centralWidget, int x, int y, int playerIndex) {
		for (int i = 0; i < player::numOfHandCards; ++i) {
			playerHandCards[i] = new QLabel(centralWidget);
			playerHandCards[i]->setObjectName(QStringLiteral("playerHandCard1"));
			playerHandCards[i]->setGeometry(QRect(handCardPosition[i][0] + x, handCardPosition[i][1] + y, 70, 100));
			playerHandCards[i]->setPixmap(QPixmap(QString::fromUtf8("image/0_00.jpg")));
			playerHandCards[i]->raise();
		}
		raise = new QPushButton(centralWidget);
		raise->setObjectName(QStringLiteral("raise"));
		raise->setGeometry(QRect(70 + x, 40 + y, 41, 23));
		check = new QPushButton(centralWidget);
		check->setObjectName(QStringLiteral("check"));
		check->setGeometry(QRect(20 + x, 40 + y, 41, 23));
		fold = new QPushButton(centralWidget);
		fold->setObjectName(QStringLiteral("fold"));
		fold->setGeometry(QRect(120 + x, 40 + y, 41, 23));
		raiseMoney = new QSpinBox(centralWidget);
		raiseMoney->setObjectName(QStringLiteral("spinBox"));
		raiseMoney->setGeometry(QRect(70 + x, 10 + y, 42, 22));

		playerName = new QLabel(centralWidget);
		playerName->setObjectName(QStringLiteral("label_4"));
		playerName->setGeometry(QRect(0 + x, 0 + y, 61, 31));
		QFont font1;
		font1.setPointSize(12);
		font1.setBold(true);
		font1.setWeight(75);
		playerName->setFont(font1);
		playerName->setLayoutDirection(Qt::LeftToRight);
		playerName->setAlignment(Qt::AlignCenter);

		raise->raise();
		check->raise();
		fold->raise();
		raiseMoney->raise();
		playerName->raise();

		raise->setText(QApplication::translate("TexasPokerNewClass", "\345\212\240\346\263\250", Q_NULLPTR));
		check->setText(QApplication::translate("TexasPokerNewClass", "\347\234\213\347\211\214", Q_NULLPTR));
		fold->setText(QApplication::translate("TexasPokerNewClass", "\345\274\203\347\211\214", Q_NULLPTR));
		playerName->setText(QStringLiteral("玩家1"));
	}
};

class Ui_singleGameWindow {
public:
	int commonCardsPosition[game::maxNumOfCommonCards][2] = { { 300,260 },{ 390,260 },{ 480,260 },{ 590,260 },{ 700,260 } };
	int playerPosition[game::maxNumOfPlayers][2] = { { 470,430 },{ 750,430 },{ 940,190 },{ 750,0 },{ 470,0 },{ 170,0 },{ 0,190 },{ 170,430 } };



	Ui_playerWindow *players[game::maxNumOfPlayers];
	QLabel *commonCards[game::maxNumOfCommonCards];
	QLabel *flop;
	QLabel *turn;
	QLabel *river;
	Ui_singleGameWindow(QWidget *centralWidget) {
		for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
			commonCards[i_card] = new QLabel(centralWidget);
			commonCards[i_card]->setObjectName(QStringLiteral("playerHandCard1_3"));
			commonCards[i_card]->setGeometry(QRect(commonCardsPosition[i_card][0], commonCardsPosition[i_card][1], 70, 100));
			commonCards[i_card]->setPixmap(QPixmap(QString::fromUtf8("image/0_00.jpg")));
			commonCards[i_card]->raise();
		}

		QFont font;
		font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
		font.setPointSize(12);
		font.setBold(true);
		font.setWeight(75);

		flop = new QLabel(centralWidget);
		flop->setObjectName(QStringLiteral("label"));
		flop->setGeometry(QRect(390, 230, 71, 31));
		flop->setFont(font);
		flop->setTextFormat(Qt::AutoText);
		flop->setScaledContents(false);
		flop->setAlignment(Qt::AlignCenter);
		flop->raise();

		turn = new QLabel(centralWidget);
		turn->setObjectName(QStringLiteral("label"));
		turn->setGeometry(QRect(590, 230, 71, 31));
		turn->setFont(font);
		turn->setTextFormat(Qt::AutoText);
		turn->setScaledContents(false);
		turn->setAlignment(Qt::AlignCenter);
		turn->raise();

		river = new QLabel(centralWidget);
		river->setObjectName(QStringLiteral("label"));
		river->setGeometry(QRect(700, 230, 71, 31));
		river->setFont(font);
		river->setTextFormat(Qt::AutoText);
		river->setScaledContents(false);
		river->setAlignment(Qt::AlignCenter);
		river->raise();

		flop->setText(QApplication::translate("QtWidgetsApplication1Class", "\347\277\273\347\211\214", Q_NULLPTR));
		turn->setText(QApplication::translate("QtWidgetsApplication1Class", "\350\275\254\347\211\214", Q_NULLPTR));
		river->setText(QApplication::translate("QtWidgetsApplication1Class", "\346\262\263\347\211\214", Q_NULLPTR));

		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			players[i_player] = new Ui_playerWindow(centralWidget, playerPosition[i_player][0], playerPosition[i_player][1], i_player);
		}
	}
};

class Ui_TexasPokerNewClass
{
public:
	Ui_singleGameWindow *table;

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

#endif 
