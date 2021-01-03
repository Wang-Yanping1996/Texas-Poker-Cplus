#include "cardHeap.h"
#include <time.h>
#include <algorithm>
#include <functional>
//card类
card::card(card const& another) {	//拷贝构造
	this->m_color = another.m_color;
	this->m_number = another.m_number;
}
card& card::operator = (card const& another) {	//赋值
	this->m_color = another.m_color;
	this->m_number = another.m_number;
	return *this;
}
bool card::operator==(card const& another) const {	//是否相同，只比较数字
	return this->m_number == another.m_number;
}
bool card::operator<(card const& another) const {	//比较大小，只比较数字
	return this->m_number < another.m_number;
}
bool card::operator>(card const& another) const {	//比较大小，只比较数字
	return this->m_number > another.m_number;
}

//cardHeap牌堆类
cardHeap::cardHeap() {	//初始化函数
	int nCard = 0;
	for (cardColor color = cardColor::Club; color <= cardColor::Spade; color = cardColor(color + 1)) {
		for (cardNumber number = cardNumber::Two; number <= cardNumber::Ace; number = cardNumber(number + 1)) {
			this->cardArray[nCard++] = card(color, number);
		}
	}
	this->topIndex = 0;
}
card cardHeap::getCard() {	//获取一张牌，这里直接获取顶部的牌
	return this->getTopCard();
}
card cardHeap::getTopCard() {	//获取顶部的牌
	if (this->topIndex >= maxCards) {
		return card(cardColor::ErrorColor, cardNumber::ErrorNumber);
	}
	card res = this->cardArray[this->topIndex];
	this->topIndex++;
	return res;
}
void cardHeap::shuffle() {	//洗牌	 Knuth-Durstenfeld Shuffle  
	for (int i = maxCards - 1; i >= 0; --i) {
		srand((unsigned)time(NULL));
		swap(this->cardArray[rand() % (i + 1)], this->cardArray[i]);
	}
	this->topIndex = 0;
}

bool cardTypeAndPoint::operator>(cardTypeAndPoint const & another) const
{
	if (this->m_cardType > another.m_cardType)
		return true;
	else if (this->m_cardType < another.m_cardType)
		return false;
	//现在相等牌型
	for (int i = 0; i < 5; ++i) {
		if (this->m_cardNumberArray[i] > another.m_cardNumberArray[i])
			return true;
		else if (this->m_cardNumberArray[i] < another.m_cardNumberArray[i])
			return false;
	}
	//全部相等
	return false;
}

bool cardTypeAndPoint::operator<(cardTypeAndPoint const & another) const
{
	if (this->m_cardType < another.m_cardType)
		return true;
	else if (this->m_cardType > another.m_cardType)
		return false;
	//现在相等牌型
	for (int i = 0; i < 5; ++i) {
		if (this->m_cardNumberArray[i] < another.m_cardNumberArray[i])
			return true;
		else if (this->m_cardNumberArray[i] > another.m_cardNumberArray[i])
			return false;
	}
	//全部相等
	return false;
}

bool cardTypeAndPoint::operator==(cardTypeAndPoint const & another) const
{
	if (this->m_cardType != another.m_cardType)
		return false;
	//现在相等牌型
	for (int i = 0; i < 5; ++i) {
		if (this->m_cardNumberArray[i] != another.m_cardNumberArray[i])
			return false;
	}
	//全部相等
	return true;
}

cardTypeAndPoint cardTypeAndPoint::calTypeAndPoint(card arr[])
{
	int i = 0;
	int pk[5];
	int cl[5];
	cardTypeAndPoint res;

	int cardNum[13] = { 0 };
	bool Straight = false;
	bool Flush = false;

	for (i = 0; i < 5; i++)
	{
		pk[i] = arr[i].getNumber();
		cl[i] = arr[i].getColor();
	}

	for (i = 0; i < 5; i++)
	{
		cardNum[pk[i]]++;
	}

	Flush = true;
	for (i = 1; i < 5; i++)
	{
		if (cl[i] != cl[i - 1])
		{
			Flush = false;
		}
	}
	sort(pk, pk + 5, greater<int>());
	Straight = true;
	for (int i = 1; i < 5; i++)
	{
		if (pk[i] != pk[i - 1] - 1)
		{
			Straight = false;
			break;
		}
	}
	vector<cardNumber>& cardNmberArray = res.getCardNumberArray();
	for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)pk[i]; }
	if (Straight || Flush)
	{
		if (Straight && Flush)
		{
			res.setCardType(cardType::StraightFlush);
		}
		else if (Straight)
		{
			res.setCardType(cardType::Straight);
		}
		else
		{
			res.setCardType(cardType::Flush);
		}
		return res;
	}
	//others
	int pairs[5] = { 0 };
	for (int i = 0; i < 13; i++)
	{
		pairs[cardNum[i]]++;
	}
	if (pairs[4] != 0)//四条
	{
		if (pk[0] != pk[1]) // abbbb &&a>b
		{
			swap(pk[0], pk[4]);
		}
		for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)pk[i]; }
		res.setCardType(cardType::FourOfKind);
	}
	else if (pairs[3] != 0)//三条或葫芦
	{
		for (i = 0; i < 3; i++)
		{
			if (pk[i] == pk[i + 1] && pk[i + 1] == pk[i + 2])
			{
				break;
			}
		}
		// i,i+1,i+2
		int buf[5];
		if (i == 0)
		{
			for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)pk[i]; }
		}
		else if (i == 1)
		{
			buf[0] = pk[1];
			buf[1] = pk[2];
			buf[2] = pk[3];
			buf[3] = pk[0];
			buf[4] = pk[4];
			for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)buf[i]; }
		}
		else
		{
			buf[0] = pk[2];
			buf[1] = pk[3];
			buf[2] = pk[4];
			buf[3] = pk[0];
			buf[4] = pk[1];
			for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)buf[i]; }
		}
		if (pairs[2] != 0)
		{
			res.setCardType(cardType::FullHouse);
		}
		else
		{
			res.setCardType(cardType::ThreeOfKind);
		}
	}
	else if (pairs[2] != 0)//一对或两对
	{
		int buf[5];
		if (pairs[2] == 2)
		{
			res.setCardType(cardType::TwoPairs);
			if (pk[0] != pk[1])
			{
				buf[0] = pk[1];
				buf[1] = pk[2];
				buf[2] = pk[3];
				buf[3] = pk[4];
				buf[4] = pk[0];
				for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)buf[i]; }
			}
			else if (pk[3] != pk[4])//aabbc
			{
				for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)pk[i]; }
			}
			else// aabcc->aaccb
			{
				swap(pk[2], pk[4]);
				for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)pk[i]; }
			}
		}
		else
		{
			res.setCardType(cardType::OnePair);
			for (i = 0; i < 4; i++)
			{
				if (pk[i] == pk[i + 1])
					break;
			}
			buf[0] = pk[i];
			buf[1] = pk[i + 1];
			int c = 2;
			//有点迷
			for (int j = 0; j<i; j++)
				buf[c++] = pk[j];
			for (int j = i + 2; j<5; j++)
				buf[c++] = pk[j];
			for (i = 0; i < 5; i++) { cardNmberArray[i] = (cardNumber)buf[i]; }
		}
	}
	else //high card
	{
		res.setCardType(cardType::HighCard);
		for (i = 0; i < 5; i++)
		{
			cardNmberArray[i] = (cardNumber)pk[i];
		}
	}
	return res;
}
