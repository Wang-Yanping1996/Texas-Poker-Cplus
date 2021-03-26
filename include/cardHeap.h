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
//�� ������ɫ������
class card {
private:
	cardColor m_color;
	cardNumber m_number;
public:
	card(cardColor color = cardColor::ErrorColor, cardNumber number = cardNumber::ErrorNumber) : m_color(color), m_number(number) {};			//���캯��
	card(card const& anotherCard);				//��������
	card& operator = (card const& another);		//��ֵ																		
	~card() = default;

	static QString getCardFileName(card const& c);
	cardColor getColor() const { return this->m_color; };		//��ȡ��ɫ
	cardNumber getNumber() const { return this->m_number; };	//��ȡ����
	bool isCardVailid()const { return this->m_color != cardColor::ErrorColor&&this->m_number != cardNumber::ErrorNumber; };

	bool operator == (card const& another) const;				//�Ƿ���ͬ��ֻ�Ƚ�����
	bool operator > (card const& another) const;				//�Ƚϴ�С��ֻ�Ƚ�����
	bool operator < (card const& another) const;				//�Ƚϴ�С��ֻ�Ƚ�����

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

//�ƶѣ�����52����
class cardHeap {
private:
	static const int maxCards = 52;	//�����ܵ�����
	int numOfCards;					//ʵ��һ��������������ҪΪ�˶��ƣ�
	//card cardArray[maxCards];		//�ƶ�����
	std::vector<card> cardArray;	
	int topIndex;					//��¼��ǰ�ƶѶ��������еڼ�����
public:
	cardHeap();			//��ʼ������
	~cardHeap() = default;

	//ɾ��һЩ��
	bool removeCards(std::unordered_set<int> const& needRemoveCards);
	bool removeCards(int begin, int end);

	card getCard();		//��ȡһ����
	card getTopCard();	//��ȡ������
	void shuffle();		//ϴ��
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


