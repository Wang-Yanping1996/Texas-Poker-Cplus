#pragma once
#include "cardHeap.h"
#include "player.h"
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <QtGui/qstandarditemmodel.h>

enum gameRound { ErrorRound = -1, Ready, Start, PreFlop, Flop, Turn, River, End };

//�ӿ���,������UIӦ��ʵ�ֵķ�����Ҳ���Ǹ�������show��hide
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

	//������
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
	virtual void showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney)const = 0;	//��֪����û���ã�allin������game��ʵ��
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
	virtual void hidePlayerAllinAction(const int nowPlayerIndex)const = 0;							//��֪����û����
	virtual void hidePlayerCheckAction(const int nowPlayerIndex)const = 0;
	virtual void hidePlayerCallAction(const int nowPlayerIndex)const = 0;
	virtual void hidePlayerFoldAction(const int nowPlayerIndex)const = 0;

	//����ӵ�show�Ʒ����������2��client��show���1����
	virtual void showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const& handCards)const = 0;
	virtual void showPlayer1CardBackOnPlayer2(const int player1Index, const int player2Index)const;
};

//�߳���
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
	static const int maxNumOfPlayers = 8;			//��������
	static const int maxNumOfCommonCards = 5;
	static const int smallBind = 1;
	static const int bigBind = 2;
	static const int initChip = 200;				//��ʼ����
	int combineMap[21][5] = { { 0,1,2,3,4 },		//������ϱ��о�Ӧ�û���λ��
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
	string m_gameID;				//��ϷID
	vector<player> m_players;		//��Ϸ�е����
	vector<card> m_commonCards;		//������
	int m_pot;						//�׳�
	vector<sidePot> m_sidePots;		//�߳� �ֲ�
	int m_minBet;					//������С��ע
	cardHeap m_cardHeap;			//�ƶ�
	gameRound m_round;				//�غ�
	int m_nowPlayerIndex;			//���ֵ�ǰ������
	int m_endPlayerIndex;			//���ֽ���������
	int m_dealer;					//dealer��˭
	set<int> m_calledPlayersIndex;	//û��fold�������ż���
	int m_readyNumOfPlayer;			//׼���õ�����
	bool m_hasStarted;				//�Ƿ���Ϸ��
	virUI* m_ui;
	//�Ʒֱ����
	unordered_map<string, int> m_macAddressToScoreChartIndex;
	QStandardItemModel* m_scoreChart;	//����ҵ÷ּ�¼
public:
	game(string gameID = "No ID",
		vector<player> players = vector<player>(maxNumOfPlayers),
		vector<card> commonCards = vector<card>(0),
		int pot = 0,
		vector<sidePot> sidePots = vector<sidePot>(0),	//���ֻ����������߳�
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
		m_commonCards.reserve(maxNumOfCommonCards);				//�����ռ�
		m_sidePots.reserve(maxNumOfPlayers);					//�����������߳�

		m_scoreChart->setHorizontalHeaderLabels({ QStringLiteral("�����"), QStringLiteral("�ܴ���"), QStringLiteral("��ǰ�ܳ���"), QStringLiteral("���ϳ���"), QStringLiteral("��ʤ") });
		m_macAddressToScoreChartIndex.clear();
	};
	~game() = default;
	
	//get���
	string getGameID() const { return this->m_gameID; };
	string getPlayerMacAddress(const int playerIndex) { return this->m_players[playerIndex].getMacAddress(); };
	unordered_map<string, int>& getMacAddressToScoreChartIndex() { return this->m_macAddressToScoreChartIndex; };
	QStandardItemModel* getScoreChart() { return this->m_scoreChart; };	
	vector<player> const& getPlayers()const { return this->m_players; };
	player& getPlayer(int playerIndex) { return this->m_players[playerIndex]; };
	player const& getPlayer(int playerIndex)const { return this->m_players[playerIndex]; };
	int getNextCalledPlayerIndex(const int nowPlayerIndex)const;	//����������ƣ������û���Ƶģ���д����
	int getPreCalledPlayerIndex(const int nowPlayerIndex)const;		//����������ƣ������û���Ƶģ���д
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
	int addNewPlayer(string const& playerName);	//������һ����ң����������ҵ�λ��
	set<int>& getCalledPlayersIndex() { return this->m_calledPlayersIndex; };
	set<int> const& getCalledPlayersIndex()const { return this->m_calledPlayersIndex; };
	virUI* getVirUIPoint()const { return this->m_ui; };
	int getNumOfReadyPlayer()const { return this->m_readyNumOfPlayer; };
	void addNumOfReadyPlayer() { this->m_readyNumOfPlayer++; };
	void clearNumOfReadyPlayer() { this->m_readyNumOfPlayer = 0; };

	gameRound updateGameRound();
	bool addPlayer(player const& p);
	bool initPlayersState();	
	void addToPot(const int addMoney) { this->m_pot += addMoney; };		//��money����׳�
	
	void setGameID(string gameID) { this->m_gameID = gameID; };
	void setPlayerName(const int playerIndex, string const& playerName) { this->m_players[playerIndex].setName(playerName); };
	void setPlayerMacAddress(const int playerIndex, string const& macAddress) { this->m_players[playerIndex].setMacAddress(macAddress); };
	void clearCommonCards() { this->m_commonCards.clear(); };
	void updatePots();		//���µ׳���߳�
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
	void updateDealer() { int nextDealer = this->getNextCalledPlayerIndex(this->m_dealer);	this->m_dealer = nextDealer; };	//�����ô�죿Ҫ��Ҫ�жϣ�
	void showDealer() { this->m_ui->showDealer(); };
	void clearCalledPlayersIndex() { this->m_calledPlayersIndex.clear(); };

	//�������ʵ�ֵĺ���
	void nowPlayerRaise(const int raiseTo);
	void nowPlayerAllin();
	void nowPlayerCall();
	void nowPlayerCheck();
	void nowPlayerFold();

	void playerEscape(const int playerIndex);	//������� ��fold����

	void afterPlayerAction();

	void nextRound();				//������һ��

	void renderGame();				//��Ⱦ��ǰ��Ϸ״̬��������Ҫ����
	//void showNowPlayerActionMessage(string const& actionMessage);
	void nowPlayerActionComplete();
	void finishThisRound();
	bool nowPlayerRender();			//��Ⱦ��ǰ��ҵ��ж����棬������Ҫ����

	void settle();					//����,ֻʣһ�˻�����Ժ����
	void calCardTypeAndPointForAll();														//Ϊ�����ڳ����˼��������Ƶ�
	cardTypeAndPoint calCardTypeAndPointForPlayer(const int playerIndex);				//Ϊ��һ��Ҽ��������Ƶ�

	void playerWin(int winnerIndex);//���xӮ�˶���Ǯ
	void playerLose(int loserIndex);//���x����
	vector<int> getMaxPlayersIndex();										//��ȡ���������ҵ����
	vector<int> getMaxPlayersIndex(vector<int> const& curPlayerIndexSet);	//��ȡ�������������������ҵ����

	bool begin();					//��ʼ��Ϸ

	//bool playersAction();			//����ж�
	//bool playerAction(const int playerIndex);	//���N�ж�
	//int  isEnd();					//��Ϸ�Ƿ������ֻʣһ��

	//ui���
	//player show
	void showPlayerChip(const int playerIndex);
	void showPlayerName(const int playerIndex);
	void showPlayerHandCards(const int playerIndex);
	void showPlayerActionMessage(const int playerIndex);
	void showPlayerSidePot(const int playerIndex, const int money);
	void showPlayerMessage(const int playerIndex, string const& message);
	void showPlayerRaiseAction(const int playerIndex);
	void showPlayerAllinAction(const int playerIndex);		//allin������
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
	//player�����Ϊ
	void showPlayerNameCardsChip(const int playerIndex);
	void showPlayerCheckRaiseFoldAction(const int playerIndex);
	void showPlayerAllinFoldAction(const int playerIndex);
	void showPlayerCallRaiseFoldAction(const int playerIndex);

	void hidePlayerAllAction(const int playerIndex);
	void hidePlayer(const int playerIndex);
	void hideNowPlayerAllAction();							//��ǰ����ж���ϣ��ر����ж�����

	void showPlayer1CardsOnPlayer2(const int player1Index, const int player2Index);
	void showOthersCardBackOnPlayerIndex(const int playerIndex);	//�ڳ�������Ҫshow�������ú���ֻӦ��beginʱ����
	void showOthersCardBackOnAllClient();							//�Թ���ҲҪshow����

	//game���
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
	void updatePlayerScore(const int playerIndex);				//�������¼Ʒֱ�
	void updateEscapedPlayerScore(const int playerIndex);		//����������ҵļƷֱ�
};

