#pragma once
#include "cardHeap.h"

enum playerType { Empty = -1, Looker, OnSitePlayer };
enum actionType { ErrorAction = -1, Nothing, Allin, Raise, Call, Check, Fold };
class player {
public:
	static const int numOfHandCards = 2;

private:

	//bool m_isRealPlayer;				//是否是真的玩家，考虑用game中玩家数组空指针代替
	playerType m_playerType;			//玩家类型，空，旁观者（无筹码），在桌上的玩家，
	string m_name;						//姓名
	vector<card> m_handCards;			//手牌
	int m_chip;							//玩家筹码
	int m_nowBet;						//现在已下注
	//int m_sidePot;					//自己的边池
	int m_winMoney;						//这局赢了多少
	string m_macAddress;				//mac地址
	cardTypeAndPoint m_cardTypeAndPoint;//牌型和牌点
	//bool m_hasFold;						//是否弃牌
	actionType m_playerAction;			//player的上一个动作			
public:
	player(playerType playerTypeInput = Empty,
		string name = "not a player",
		vector<card> handCards = {card(), card()},
		int chip = -1,
		int nowBet = -1,
		//int sidePot = -1,
		int winMoney = 0,
		string macAddress = "",
		cardTypeAndPoint cardTypeAndPoint = cardTypeAndPoint(cardType::ErrorType),
		//bool hasFold = false,
		actionType playerAction = actionType::ErrorAction
	) :m_playerType(playerTypeInput),
		m_name(name),
		m_handCards(handCards),
		m_chip(chip),
		m_nowBet(nowBet),
		//m_sidePot(sidePot),
		m_winMoney(winMoney),
		m_macAddress(macAddress),
		m_cardTypeAndPoint(cardTypeAndPoint),
		//m_hasFold(hasFold)
		m_playerAction(playerAction){};
	player(player const& another);
	player& operator = (player const& another);
	~player() = default;

	//bool isRealPlayer() const { return this->m_isRealPlayer; };
	playerType getPlayerType()const { return this->m_playerType; };
	string getName() const { return this->m_name; };
	vector<card> const& getHandCards() const { return this->m_handCards; };
	int getChip() const { return this->m_chip; };
	int getNowBet() const { return this->m_nowBet; };
	//int getSidePot() const { return this->m_sidePot; };
	int getWinMoney() const { return this->m_winMoney; };
	string getMacAddress() const { return this->m_macAddress; };
	cardTypeAndPoint getCardTypeAndPoint() const { return this->m_cardTypeAndPoint; };
	//bool hasFold() const { return this->m_playerAction == actionType::Fold; };
	actionType getPlayerAction()const { return this->m_playerAction; };
	bool hasAllin()const { return this->m_playerAction == actionType::Allin && this->m_chip == 0; };
	bool isAllinThisRound(){ return this->m_playerAction == actionType::Allin && this->m_chip == 0 && this->m_nowBet > 0; };

	//void setRealPlayer(const bool isRealPlayer) { this->m_isRealPlayer = isRealPlayer; };
	void setPlayerType(const playerType playerTypeInput) { this->m_playerType = playerTypeInput; };
	void setName(string name) { this->m_name = name; };
	void setHandCards(vector<card> const& handCards) { this->m_handCards = handCards; };
	void clearHandCards() { this->m_handCards.clear(); };
	void setChip(const int chip) { this->m_chip = chip; };
	void setNowBet(const int nowBet) { this->m_nowBet = nowBet; };
	//void setSidePot(const int sidePot) { this->m_sidePot = sidePot; };
	void setWinMoney(const int winMoney) { this->m_winMoney = winMoney; };
	void addToWinMoney(const int add) { this->m_winMoney += add; };
	void setMacAddress(string macAddress) { this->m_macAddress = macAddress; };
	void setCardTypeAndPoint(const cardTypeAndPoint cardTypeAndPoint) { this->m_cardTypeAndPoint = cardTypeAndPoint; };
	void setFold() { this->setPlayerAction(actionType::Fold); };
	void setPlayerAction(actionType action) { this->m_playerAction = action; };

	void action();
	void add(const int addTo);
	void fold();
	void call();

	void win();
	void lose();

	void initPlayerState();
};


