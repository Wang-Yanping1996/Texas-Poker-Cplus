#include "player.h"

void player::add(const int addTo){
	this->m_chip -= addTo - this->m_nowBet;	//需要补上这么多
	this->m_nowBet = addTo;
}

void player::fold(){
	this->m_hasFold = true;
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
		this->setFold(false);
	}
}
