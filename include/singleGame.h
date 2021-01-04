#pragma once
#include "game.h"
//#include "ui_TexasPokerNew.h"
#include "TexasPokerSingleUi.h"

class singleGame : public game {
private:
	Ui_singleGameWindow *m_singleGameWindow;
public:
	singleGame(string gameID = "No ID",
		vector<player> players = vector<player>(maxNumOfPlayers),
		vector<card> commonCards = vector<card>(0),
		int pot = 0,
		vector<sidePot> sidePots = vector<sidePot>(0),	//最多只有玩家数个边池
		int minBet = 0,
		cardHeap cardHeap = cardHeap(),
		gameRound round = ErrorRound,
		int nowPlayerIndex = -1,
		int endPlayerIndex = -1,
		int dealer = rand()&maxNumOfPlayers,
		set<int> calledPlayersIndex = {},
		Ui_singleGameWindow *singleGameWindow = nullptr
	) :game(gameID, players, commonCards, pot, sidePots, minBet, cardHeap, round, nowPlayerIndex, endPlayerIndex, dealer, calledPlayersIndex), m_singleGameWindow(singleGameWindow) {};

	void setGameWindow(Ui_singleGameWindow *singleGameWindow = nullptr) { this->m_singleGameWindow = singleGameWindow; };
	Ui_singleGameWindow* getGameWindow()const { return this->m_singleGameWindow; };

	 void renderGame();		//渲染当前游戏状态，可能需要重载
	 void hideNowPlayerAction();	
	 void showNowPlayerActionMessage(QString const& actionMessage);
	 void nowPlayerActionComplete();
	 void finishThisRound();
	 bool nowPlayerRender();
};

