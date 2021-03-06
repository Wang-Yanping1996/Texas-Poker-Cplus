#include "cardHeap.h"
#include <time.h>
#include <algorithm>
#include <functional>

cardNumber to_cardNumber(const int n)
{
	return cardNumber(n - 2 + (int)cardNumber::Two); 
}

int to_int(const cardNumber num)
{
	return 2 + (int)num - (int)cardNumber::Two; 
}

string to_string(cardType type) {
	if (type == cardType::HighCard)
		return "高牌";
	else if (type == cardType::OnePair)
		return "一对";
	else if (type == cardType::TwoPairs)
		return "两对";
	else if (type == cardType::ThreeOfKind)
		return "三条";
	else if (type == cardType::Straight)
		return "顺子";
	else if (type == cardType::Flush)
		return "同花";
	else if (type == cardType::FullHouse)
		return "葫芦";
	else if (type == cardType::FourOfKind)
		return "四条";
	else if (type == cardType::StraightFlush)
		return "同花顺";
	else if (type == cardType::RoyalFlush)
		return "皇家同花顺";
	else {}

	return "错误";
};

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

QString card::getCardFileName(card const& c) {
	cardColor color = c.getColor();
	cardNumber num = c.getNumber();
	if (color == cardColor::CardBackColor&&num == cardNumber::CardBackNumber) {
		QString cardFileName = QString::fromUtf8("image/poker/") + QString::fromUtf8("back") + QString::fromUtf8(".jpg");
		return cardFileName;
	}
	QString cardFileName = QString::fromUtf8("image/poker/") + QString::number((int)color) + QString::fromUtf8("_") + QString::number((int)(num / 10)) + QString::number((int)(num % 10)) + QString::fromUtf8(".jpg");
	return cardFileName;
}

std::string card::cardToString(card const& c) {
	std::string res;
	cardColor color = c.getColor();
	switch (color)
	{
	case Club:
		res += 'C';
		break;
	case Diamond:
		res += 'D';
		break;
	case Heart:
		res += 'H';
		break;
	case Spade:
		res += 'S';
		break;
	default:
		break;
	}
	const int actualNum = to_int(c.getNumber());
	if (actualNum <= 10 && actualNum >= 2) {
		res += std::to_string(actualNum);
	}
	else if (actualNum == 11) {
		res += 'J';
	}
	else if (actualNum == 12) {
		res += 'Q';
	}
	else if (actualNum == 13) {
		res += 'K';
	}
	else if (actualNum == 14) {
		res += 'A';
	}
	else {
		res = "未知";
	}
	return res;
}
//cardHeap牌堆类
cardHeap::cardHeap() {	//初始化函数
	this->numOfCards = cardHeap::maxCards;
	int nCard = 0;
	this->cardArray.resize(this->numOfCards);
	for (cardColor color = cardColor::Club; color <= cardColor::Spade; color = cardColor(color + 1)) {
		for (cardNumber number = cardNumber::Two; number <= cardNumber::Ace; number = cardNumber(number + 1)) {
			this->cardArray[nCard++] = card(color, number);
		}
	}
	this->topIndex = 0;
}
bool cardHeap::removeCards(std::unordered_set<int> const & needRemoveCards)
{
	std::vector<card> newCardArray;
	for (auto const c : this->cardArray) {
		const int cNum = to_int(c.getNumber());
		if (needRemoveCards.find(cNum) == needRemoveCards.end()) {		//不在范围内加入
			newCardArray.push_back(c);
		}
	}
	this->numOfCards = newCardArray.size();
	this->cardArray.swap(newCardArray);
	return true;
}
bool cardHeap::removeCards(int begin, int end)
{
	if (begin > end) {	//保证end比begin大
		swap(begin, end);
	}
	std::vector<card> newCardArray;
	for (auto const c : this->cardArray) {
		const int cNum = to_int(c.getNumber());
		if (cNum < begin || cNum > end ) {	//不在范围内 加入
			newCardArray.push_back(c);
		}
	}
	this->numOfCards = newCardArray.size();
	this->cardArray.swap(newCardArray);
	return true;
}
card cardHeap::getCard() {	//获取一张牌，这里直接获取顶部的牌
	return this->getTopCard();
}
card cardHeap::getTopCard() {	//获取顶部的牌
	if (this->topIndex >= this->numOfCards) {
		return card(cardColor::ErrorColor, cardNumber::ErrorNumber);
	}
	card res = this->cardArray[this->topIndex];
	this->topIndex++;
	return res;
}
void cardHeap::shuffle() {	//洗牌	 Knuth-Durstenfeld Shuffle  
	srand((unsigned)time(NULL));
	for (int i = this->numOfCards - 1; i >= 0; --i) {
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
