#pragma once
#include "cardHeap.h"
#include "player.h"
#include <set>
#include <map>

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

enum gameRound {ErrorRound = -1, Start, PreFlop, Flop, Turn, River, End};
class game {
public:
	static const int maxNumOfPlayers = 8;			//��������
	static const int maxNumOfCommonCards = 5;
	static const int smallBind = 1;
	static const int bigBind = 2;
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
	vector<sidePot> m_sidePots;	//�߳� �ֲ�
	int m_minBet;					//������С��ע
	cardHeap m_cardHeap;			//�ƶ�
	gameRound m_round;				//�غ�
	int m_nowPlayerIndex;			//���ֵ�ǰ������
	int m_endPlayerIndex;			//���ֽ���������
	int m_dealer;					//dealer��˭
	set<int> m_calledPlayersIndex;	//û��fold�������ż���
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
		int dealer = -1,
		set<int> calledPlayersIndex = {}
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
		m_calledPlayersIndex(calledPlayersIndex){
		m_commonCards.reserve(maxNumOfCommonCards);	//�����ռ�
		m_sidePots.reserve(maxNumOfPlayers);			//�����������߳�
	};
	~game() = default;
	
	string getGameID() const { return this->m_gameID; };
	vector<player> const& getPlayers()const { return this->m_players; };
	player& getPlayer(int playerIndex) { return this->m_players[playerIndex]; };
	player const& getPlayer(int playerIndex)const { return this->m_players[playerIndex]; };
	int getNextCalledPlayerIndex(const int nowPlayerIndex)const;	//����������ƣ������û���Ƶģ���д����
	int getPreCalledPlayerIndex(const int nowPlayerIndex)const;	//����������ƣ������û���Ƶģ���д

	vector<card> const& getCommonCards()const { return this->m_commonCards; };
	int getPot()const { return this->m_pot; };
	int getMinBet()const { return this->m_minBet; };
	int getNowPlayerIndex()const { return this->m_nowPlayerIndex; };
	int getEndPlayerIndex()const { return this->m_endPlayerIndex; };
	int getRoundBeginPlayerIndex()const;
	int getRoundEndPlayerIndex()const;
	int getDealer()const { return this->m_dealer; };
	set<int>& getCalledPlayersIndex() { return this->m_calledPlayersIndex; };
	set<int> const& getCalledPlayersIndex()const { return this->m_calledPlayersIndex; };

	void setGameID(string gameID) { this->m_gameID = gameID; };

	bool addPlayer(player const& p);
	int getNumOfPlayers()const;
	player& getPlayer(string playerID)const;
	bool initPlayersState();

	int getNumOfCommonCards()const { return this->m_commonCards.size(); };
	void clearCommonCards() { this->m_commonCards.clear(); };
	gameRound updateGameRound();

	void addToPot();
	void updatePots();		//���µ׳���߳�
	void clearPot() { this->m_pot = 0; };
	int addNewSidePot(const int money) { this->m_sidePots.push_back(sidePot(0)); return (int)this->m_sidePots.size() - 1; };
	void addPlayerToLastSidePot(const int sidePotIndex, const int playerAddedIndex) { this->m_sidePots[sidePotIndex].insertParticipate(playerAddedIndex); };
	void clearSidePot() { this->m_sidePots.clear(); };

	void setMinBet(int minBet) { this->m_minBet = minBet; };

	void shuffleCardHeap() { this->m_cardHeap.shuffle(); };
	void sendCardsToPlayers();
	void sendCardsToTable();

	void updateEndPlayerIndex();
	void updateNowPlayerIndex();
	void setNowPlayerIndex(const int nowPlayerIndex) { this->m_nowPlayerIndex = nowPlayerIndex; };
	void setEndPlayerIndex(const int endPlayerIndex) { this->m_endPlayerIndex = endPlayerIndex; };
	void updateDealer() {
		int nextDealer = this->getNextCalledPlayerIndex(this->m_dealer);
		this->m_dealer = nextDealer;	//�����ô�죿Ҫ��Ҫ�жϣ�

		//�ƺ�û���˺����
		//int nextDealerIndex = this->m_dealer;
		//do {
		//	nextDealerIndex++;
		//	if (nextDealerIndex >= maxNumOfPlayers)
		//		nextDealerIndex -= maxNumOfPlayers;
		//	if (this->m_players[nextDealerIndex].getPlayerType() == playerType::OnSitePlayer) {	//��getRoundBeginPlayerIndex��֮ͬ�������б��������ڸ���calledPlayer�Ժ�Ӧ������һ��
		//		this->m_dealer = nextDealerIndex;
		//		return;		//�ҵ���
		//	}
		//} while (nextDealerIndex != this->m_dealer);	//һȦ�˻�û�ҵ�
		//this->m_dealer = -1;
		//return;
		//this->m_dealer = this->getNextPlayerIndex(this->m_dealer);		//updateDealer֮ǰӦ�ü�����calledPlayer��ֱ�ӵ������Ӧ�þ��У������У��Ͼ�dealer���ܲ��ڱ�����Ϸ��
	};

	void clearCalledPlayersIndex() { this->m_calledPlayersIndex.clear(); };

	void nowPlayerRaise(const int raiseTo);
	void nowPlayerCall();
	void nowPlayerCheck();
	void nowPlayerFold();

	void afterPlayerAction();
	void nextRound();				//������һ��

	virtual void renderGame();		//��Ⱦ��ǰ��Ϸ״̬��������Ҫ����
	virtual bool nowPlayerRender();	//��Ⱦ��ǰ��ҵ��ж����棬������Ҫ����

	void settle();			//����,ֻʣһ�˻�����Ժ����
	void calCardTypeAndPointForAll();							//Ϊ�����ڳ����˼��������Ƶ�
	void calCardTypeAndPointForPlayer(const int playerIndex);	//Ϊ��һ��Ҽ��������Ƶ�

	void playerWin(int winnerIndex);				//���xӮ�˶���Ǯ
	void playerLose(int loserIndex);				//���x����
	vector<int> getMaxPlayersIndex();										//��ȡ���������ҵ����
	vector<int> getMaxPlayersIndex(vector<int> const& curPlayerIndexSet);	//��ȡ�������������������ҵ����

	//
	bool begin();					//��ʼ��Ϸ
	//bool playersAction();			//����ж�
	//bool playerAction(const int playerIndex);	//���N�ж�
	//int  isEnd();					//��Ϸ�Ƿ������ֻʣһ��
};



