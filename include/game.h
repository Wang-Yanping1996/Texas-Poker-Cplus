#pragma once
#include "cardHeap.h"
#include "player.h"
#include <string>
#include <set>
#include <map>

enum gameRound { ErrorRound = -1, Ready, Start, PreFlop, Flop, Turn, River, End };

//接口类
class virUI {
public:
	virtual void showCommonCards(vector<card> const& commonCards)const = 0;
	virtual void hideCommonCards()const = 0;

	virtual void showRound(gameRound nowRound)const = 0;
	virtual void hideRound()const = 0;
	virtual void showPot(const int potNum)const = 0;
	virtual void hidePot()const = 0;

	//玩家相关
	//void showPlayerAction(int playerIndex, player const& needShowPlayer)const;
	//virtual void showPlayer(const int playerIndex, player const& needShowPlayer)const = 0;
	virtual void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const = 0;
	virtual void showPlayerName(const int playerIndex, string const& playerName)const = 0;
	virtual void showPlayerChip(const int playerIndex, const int chip)const = 0;
	virtual void showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer)const = 0;
	virtual void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const = 0;

	virtual void playerCheckRaiseFoldAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const = 0;
	virtual void playerAllinFoldAction(const int nowPlayerIndexconst, int allmoney)const = 0;
	virtual void playerCallRaiseFoldAction(const int nowPlayerIndex, const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const = 0;

	virtual void hidePlayerHandCards(const int playerIndex)const = 0;
	virtual void hidePlayerName(const int playerIndex)const = 0;
	virtual void hidePlayerChip(const int playerIndex)const = 0;
	virtual void hidePlayerNameCardsChip(const int playerIndex)const = 0;
	virtual void hidePlayerActionMessage(const int playerIndex)const = 0;
	virtual void hidePlayerAction(const int playerIndex)const = 0;
	virtual void hidePlayer(const int playerIndex)const = 0;
};

//边池类
class sidePot {
private:
	int m_sidePotMoney;
	vector<int> m_sidePotParticipateIndex;
public:
	sidePot(int sidePotMoney = 0, vector<int> const& sidePotParticipateIndex = vector<int>(0)) : m_sidePotMoney(sidePotMoney), m_sidePotParticipateIndex(sidePotParticipateIndex) {};
	int getSidePotMoney()const { return this->m_sidePotMoney; };
	vector<int>& getSidePotParticipatIndex() { return this->m_sidePotParticipateIndex; };
	vector<int> const& getSidePotParticipatIndex()const { return this->m_sidePotParticipateIndex; };
	
	void setSidePotMoney(const int money) { this->m_sidePotMoney = money; };
	void insertParticipate(const int participateIndex) { this->m_sidePotParticipateIndex.push_back(participateIndex); };
};

class game {
public:
	static const int maxNumOfPlayers = 8;			//最多多少人
	static const int maxNumOfCommonCards = 5;
	static const int smallBind = 1;
	static const int bigBind = 2;
	int combineMap[21][5] = { { 0,1,2,3,4 },		//排列组合表，感觉应该换个位置
						{ 0,1,2,3,5 },
						{ 0,1,2,3,6 },
						{ 0,1,2,4,5 },
						{ 0,1,2,4,6 },
						{ 0,1,2,5,6 },
						{ 0,1,3,4,5 },
						{ 0,1,3,4,6 },
						{ 0,1,3,5,6 },
						{ 0,1,4,5,6 },
						{ 0,2,3,4,5 },
						{ 0,2,3,4,6 },
						{ 0,2,3,5,6 },
						{ 0,2,4,5,6 },
						{ 0,3,4,5,6 },
						{ 1,2,3,4,5 },
						{ 1,2,3,4,6 },
						{ 1,2,3,5,6 },
						{ 1,2,4,5,6 },
						{ 1,3,4,5,6 },
						{ 2,3,4,5,6 }
						};
private:
	string m_gameID;				//游戏ID
	vector<player> m_players;		//游戏中的玩家
	vector<card> m_commonCards;		//公共牌
	int m_pot;						//底池
	vector<sidePot> m_sidePots;	//边池 分层
	int m_minBet;					//本轮最小下注
	cardHeap m_cardHeap;			//牌堆
	gameRound m_round;				//回合
	int m_nowPlayerIndex;			//本轮当前玩家序号
	int m_endPlayerIndex;			//本轮结束玩家序号
	int m_dealer;					//dealer是谁
	set<int> m_calledPlayersIndex;	//没有fold的玩家序号集合
	virUI* m_ui;
public:
	game(string gameID = "No ID",
		vector<player> players = vector<player>(maxNumOfPlayers),
		vector<card> commonCards = vector<card>(0),
		int pot = 0,
		vector<sidePot> sidePots = vector<sidePot>(0),	//最多只有玩家数个边池
		int minBet = 0,
		cardHeap cardHeap = cardHeap(),
		gameRound round = ErrorRound,
		int nowPlayerIndex = -1,
		int endPlayerIndex = -1,
		int dealer = rand()%maxNumOfPlayers,
		set<int> calledPlayersIndex = {},
		virUI* ui = nullptr
	) :m_gameID(gameID),
		m_players(players),
		m_commonCards(commonCards),
		m_pot(pot),
		m_sidePots(sidePots),
		m_minBet(minBet),
		m_cardHeap(cardHeap),
		m_round(round),
		m_nowPlayerIndex(nowPlayerIndex),
		m_endPlayerIndex(endPlayerIndex),
		m_dealer(dealer),
		m_calledPlayersIndex(calledPlayersIndex),
		m_ui(ui){
		m_commonCards.reserve(maxNumOfCommonCards);	//保留空间
		m_sidePots.reserve(maxNumOfPlayers);			//最多玩家数个边池
	};
	~game() = default;
	
	string getGameID() const { return this->m_gameID; };
	vector<player> const& getPlayers()const { return this->m_players; };
	player& getPlayer(int playerIndex) { return this->m_players[playerIndex]; };
	player const& getPlayer(int playerIndex)const { return this->m_players[playerIndex]; };
	int getNextCalledPlayerIndex(const int nowPlayerIndex)const;	//输入可以弃牌，输出还没弃牌的，重写过了
	int getPreCalledPlayerIndex(const int nowPlayerIndex)const;	//输入可以弃牌，输出还没弃牌的，补写

	vector<card> const& getCommonCards()const { return this->m_commonCards; };
	int getPot()const { return this->m_pot; };
	int getMinBet()const { return this->m_minBet; };
	gameRound getGameRound()const { return this->m_round; };
	player const& getNowPlayer()const { return this->m_players[this->getNowPlayerIndex()]; };
	int getNowPlayerIndex()const { return this->m_nowPlayerIndex; };
	int getEndPlayerIndex()const { return this->m_endPlayerIndex; };
	int getRoundBeginPlayerIndex()const;
	int getRoundEndPlayerIndex()const;
	int getDealer()const { return this->m_dealer; };
	set<int>& getCalledPlayersIndex() { return this->m_calledPlayersIndex; };
	set<int> const& getCalledPlayersIndex()const { return this->m_calledPlayersIndex; };
	virUI* getVirUIPoint()const { return this->m_ui; };

	void setGameID(string gameID) { this->m_gameID = gameID; };

	bool addPlayer(player const& p);
	int getNumOfPlayers()const;
	player& getPlayer(string playerID)const;
	bool initPlayersState();

	int getNumOfCommonCards()const { return this->m_commonCards.size(); };
	void clearCommonCards() { this->m_commonCards.clear(); };
	gameRound updateGameRound();

	void addToPot();
	void updatePots();		//更新底池与边池
	void clearPot() { this->m_pot = 0; };
	int addNewSidePot(const int money) { this->m_sidePots.push_back(sidePot(0)); return (int)this->m_sidePots.size() - 1; };
	void addPlayerToLastSidePot(const int sidePotIndex, const int playerAddedIndex) { this->m_sidePots[sidePotIndex].insertParticipate(playerAddedIndex); };
	void clearSidePot() { this->m_sidePots.clear(); };

	void setMinBet(int minBet) { this->m_minBet = minBet; };
	void setVirUIPoint(virUI* ui = nullptr) { this->m_ui = ui; };

	void shuffleCardHeap() { this->m_cardHeap.shuffle(); };
	void sendCardsToPlayers();
	void sendCardsToTable();

	void updateEndPlayerIndex();
	void updateNowPlayerIndex();
	void setNowPlayerIndex(const int nowPlayerIndex) { this->m_nowPlayerIndex = nowPlayerIndex; };
	void setEndPlayerIndex(const int endPlayerIndex) { this->m_endPlayerIndex = endPlayerIndex; };
	void updateDealer() {
		int nextDealer = this->getNextCalledPlayerIndex(this->m_dealer);
		this->m_dealer = nextDealer;	//相等怎么办？要不要判断？

		//似乎没用了后面的
		//int nextDealerIndex = this->m_dealer;
		//do {
		//	nextDealerIndex++;
		//	if (nextDealerIndex >= maxNumOfPlayers)
		//		nextDealerIndex -= maxNumOfPlayers;
		//	if (this->m_players[nextDealerIndex].getPlayerType() == playerType::OnSitePlayer) {	//与getRoundBeginPlayerIndex不同之处在于判别条件，在更新calledPlayer以后应该两者一样
		//		this->m_dealer = nextDealerIndex;
		//		return;		//找到了
		//	}
		//} while (nextDealerIndex != this->m_dealer);	//一圈了还没找到
		//this->m_dealer = -1;
		//return;
		//this->m_dealer = this->getNextPlayerIndex(this->m_dealer);		//updateDealer之前应该加入了calledPlayer，直接调用这个应该就行，并不行，上局dealer可能不在本局游戏中
	};

	void clearCalledPlayersIndex() { this->m_calledPlayersIndex.clear(); };

	void nowPlayerRaise(const int raiseTo);
	void nowPlayerCall();
	void nowPlayerCheck();
	void nowPlayerFold();

	void afterPlayerAction();
	void nextRound();				//进行下一轮

	//virtual void renderNowPlayerActionUI() = 0;	//渲染当前玩家的行动界面，可能需要重载
	//virtual void hideNowPlayerAction();	//当前玩家行动完毕，关闭其行动界面
	//virtual void renderGame();			//渲染当前游戏状态，可能需要重载
	//virtual void nowPlayerActionComplete();
	//virtual void finishThisRound();
	//virtual bool nowPlayerRender();		//渲染当前玩家的行动界面，可能需要重载

	void renderGame();			//渲染当前游戏状态，可能需要重载
	void hideNowPlayerAction();	//当前玩家行动完毕，关闭其行动界面
	void showNowPlayerActionMessage(string const& actionMessage);
	void nowPlayerActionComplete();
	void finishThisRound();
	bool nowPlayerRender();		//渲染当前玩家的行动界面，可能需要重载


	void settle();			//结算,只剩一人或河牌以后计算
	void calCardTypeAndPointForAll();							//为所有在场的人计算牌型牌点
	void calCardTypeAndPointForPlayer(const int playerIndex);	//为单一玩家计算牌型牌点

	void playerWin(int winnerIndex);				//玩家x赢了多少钱
	void playerLose(int loserIndex);				//玩家x输了
	vector<int> getMaxPlayersIndex();										//获取手牌最大玩家的序号
	vector<int> getMaxPlayersIndex(vector<int> const& curPlayerIndexSet);	//获取输入数组中手牌最大玩家的序号

	//
	bool begin();					//开始游戏
	//bool playersAction();			//玩家行动
	//bool playerAction(const int playerIndex);	//玩家N行动
	//int  isEnd();					//游戏是否结束，只剩一人
};



