#include "player.h"

player::player(player const & another) {
	this->m_playerType = another.m_playerType;
	this->m_name = another.m_name;
	this->m_handCards = another.m_handCards;
	this->m_chip = another.m_chip;
	this->m_nowBet = another.m_nowBet;
	this->m_winMoney = another.m_winMoney;
	this->m_IP = another.m_IP;
	this->m_cardTypeAndPoint = another.m_cardTypeAndPoint;
	this->m_playerAction = another.m_playerAction;
}

player & player::operator=(player const & another)
{
	this->m_playerType = another.m_playerType;
	this->m_name = another.m_name;
	this->m_handCards = another.m_handCards;
	this->m_chip = another.m_chip;
	this->m_nowBet = another.m_nowBet;
	this->m_winMoney = another.m_winMoney;
	this->m_IP = another.m_IP;
	this->m_cardTypeAndPoint = another.m_cardTypeAndPoint;
	this->m_playerAction = another.m_playerAction;
	return *this;
}

void player::add(const int addTo){
	this->m_chip -= addTo - this->m_nowBet;	//需要补上这么多
	this->m_nowBet = addTo;
}

void player::fold(){
	this->setFold();
}

void player::win(){
	this->m_chip += this->m_winMoney;
	this->m_winMoney = 0;
	//this->m_sidePot = 0;
}

void player::lose(){
	this->m_winMoney = 0;
	if (this->m_chip == 0) {
		this->m_playerType = playerType::Looker;
	}
}

void player::initPlayerState(){
	if (this->m_playerType == playerType::OnSitePlayer) {
		this->clearHandCards();
		this->setNowBet(0);
		this->setWinMoney(0);
		this->setCardTypeAndPoint(cardTypeAndPoint(cardType::ErrorType));
		this->setPlayerAction(actionType::Nothing);
		//this->setFold();
	}
}
