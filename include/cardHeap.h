#pragma once
#include <vector>
#include <unordered_set>

#include <QtWidgets/QMainWindow>
//#define MAXCARDS (52)
using namespace std;

enum cardColor { ErrorColor = -1, Club, Diamond, Heart, Spade, CardBackColor, E = -1, C, D, H, S };
enum cardNumber { ErrorNumber = -1, Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace, CardBackNumber};
enum cardType {ErrorType = -1, HighCard, OnePair, TwoPairs, ThreeOfKind, Straight, Flush, FullHouse, FourOfKind, StraightFlush, RoyalFlush};

cardNumber to_cardNumber(const int n);
int to_int(const cardNumber num);
string to_string(cardType type);
//牌 包括颜色和数字
class card {
private:
	cardColor m_color;
	cardNumber m_number;
public:
	card(cardColor color = cardColor::ErrorColor, cardNumber number = cardNumber::ErrorNumber) : m_color(color), m_number(number) {};			//构造函数
	card(card const& anotherCard);				//拷贝构造
	card& operator = (card const& another);		//赋值																		
	~card() = default;

	static QString getCardFileName(card const& c);
	cardColor getColor() const { return this->m_color; };		//获取颜色
	cardNumber getNumber() const { return this->m_number; };	//获取数字
	bool isCardVailid()const { return this->m_color != cardColor::ErrorColor&&this->m_number != cardNumber::ErrorNumber; };

	bool operator == (card const& another) const;				//是否相同，只比较数字
	bool operator > (card const& another) const;				//比较大小，只比较数字
	bool operator < (card const& another) const;				//比较大小，只比较数字

};
//QString getCardFileName(card const& c);

//class nCards{
//private:
//	int m_numOfCards;
//	vector<card> m_cardArray;
//public:
//	int	getNumOfCards() const { return this->m_numOfCards; };
//};
////
//class twoCards {
//
//};

//牌堆，包括52张牌
class cardHeap {
private:
	static const int maxCards = 52;	//最大可能的牌数
	int numOfCards;					//实际一副牌中牌数（主要为了短牌）
	//card cardArray[maxCards];		//牌堆数组
	std::vector<card> cardArray;	
	int topIndex;					//记录当前牌堆顶是数组中第几张牌
public:
	cardHeap();			//初始化函数
	~cardHeap() = default;

	//删除一些牌
	bool removeCards(std::unordered_set<int> const& needRemoveCards);
	bool removeCards(int begin, int end);

	card getCard();		//获取一张牌
	card getTopCard();	//获取顶部牌
	void shuffle();		//洗牌
};

class cardTypeAndPoint {
private:
	static const int maxNumOfCard = 5;
	cardType m_cardType;
	vector<cardNumber> m_cardNumberArray;
public:
	cardTypeAndPoint(cardType cardType = cardType::ErrorType, vector<cardNumber> cardNumberArray = vector<cardNumber>(maxNumOfCard, cardNumber::ErrorNumber)) :m_cardType(cardType), m_cardNumberArray(cardNumberArray) {};

	cardType getCardType() const { return this->m_cardType; };
	vector<cardNumber>& getCardNumberArray() { return this->m_cardNumberArray; };

	void setCardType(cardType cardType) { this->m_cardType = cardType; };
	void setCardNumberArray(int arr[]) {
		this->m_cardNumberArray.resize(maxNumOfCard);
		for (int i = 0; i < maxNumOfCard; ++i) 
			this->m_cardNumberArray[i] = (cardNumber)arr[i];
	}

	bool operator > (cardTypeAndPoint const& another) const;
	bool operator < (cardTypeAndPoint const& another) const;
	bool operator == (cardTypeAndPoint const& another) const;

	static cardTypeAndPoint calTypeAndPoint(card arr[]);
};


