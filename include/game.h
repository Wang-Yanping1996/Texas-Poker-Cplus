#pragma once
#include "cardHeap.h"
#include "player.h"
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <QtGui/qstandarditemmodel.h>

enum gameRound { ErrorRound = -1, Ready, Start, PreFlop, Flop, Turn, River, End };

//接口类,包含了UI应该实现的方法，也就是各部件的show和hide
class virUI {
public:
	virtual void showDealer()const = 0;

	virtual void showCommonCards(vector<card> const& commonCards)const = 0;
	virtual void hideCommonCards()const = 0;
	virtual void showRound(gameRound nowRound)const = 0;
	virtual void hideRound()const = 0;
	virtual void showPot(const int potNum)const = 0;
	virtual void hidePot()const = 0;
	virtual void showBegin(const int playerIndex)const = 0;
	virtual void hideBegin(const int playerIndex)const = 0;

	//玩家相关
	virtual void showPlayerHandCards(const int playerIndex, vector<card> const& handCards)const = 0;
	virtual void showPlayerName(const int playerIndex, string const& playerName)const = 0;
	virtual void showPlayerChip(const int playerIndex, const int chip)const = 0;
	virtual void showPlayerActionMessage(const int playerIndex, string const& actionMessage)const = 0;
	virtual void showPlayerSidePot(const int playerIndex, const int money)const = 0;
	//virtual void showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer)const = 0;

	//virtual void playerCheckRaiseFoldAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const = 0;
	//virtual void playerAllinFoldAction(const int nowPlayerIndexconst, int allmoney)const = 0;
	//virtual void playerCallRaiseFoldAction(const int nowPlayerIndex, const int callMoney, const int minRaiseMoney, const int maxRaiseMoney)const = 0;
	virtual void showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const = 0;
	virtual void showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney)const = 0;	//不知道有没有用，allin可以在game中实现
	virtual void showPlayerCheckAction(const int nowPlayerIndex)const = 0;
	virtual void showPlayerCallAction(const int nowPlayerIndex, const int callMoney)const = 0;
	virtual void showPlayerFoldAction(const int nowPlayerIndex)const = 0;

	virtual void hidePlayerHandCards(const int playerIndex)const = 0;
	virtual void hidePlayerName(const int playerIndex)const = 0;
	virtual void hidePlayerChip(const int playerIndex)const = 0;
	virtual void hidePlayerActionMessage(const int playerIndex)const = 0;
	virtual void hidePlayerSidePot(const int playerIndex)const = 0;

	//virtual void hidePlayerNameCardsChip(const int playerIndex)const = 0;
	//virtual void hidePlayerAction(const int playerIndex)const = 0;
	//virtual void hidePlayer(const int playerIndex)const = 0;

	virtual void hidePlayerRaiseAction(const int nowPlayerIndex)const = 0;
	virtual void hidePlayerAllinAction(const int nowPlayerIndex)const = 0;							//不知道有没有用
	virtual void hidePlayerCheckAction(const int nowPlayerIndex)const = 0;
	virtual void hidePlayerCallAction(const int nowPlayerIndex)const = 0;
	virtual void hidePlayerFoldAction(const int nowPlayerIndex)const = 0;

	//后添加的show牌方法，在玩家2的client上show玩家1的牌
	virtual void showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const& handCards)const = 0;
	virtual void showPlayer1CardBackOnPlayer2(const int player1Index, const int player2Index)const;
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
	static const int initChip = 200;				//初始筹码
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
	vector<sidePot> m_sidePots;		//边池 分层
	int m_minBet;					//本轮最小下注
	cardHeap m_cardHeap;			//牌堆
	gameRound m_round;				//回合
	int m_nowPlayerIndex;			//本轮当前玩家序号
	int m_endPlayerIndex;			//本轮结束玩家序号
	int m_dealer;					//dealer是谁
	set<int> m_calledPlayersIndex;	//没有fold的玩家序号集合
	int m_readyNumOfPlayer;			//准备好的人数
	bool m_hasStarted;				//是否游戏中
	virUI* m_ui;
	//计分表添加
	unordered_map<string, int> m_macAddressToScoreChartIndex;
	QStandardItemModel* m_scoreChart;	//各玩家得分记录
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
		int readyNumOfPlayer = 0,
		bool hasStarted = false,
		virUI* ui = nullptr,
		unordered_map<string, int> macAddressToScoreChartIndex = {},
		QStandardItemModel* scoreChart = new QStandardItemModel()
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
		m_readyNumOfPlayer(readyNumOfPlayer),
		m_hasStarted(hasStarted),
		m_ui(ui),
		m_macAddressToScoreChartIndex(macAddressToScoreChartIndex),
		m_scoreChart(scoreChart)
	{
		m_commonCards.reserve(maxNumOfCommonCards);				//保留空间
		m_sidePots.reserve(maxNumOfPlayers);					//最多玩家数个边池

		m_scoreChart->setHorizontalHeaderLabels({ QStringLiteral("玩家名"), QStringLiteral("总带入"), QStringLiteral("当前总筹码"), QStringLiteral("场上筹码"), QStringLiteral("净胜") });
		m_macAddressToScoreChartIndex.clear();
	};
	~game() = default;
	
	//get玩家
	string getGameID() const { return this->m_gameID; };
	string getPlayerMacAddress(const int playerIndex) { return this->m_players[playerIndex].getMacAddress(); };
	unordered_map<string, int>& getMacAddressToScoreChartIndex() { return this->m_macAddressToScoreChartIndex; };
	QStandardItemModel* getScoreChart() { return this->m_scoreChart; };	
	vector<player> const& getPlayers()const { return this->m_players; };
	player& getPlayer(int playerIndex) { return this->m_players[playerIndex]; };
	player const& getPlayer(int playerIndex)const { return this->m_players[playerIndex]; };
	int getNextCalledPlayerIndex(const int nowPlayerIndex)const;	//输入可以弃牌，输出还没弃牌的，重写过了
	int getPreCalledPlayerIndex(const int nowPlayerIndex)const;		//输入可以弃牌，输出还没弃牌的，补写
	void setGameHasStarted(const bool hasStarted) { this->m_hasStarted = hasStarted; };
	bool getGameHasStarted()const { return this->m_hasStarted; };
	//get game
	int getNumOfPlayers()const;
	player& getPlayer(string playerID)const;
	vector<card> const& getCommonCards()const { return this->m_commonCards; };
	int getNumOfCommonCards()const { return this->m_commonCards.size(); };
	int getPot()const { return this->m_pot; };
	int getMinBet()const { return this->m_minBet; };
	gameRound getGameRound()const { return this->m_round; };
	player const& getNowPlayer()const { return this->m_players[this->getNowPlayerIndex()]; };
	int getNowPlayerIndex()const { return this->m_nowPlayerIndex; };
	int getEndPlayerIndex()const { return this->m_endPlayerIndex; };
	int getRoundBeginPlayerIndex()const;
	int getRoundEndPlayerIndex()const;
	int getDealer()const { return this->m_dealer; };
	int addNewPlayer(string const& playerName);	//随机添加一个玩家，返回添加玩家的位置
	set<int>& getCalledPlayersIndex() { return this->m_calledPlayersIndex; };
	set<int> const& getCalledPlayersIndex()const { return this->m_calledPlayersIndex; };
	virUI* getVirUIPoint()const { return this->m_ui; };
	int getNumOfReadyPlayer()const { return this->m_readyNumOfPlayer; };
	void addNumOfReadyPlayer() { this->m_readyNumOfPlayer++; };
	void clearNumOfReadyPlayer() { this->m_readyNumOfPlayer = 0; };

	gameRound updateGameRound();
	bool addPlayer(player const& p);
	bool initPlayersState();	
	void addToPot(const int addMoney) { this->m_pot += addMoney; };		//把money加入底池
	
	void setGameID(string gameID) { this->m_gameID = gameID; };
	void setPlayerName(const int playerIndex, string const& playerName) { this->m_players[playerIndex].setName(playerName); };
	void setPlayerMacAddress(const int playerIndex, string const& macAddress) { this->m_players[playerIndex].setMacAddress(macAddress); };
	void clearCommonCards() { this->m_commonCards.clear(); };
	void updatePots();		//更新底池与边池
	void clearPot() { this->m_pot = 0; };
	int addNewSidePot(const int money) { this->m_sidePots.push_back(sidePot(money)); return (int)this->m_sidePots.size() - 1; };
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
	void updateDealer() { int nextDealer = this->getNextCalledPlayerIndex(this->m_dealer);	this->m_dealer = nextDealer; };	//相等怎么办？要不要判断？
	void showDealer() { this->m_ui->showDealer(); };
	void clearCalledPlayersIndex() { this->m_calledPlayersIndex.clear(); };

	//五个必须实现的函数
	void nowPlayerRaise(const int raiseTo);
	void nowPlayerAllin();
	void nowPlayerCall();
	void nowPlayerCheck();
	void nowPlayerFold();

	void playerEscape(const int playerIndex);	//逃跑玩家 按fold处理

	void afterPlayerAction();

	void nextRound();				//进行下一轮

	void renderGame();				//渲染当前游戏状态，可能需要重载
	//void showNowPlayerActionMessage(string const& actionMessage);
	void nowPlayerActionComplete();
	void finishThisRound();
	bool nowPlayerRender();			//渲染当前玩家的行动界面，可能需要重载

	void settle();					//结算,只剩一人或河牌以后计算
	void calCardTypeAndPointForAll();														//为所有在场的人计算牌型牌点
	cardTypeAndPoint calCardTypeAndPointForPlayer(const int playerIndex);				//为单一玩家计算牌型牌点

	void playerWin(int winnerIndex);//玩家x赢了多少钱
	void playerLose(int loserIndex);//玩家x输了
	vector<int> getMaxPlayersIndex();										//获取手牌最大玩家的序号
	vector<int> getMaxPlayersIndex(vector<int> const& curPlayerIndexSet);	//获取输入数组中手牌最大玩家的序号

	bool begin();					//开始游戏

	//bool playersAction();			//玩家行动
	//bool playerAction(const int playerIndex);	//玩家N行动
	//int  isEnd();					//游戏是否结束，只剩一人

	//ui相关
	//player show
	void showPlayerChip(const int playerIndex);
	void showPlayerName(const int playerIndex);
	void showPlayerHandCards(const int playerIndex);
	void showPlayerActionMessage(const int playerIndex);
	void showPlayerSidePot(const int playerIndex, const int money);
	void showPlayerMessage(const int playerIndex, string const& message);
	void showPlayerRaiseAction(const int playerIndex);
	void showPlayerAllinAction(const int playerIndex);		//allin加上了
	void showPlayerCheckAction(const int playerIndex);
	void showPlayerCallAction(const int playerIndex);
	void showPlayerFoldAction(const int playerIndex);	
	void showNowPlayerChip() { this->showPlayerChip(this->m_nowPlayerIndex); };
	void showNowPlayerName() { this->showPlayerName(this->m_nowPlayerIndex); };
	void showNowPlayerHandCards() { this->showPlayerHandCards(this->m_nowPlayerIndex); };
	void showNowPlayerActionMessage() { this->showPlayerActionMessage(this->m_nowPlayerIndex); };
	void showNowPlayerRaiseAction() { this->showPlayerRaiseAction(this->m_nowPlayerIndex); };
	void showNowPlayerCheckAction() { this->showPlayerCheckAction(this->m_nowPlayerIndex); };
	void showNowPlayerCallAction() { this->showPlayerCallAction(this->m_nowPlayerIndex); };
	void showNowPlayerFoldAction() { this->showPlayerFoldAction(this->m_nowPlayerIndex); };
	//player hide
	void hidePlayerChip(const int playerIndex) { this->m_ui->hidePlayerChip(playerIndex); };
	void hidePlayerName(const int playerIndex) { this->m_ui->hidePlayerName(playerIndex); };
	void hidePlayerHandCards(const int playerIndex) { this->m_ui->hidePlayerHandCards(playerIndex); };
	void hidePlayerActionMessage(const int playerIndex) { this->m_ui->hidePlayerActionMessage(playerIndex); };
	void hidePlayerSidePot(const int playerIndex) { this->m_ui->hidePlayerSidePot(playerIndex); };
	void hideAllPlayerSidePot() { for (int i = 0; i < maxNumOfPlayers; ++i) this->m_ui->hidePlayerSidePot(i); };
	void hidePlayerRaiseAction(const int playerIndex) { this->m_ui->hidePlayerRaiseAction(playerIndex); };
	void hidePlayerAllinAction(const int playerIndex) { this->m_ui->hidePlayerAllinAction(playerIndex); };
	void hidePlayerCheckAction(const int playerIndex) { this->m_ui->hidePlayerCheckAction(playerIndex); };
	void hidePlayerCallAction(const int playerIndex) { this->m_ui->hidePlayerCallAction(playerIndex); };
	void hidePlayerFoldAction(const int playerIndex) { this->m_ui->hidePlayerFoldAction(playerIndex); };
	void hideNowPlayerChip() { this->hidePlayerChip(this->m_nowPlayerIndex); };
	void hideNowPlayerName() { this->hidePlayerName(this->m_nowPlayerIndex); };
	void hideNowPlayerHandCards() { this->hidePlayerHandCards(this->m_nowPlayerIndex); };
	void hideNowPlayerActionMessage() { this->hidePlayerActionMessage(this->m_nowPlayerIndex); };
	void hideNowPlayerRaiseAction() { this->hidePlayerRaiseAction(this->m_nowPlayerIndex); };
	void hideNowPlayerCheckAction() { this->hidePlayerCheckAction(this->m_nowPlayerIndex); };
	void hideNowPlayerCallAction() { this->hidePlayerCallAction(this->m_nowPlayerIndex); };
	void hideNowPlayerFoldAction() { this->hidePlayerFoldAction(this->m_nowPlayerIndex); }; 
	//player组合行为
	void showPlayerNameCardsChip(const int playerIndex);
	void showPlayerCheckRaiseFoldAction(const int playerIndex);
	void showPlayerAllinFoldAction(const int playerIndex);
	void showPlayerCallRaiseFoldAction(const int playerIndex);

	void hidePlayerAllAction(const int playerIndex);
	void hidePlayer(const int playerIndex);
	void hideNowPlayerAllAction();							//当前玩家行动完毕，关闭其行动界面

	void showPlayer1CardsOnPlayer2(const int player1Index, const int player2Index);
	void showOthersCardBackOnPlayerIndex(const int playerIndex);	//在场的人需要show卡背，该函数只应在begin时调用
	void showOthersCardBackOnAllClient();							//旁观者也要show卡背

	//game相关
	void showCommonCards() { this->m_ui->showCommonCards(this->m_commonCards); };
	void showRound() { this->m_ui->showRound(this->m_round); };
	void showPot() { this->m_ui->showPot(this->m_pot); };
	void hideCommonCards() { this->m_ui->hideCommonCards(); };
	void hideRound() { this->m_ui->hideRound(); };
	void hidePot() { this->m_ui->hidePot(); };
	void showBegin(const int playerIndex) { this->m_ui->showBegin(playerIndex); };
	void hideBegin(const int playerIndex) { this->m_ui->hideBegin(playerIndex); };
	void showAllBegin();
	void hideAllBegin();

	void showGameEnd();
	void updatePlayerScore(const int playerIndex);				//正常更新计分表
	void updateEscapedPlayerScore(const int playerIndex);		//更新逃跑玩家的计分表
};

