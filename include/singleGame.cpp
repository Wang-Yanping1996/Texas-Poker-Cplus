#include "game.h"
#include "ui_TexasPokerNew.h"


class singleGame : public game {
private:
	Ui_singleGameWindow *m_singleGameWindow;
public:
	singleGame(string gameID = "No ID",
		vector<player> players = vector<player>(maxNumOfPlayers),
		vector<card> commonCards = vector<card>(0),
		int pot = 0,
		vector<sidePot> sidePots = vector<sidePot>(0),	//���ֻ����������߳�
		int minBet = 0,
		cardHeap cardHeap = cardHeap(),
		gameRound round = ErrorRound,
		int nowPlayerIndex = -1,
		int endPlayerIndex = -1,
		int dealer = -1,
		set<int> calledPlayersIndex = {},
		Ui_singleGameWindow *singleGameWindow = nullptr
	) :game(gameID, players, commonCards, pot, sidePots, minBet, cardHeap, round, nowPlayerIndex, endPlayerIndex, dealer, calledPlayersIndex), m_singleGameWindow(singleGameWindow) {};

	void setGameWindow(Ui_singleGameWindow *singleGameWindow = nullptr) { this->m_singleGameWindow = singleGameWindow; };
	Ui_singleGameWindow* getGameWindow()const { return this->m_singleGameWindow; };

	virtual void renderGame();		//��Ⱦ��ǰ��Ϸ״̬��������Ҫ����
	virtual bool nowPlayerRender();
};
