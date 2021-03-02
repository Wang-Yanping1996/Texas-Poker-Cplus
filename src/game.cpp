#include "game.h"
#include <iostream>
#include <algorithm>
//int game::getNextPlayerIndex(int nowPlayerIndex)const {
//	auto nowPlayerIterator = this->m_calledPlayersIndex.find(nowPlayerIndex);
//	if (nowPlayerIterator == this->m_calledPlayersIndex.end()) {	//û�ҵ���ǰ���
//		return -1;
//	}
//	auto nextPlayerIterator = nowPlayerIterator++;
//	if (nextPlayerIterator == this->m_calledPlayersIndex.end()) {
//		nextPlayerIterator = this->m_calledPlayersIndex.begin();
//	}
//	return *nextPlayerIterator;
//}
int game::getNextCalledPlayerIndex(const int nowPlayerIndex)const {	//����������ƣ������û���Ƶ�
	if (this->m_calledPlayersIndex.size() < 1)		//û����
		return -1;
	if (this->m_calledPlayersIndex.size() == 1)		//����һ���������
		return *(this->m_calledPlayersIndex.begin());
	//������nowPlayerIndex���Ƶ����
	int nextCalledPlayerIndex = nowPlayerIndex;
	do {
		nextCalledPlayerIndex++;
		if (nextCalledPlayerIndex >= maxNumOfPlayers)
			nextCalledPlayerIndex -= maxNumOfPlayers;
	} while (this->m_calledPlayersIndex.find(nextCalledPlayerIndex) == this->m_calledPlayersIndex.end());	//�Ҳ���һֱѭ��
	return nextCalledPlayerIndex;
}
int game::getPreCalledPlayerIndex(const int nowPlayerIndex) const{	//����������ƣ������û���Ƶ�
	if (this->m_calledPlayersIndex.size() < 1)		//û����
		return -1;
	if (this->m_calledPlayersIndex.size() == 1)		//����һ���������
		return *(this->m_calledPlayersIndex.begin());
	int preCalledPlayerIndex = nowPlayerIndex;
	do {
		preCalledPlayerIndex--;
		if (preCalledPlayerIndex < 0)
			preCalledPlayerIndex += maxNumOfPlayers;
	} while (this->m_calledPlayersIndex.find(preCalledPlayerIndex) == this->m_calledPlayersIndex.end());	//�Ҳ���һֱѭ��
	return preCalledPlayerIndex;
}
int game::getNumOfPlayers() const
{
	int numOfPlayer = 0;
	for (int i = 0; i < game::maxNumOfPlayers; ++i) {
		if (this->m_players[i].getPlayerType() == playerType::OnSitePlayer)
			numOfPlayer++;
	}
	return numOfPlayer;
}
int game::getRoundBeginPlayerIndex() const {
	//int beginPlayerIndex = this->m_dealer;				//��dealer��һλ��ʼ
	//do {
	//	beginPlayerIndex++;								//��++
	//	if (beginPlayerIndex >= maxNumOfPlayers)		//ѭ��	
	//		beginPlayerIndex -= maxNumOfPlayers;
	//	if (this->m_calledPlayersIndex.find(beginPlayerIndex) != this->m_calledPlayersIndex.end()) 
	//		return beginPlayerIndex;					//�ҵ��ˣ��˳�
	//} while (beginPlayerIndex != this->m_dealer);		//����һȦ��û�ҵ�������
	//return -1;

	//��д��
	return this->getNextCalledPlayerIndex(this->m_dealer);	//ֱ�ӻ�ȡdealer��һ��
}
int game::getRoundEndPlayerIndex() const {
	//��д�ˣ�dealer�ھ���dealer�����ھ���ǰһ����Ч��
	if (this->m_calledPlayersIndex.find(this->m_dealer) != this->m_calledPlayersIndex.end())
		return this->m_dealer;
	return  this->getPreCalledPlayerIndex(this->m_dealer);
}

int game::addNewPlayer(string const& playerName)
{
	vector<int> emptyPlayerIndex(0);
	for (int i = 0; i < (int)this->m_players.size(); ++i) {
		if (this->m_players[i].getPlayerType() == playerType::Empty)
			emptyPlayerIndex.push_back(i);
	}
	const int index = rand() % emptyPlayerIndex.size();
	const int addPlayerIndex = emptyPlayerIndex[index];
	if (this->m_hasStarted)	//��Ϸ�ѿ�ʼ��������Ϊ�Թ���
		this->m_players[addPlayerIndex] = player(playerType::Looker, playerName, { card(), card() }, this->initChip);
	else					//��Ϸδ��ʼ��������ֱ���ϳ�
		this->m_players[addPlayerIndex] = player(playerType::OnSitePlayer, playerName, { card(), card() }, this->initChip);
	return addPlayerIndex;
}

//bool game::playersAction() {			//Ҫ��Ҫ���ǰ�m_calledPlayersIndex�������û�ָ���set���о�������һ�㣿
//	int nowPlayerIndex = this->getNextPlayerIndex(this->m_dealer);
//	int endPlayerIndex = nowPlayerIndex;
//	do {
//		this->playerAction(nowPlayerIndex);
//		nowPlayerIndex = this->getNextPlayerIndex(nowPlayerIndex);
//	} while (nowPlayerIndex != endPlayerIndex);
//	return true;
//}
//bool game::playerAction(const int playerIndex){
//	player& nowPlayer = this->getPlayer(playerIndex);
//	//�Ժ�Ҫ��
//	int actionType = -1;	
//	cout << "���" << playerIndex << "�ж���(1 raise, 2 check, 3 fold)" << endl;
//	cin >> actionType;
//	if (actionType == 1) {
//		int raiseTo = -1;
//		while (raiseTo < this->m_minBet) {
//			cout << "raise to: (min " << this->m_minBet << ")" << endl;
//			cin >> raiseTo;
//		}
//		nowPlayer.raise(raiseTo);
//	}
//	else if (actionType == 2) {
//		
//	}
//	//nowPlayer.action();
//	if (nowPlayer.hasFold()) {
//		this->m_calledPlayersIndex.erase(playerIndex);
//	}
//	return true;
//}
void game::sendCardsToPlayers() {
	for (auto nowPlayerIndex = this->m_calledPlayersIndex.begin(); nowPlayerIndex != this->m_calledPlayersIndex.end(); ++nowPlayerIndex) {
		player& nowplayer = this->getPlayer(*nowPlayerIndex);
		card card1 = this->m_cardHeap.getCard();
		card card2 = this->m_cardHeap.getCard();
		nowplayer.setHandCards(vector<card>{card1, card2});
	}
}
void game::sendCardsToTable() {
	if (this->m_round == PreFlop) {	//��ǰ
		this->sendCardsToPlayers();
	}
	else if (this->m_round == Flop) {	//���ƣ�������
		//this->m_commonCards[0] = this->m_cardHeap.getCard();
		//this->m_commonCards[1] = this->m_cardHeap.getCard();
		//this->m_commonCards[2] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
	else if (this->m_round == Turn) {	//ת�ƣ���һ��
		//this->m_commonCards[3] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
	else if (this->m_round == River) {	//���ƣ���һ��
		//this->m_commonCards[4] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
}

//�ĸ���ǰ��ҵĶ��������Խ���Ұ���ȫ��������
void game::nowPlayerRaise(const int raiseTo) {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	if (raiseTo >= nowPlayer.getChip() + nowPlayer.getNowBet()) {	//�Ƿ���allin
		this->nowPlayerAllin();
		return;
	}
	nowPlayer.add(raiseTo);
	nowPlayer.setPlayerAction(actionType::Raise);
	this->setMinBet(raiseTo);			//����������С��ע
	//���µ�ǰ��Һͽ�����ң���Ϊraise�ˣ���ȻҪ��һ�֣����Բ��ж��Ƿ������һ��
	//this->afterPlayerAction();		//���Ա���ɾ��
	//this->nowPlayerActionComplete();	//��ǰ��ҽ����ж���Ⱦ
	this->m_endPlayerIndex = this->getPreCalledPlayerIndex(this->m_nowPlayerIndex);
	this->afterPlayerAction();
	//this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//��һλ���
	//this->nowPlayerRender();															//��Ⱦ��һλ��ҵĽ���
}
void game::nowPlayerAllin() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	const int remainderChip = nowPlayer.getChip();
	const int nowBet = nowPlayer.getNowBet();
	const int allMoney = remainderChip + nowBet;
	nowPlayer.add(allMoney);			//��ע������Ǯ
	nowPlayer.setPlayerAction(actionType::Allin);

	if (this->m_minBet < allMoney) {	//allin��Ǯ��������ע����Ҫ���½������
		this->m_endPlayerIndex = this->getPreCalledPlayerIndex(this->m_nowPlayerIndex);
		this->setMinBet(allMoney);		//����������С��ע
	}
	this->afterPlayerAction();

}
void game::nowPlayerCall() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(this->m_minBet);		//������Ҽӵ���Сbet
	nowPlayer.setPlayerAction(actionType::Call);
	this->afterPlayerAction();
}
void game::nowPlayerCheck() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	//nowPlayer.add(0);		//�ӵ�0������ɾ��
	nowPlayer.setPlayerAction(actionType::Check);
	this->afterPlayerAction();
}
void game::nowPlayerFold() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.fold();
	this->hideNowPlayerHandCards();			//�������ƣ�����fold������Ϊ����
	this->m_calledPlayersIndex.erase(this->m_nowPlayerIndex);	//����call������ɾ��
	//nowPlayer.setPlayerAction(actionType::Fold);	//����player.fold()��
	this->afterPlayerAction();
}
void game::playerEscape(const int playerIndex)	//����fold����
{
	this->hidePlayer(playerIndex);				//ֱ������

	if (this->getEndPlayerIndex() == playerIndex) {
		const int newEndPlayerIndex = this->getPreCalledPlayerIndex(playerIndex);
		this->setEndPlayerIndex(newEndPlayerIndex);
	}
	player& escapedPlayer = this->getPlayer(playerIndex);
	this->addToPot(escapedPlayer.getNowBet());		//����ע����׳�		
	this->updateEscapedPlayerScore(playerIndex);	//����������ҵļƷֱ�

	this->hidePlayerHandCards(playerIndex);			//�������ƣ�����fold������Ϊ����
	this->m_calledPlayersIndex.erase(playerIndex);	//����call������ɾ��

	escapedPlayer = player();		//����һ����player����

	if (this->m_calledPlayersIndex.size() <= 1) {	//û���ˣ�����
		this->updatePots();										//���µ׳���߳�

		this->settle();
		//Ȼ����Ⱦ�������ֻ��棬����ÿλ��ҵ��ٴο�ʼ����show�ơ����ͣ��ٴο�ʼ������һ����Ϸ
		this->showGameEnd();
		this->setGameHasStarted(false);	//��Ϸ��������Ϸ״̬����Ϊδ��ʼ
		return;
	}
	//this->afterPlayerAction();	//����Ⱦ�˰�
}
void game::afterPlayerAction(){									//����ж���
	this->nowPlayerActionComplete();	//��ǰ��ҽ����ж���Ⱦ
	if (this->m_nowPlayerIndex == this->m_endPlayerIndex || this->m_calledPlayersIndex.size() <= 1) {		//���ֽ���
		this->updatePots();										//���µ׳���߳�
		this->finishThisRound();								//���ֽ��������Ⱦ
		this->nextRound();										//��һ��
	}
	else {
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//��һλ���
		this->nowPlayerRender();															//��Ⱦ��һλ��ҵĽ���
	}
}

void game::calCardTypeAndPointForAll() {
	for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
		int playerIndex = *playerIndexIterator;
		calCardTypeAndPointForPlayer(playerIndex);
	}
}

cardTypeAndPoint game::calCardTypeAndPointForPlayer(const int playerIndex) {
	player& player = this->getPlayer(playerIndex);
	vector<card> const& handCards = player.getHandCards();
	vector<card> const& commonCards = this->getCommonCards();
	card combine[5];
	for (int i = 0; i < 21; ++i) {
		for (int j = 0; j < 5; ++j) {
			//��ȡ���
			if (combineMap[i][j] >= 5)
				combine[j] = handCards[combineMap[i][j] - 5];
			else
				combine[j] = commonCards[combineMap[i][j]];
			//����
			cardTypeAndPoint nowCardTypeAndPoint = cardTypeAndPoint::calTypeAndPoint(combine);
			if (nowCardTypeAndPoint > player.getCardTypeAndPoint())
				player.setCardTypeAndPoint(nowCardTypeAndPoint);
		}
	}
	return player.getCardTypeAndPoint();
}


void game::nextRound() {
	if (this->m_calledPlayersIndex.size()<=1||this->m_round >= gameRound::River) {	//û���˻������һ�ֽ������ˣ�����
		this->settle();
		//Ȼ����Ⱦ�������ֻ��棬����ÿλ��ҵ��ٴο�ʼ����show�ơ����ͣ��ٴο�ʼ������һ����Ϸ
		this->showGameEnd();
		this->setGameHasStarted(false);	//��Ϸ��������Ϸ״̬����Ϊδ��ʼ
		return;
	}
	this->m_round = gameRound(this->m_round + 1);
	this->sendCardsToTable();
	this->m_nowPlayerIndex = this->getRoundBeginPlayerIndex();			//��һ�ֿ�ʼ����ң����������Ϊdealer����������
	this->m_endPlayerIndex = this->getRoundEndPlayerIndex();			//��һ�ֽ��������
	this->renderGame();													//��Ϊ�·����ˣ�����Ҫ������Ⱦ
	if (this->m_round == PreFlop) {										//��ǰ��äע
		//��ǰ��ÿ���ͻ��˶���Ҫshow���г��Լ������ڳ���ҵĿ���
		this->showOthersCardBackOnAllClient();

		this->getPlayer(this->m_nowPlayerIndex).add(smallBind);			//Сä
		this->getPlayer(this->m_nowPlayerIndex).setPlayerAction(actionType::Raise);
		this->nowPlayerActionComplete();
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);

		this->getPlayer(this->m_nowPlayerIndex).add(bigBind);			//��ä
		this->getPlayer(this->m_nowPlayerIndex).setPlayerAction(actionType::Raise);
		this->nowPlayerActionComplete();
		this->m_endPlayerIndex = this->m_nowPlayerIndex;				//��һ�ֵ���ä���� !!!!!��û�����⣿
		this->m_minBet = bigBind;
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//��ä��һλ��ʼ
	}
	this->nowPlayerRender();		//��Ⱦ��ǰ�ж����
	return;
}

//void game::renderGame()
//{
//}
//
//void game::nowPlayerActionComplete()
//{
//}
//
//void game::finishThisRound()
//{
//}
//bool game::nowPlayerRender() {
//	//��ǰ����Ѿ�allin����ʲô����������������Ⱦ����ֱ�ӵ����ж����
//	if (this->m_players[this->m_nowPlayerIndex].hasAllin()) {
//		this->afterPlayerAction();
//		return true;
//	}
//	//��Ϊ��ʱ�ǵ������ԣ��������������Ⱦ����ǰplayer�İ���
//	{
//
//	}
//	//������server�Ļ���������
//	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
//	//nowPlayer.enableOperation();	//client������ʾ����
//	return true;
//}

void game::renderGame() {
	//������Ⱦ����
	{
		vector<card> const& commonCards = this->getCommonCards();	//������
		this->showCommonCards();
		this->showRound();	//�ִ�
		this->showPot();
	}
	//ÿλ���
	{
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			player const& curPlayer = this->getPlayer(i_player);
			if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {	//����������ֳ�����ƶ���ʾ
				this->hidePlayerAllAction(i_player);
				this->hidePlayerActionMessage(i_player);
				this->showPlayerNameCardsChip(i_player);
			}
			else if (curPlayer.getPlayerType() == playerType::Looker) {		//�Թ���ֻ��ʾ���ֺͳ���
				this->hidePlayerAllAction(i_player);
				this->hidePlayerActionMessage(i_player);
				this->hidePlayerHandCards(i_player);
				this->showPlayerName(i_player);
				this->showPlayerChip(i_player);
			}
			else if (curPlayer.getPlayerType() == playerType::Empty) {		//����������
				this->hidePlayer(i_player);
			}
			else {}
		}
	}
}

void game::hideNowPlayerAllAction() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	this->hidePlayerAllAction(nowPlayerIndex);
}

void game::showPlayer1CardsOnPlayer2(const int player1Index, const int player2Index)
{
	vector<card> handCards = this->m_players[player1Index].getHandCards();
	this->m_ui->showPlayer1HandCardOnPlayer2(player1Index, player2Index, handCards);
}

void game::showOthersCardBackOnPlayerIndex(const int playerIndex)
{
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {	//�ڳ�������Ҫshow�������ú���ֻӦ��beginʱ����
		if (this->m_players[i_player].getPlayerType() == playerType::OnSitePlayer && i_player != playerIndex) {
			this->m_ui->showPlayer1CardBackOnPlayer2(i_player, playerIndex);
		}
	}
}

void game::showOthersCardBackOnAllClient()
{
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getPlayerType() != playerType::Empty) {	//�Թ���ҲҪshow����
			this->showOthersCardBackOnPlayerIndex(i_player);
		}
	}
}

void game::showAllBegin()
{
	for (int i = 0; i < game::maxNumOfPlayers; ++i) {
		if (this->m_players[i].getPlayerType() == playerType::OnSitePlayer)
			this->m_ui->showBegin(i);
	}
}

void game::hideAllBegin()
{
	for (int i = 0; i < game::maxNumOfPlayers; ++i) {
		if (this->m_players[i].getPlayerType() == playerType::OnSitePlayer)
			this->m_ui->hideBegin(i);
	}
}

void game::showGameEnd()
{
	//��Ϸ����
	//���Ӯ�ý���
	for (int i_player : this->m_calledPlayersIndex) {
		player const& curPlayer = this->getPlayer(i_player);
		const int winMoney = curPlayer.getWinMoney();
		string outputMessage = "";
		if (this->m_calledPlayersIndex.size() > 1) {
			const cardType cardType = curPlayer.getCardTypeAndPoint().getCardType();
			outputMessage += "���ͣ�" + to_string(cardType) + "\n";
		}
		if (winMoney > 0) {		//���Ӯ��
			outputMessage += "���Ӯ�ã�" + to_string(winMoney);
		}
		else {
			outputMessage += "�������";
		}
		this->showPlayerMessage(i_player, outputMessage);

		if (this->m_calledPlayersIndex.size() > 1) {	//�����ƻ�ʤ����show
			//show�ڳ���ҵ��ƣ��ǿվ�show
			for (int i_player2 = 0; i_player2 < this->m_players.size(); i_player2++) {
				if (i_player2 != i_player && this->m_players[i_player2].getPlayerType() != playerType::Empty) {
					this->showPlayer1CardsOnPlayer2(i_player, i_player2);
				}
			}
			//for (int i_player2 : this->m_calledPlayersIndex) {
			//	if (i_player != i_player2) {
			//	}
			//}
		}
	}
	//��ʼ��������
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getPlayerType() == playerType::Looker&&this->m_players[i_player].getChip() > 0) {
			this->m_players[i_player].setPlayerType(playerType::OnSitePlayer);	//�ڳ���looker����Ǯ�ı�Ϊonsite
		}
	}
	this->showAllBegin();
}

void game::updatePlayerScore(const int playerIndex) {
	const string macAddress = this->getPlayerMacAddress(playerIndex);
	auto iter = this->m_macAddressToScoreChartIndex.find(macAddress);
	if (iter == this->m_macAddressToScoreChartIndex.end()) {		//û�ҵ�
		this->m_ui->showPlayerActionMessage(this->getNowPlayerIndex(), "���¼Ʒֱ�ʧ��");
		return;
	}
	const int playerRow = iter->second;													//����ڱ����к�
	const int playerChip = this->getPlayer(playerIndex).getChip();						//��ҵ�ǰ���ϳ���
	const int totalBuyIn = this->m_scoreChart->item(playerRow, 1)->text().toInt();		//���������
	int totalChip = this->m_scoreChart->item(playerRow, 2)->text().toInt();				//����ܳ���
	const int onTableChip = this->m_scoreChart->item(playerRow, 3)->text().toInt();		//�ϴθ��µģ������ϵĳ���

	const int modifiedChip = playerChip - onTableChip;									//��ǰ���ϳ��룬���ϴ����ϳ���֮��
	totalChip += modifiedChip;															//�����ܳ���
	const int totalWin = totalChip - totalBuyIn;										//������ӯ��
	this->m_scoreChart->setItem(playerRow, 2, new QStandardItem(QString::number(totalChip)));
	this->m_scoreChart->setItem(playerRow, 3, new QStandardItem(QString::number(playerChip)));
	this->m_scoreChart->setItem(playerRow, 4, new QStandardItem(QString::number(totalWin)));
}
//������һ����ֻ�����ϳ����޸Ĳ�һ��������
void game::updateEscapedPlayerScore(const int playerIndex) {
	const string macAddress = this->getPlayerMacAddress(playerIndex);
	auto iter = this->m_macAddressToScoreChartIndex.find(macAddress);
	if (iter == this->m_macAddressToScoreChartIndex.end()) {		//û�ҵ�
		this->m_ui->showPlayerActionMessage(this->getNowPlayerIndex(), "���¼Ʒֱ�ʧ��");
		return;
	}
	const int playerRow = iter->second;													//����ڱ����к�
	const int playerChip = this->getPlayer(playerIndex).getChip();						//��ҵ�ǰ���ϳ���
	const int totalBuyIn = this->m_scoreChart->item(playerRow, 1)->text().toInt();		//���������
	int totalChip = this->m_scoreChart->item(playerRow, 2)->text().toInt();				//����ܳ���
	const int onTableChip = this->m_scoreChart->item(playerRow, 3)->text().toInt();		//�ϴθ��µģ������ϵĳ���

	const int modifiedChip = playerChip - onTableChip;									//��ǰ���ϳ��룬���ϴ����ϳ���֮��
	totalChip += modifiedChip;															//�����ܳ���
	const int totalWin = totalChip - totalBuyIn;										//������ӯ��
	this->m_scoreChart->setItem(playerRow, 2, new QStandardItem(QString::number(totalChip)));
	this->m_scoreChart->setItem(playerRow, 3, new QStandardItem(QString::number(0)));	//���ϳ����Ϊ0
	this->m_scoreChart->setItem(playerRow, 4, new QStandardItem(QString::number(totalWin)));
}
void game::nowPlayerActionComplete() {
	this->hideNowPlayerAllAction();		//���ص�ǰ����ж�����
	this->showNowPlayerChip();			//��ʾ��ǰ��ҳ�����Ϣ
	this->showNowPlayerActionMessage();	//��ǰ����ж���Ϣ
}

void game::finishThisRound() {
	this->showPot();	//չʾ�׳�
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		player& curPlayer = this->getPlayer(i_player);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			this->hidePlayerActionMessage(i_player);
			if (curPlayer.getPlayerAction() != actionType::Allin) {		//���ڷ�allin����ң��趨�ж�ΪʲôҲû�����������ж�
				curPlayer.setPlayerAction(actionType::Nothing);
			}
		}
	}
}

bool game::nowPlayerRender() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	player const& nowPlayer = this->getPlayer(nowPlayerIndex);
	//this->hidePlayerActionMessage(nowPlayerIndex);		//Ӧ�õ����ó�ȥ��Ϊһ������
	this->showPlayerMessage(nowPlayerIndex, "����ж���...");	//��ʾ��ǰ�ж����

	if (nowPlayer.hasAllin()) {
		this->afterPlayerAction();
		return true;
	}
	const int minBet = this->getMinBet();
	const int playerNowBet = nowPlayer.getNowBet();
	const int playerChip = nowPlayer.getChip();
	if (minBet <= playerNowBet) {	//��ǰ��עС�ڱ�������ע
		this->showPlayerCheckRaiseFoldAction(nowPlayerIndex);
	}
	else if (minBet >= playerChip) {	//��Ҫallin
		this->showPlayerAllinFoldAction(nowPlayerIndex);
	}
	else if (minBet < playerChip && minBet > playerNowBet) {	//��ǰ��С��ע��������ע
		this->showPlayerCallRaiseFoldAction(nowPlayerIndex);
	}
	else {}
	return true;
}


bool game::begin() {
	//�ȼ����ǲ���������Ҷ�׼����
	const int numOfPlayer = this->getNumOfPlayers();
	if (this->m_readyNumOfPlayer < numOfPlayer) {
		return false;
	}

	//this->hideBegin();
	this->m_calledPlayersIndex.clear();
	for (int playerIndex = 0; playerIndex < maxNumOfPlayers; ++playerIndex) {
		//if (this->m_players[playerIndex].getPlayerType() == playerType::Looker && this->m_players[playerIndex].getChip() > 0) {
		//	this->m_players[playerIndex].setPlayerType(playerType::OnSitePlayer);	//��Ǯ���Թ����ϳ�
		//}
		if (this->m_players[playerIndex].getPlayerType() == playerType::OnSitePlayer) {
			this->m_calledPlayersIndex.insert(playerIndex);
		}
		this->m_players[playerIndex].initPlayerState();		//��ʼ�����״̬
	}
	if (this->m_calledPlayersIndex.size() <= 1)	//С�ڵ���1���˲�����
		return false;

	this->m_readyNumOfPlayer = 0;

	this->m_round = Start;		//��ʼ
	this->setGameHasStarted(true);	//��Ϸ״̬��Ϊ��ʼ
	this->updateDealer();		//����dealer
	this->showDealer();			//�ڿͻ�����ʾdealer

	//this->initPlayersState();	//��ʼ�����״̬
	this->clearCommonCards();	//������ϵ���
	this->clearPot();
	this->clearSidePot();		//��ձ߳�
	this->hideAllPlayerSidePot();	//���ر߳أ����Բ�д��
	this->shuffleCardHeap();	//ϴ��
	this->renderGame();			//��Ⱦ��Ϸ����
	this->nextRound();			//������һ��
	return true;
}


//bool game::begin() {
//	const int numOfPlayers = this->getCalledPlayersIndex().size();
//	if (numOfPlayers <= 1) {
//		return false;
//	}
//	
//	//��ʼ��Ϸ
//	this->initPlayersState();	//��ʼ�����״̬
//	this->m_commonCards.clear();//������ϵ���
//	this->shuffleCardHeap();	//ϴ��
//	//sendCardsToPlayers();		//����ҷ���
//	for (this->m_round = PreFlop; this->m_round <= River; this->m_round = gameRound(this->m_round + 1)) {
//		this->sendCardsToTable();	//���ƣ��������� ����ҷ���
//		this->playersAction();		//����ж�
//		this->updatePot();			//���µ׳�
//		if (this->getCalledPlayersIndex().size() == 1) {
//			break;
//		}
//	}
//	this->settle();	//����
//
//	return true;
//}
void game::playerWin(int winnerIndex){	//����������ȾӮ�˵ĳ���
	//����

	player& winnerPlayer = this->getPlayer(winnerIndex);
	winnerPlayer.win();
}

void game::playerLose(int loserIndex){	//����������ȾӮ�˵ĳ���
	//����
	player& losePlayer = this->getPlayer(loserIndex);
	losePlayer.lose();
}

vector<int> game::getMaxPlayersIndex(){
	vector<int> res;
	for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
		int playerIndex = *playerIndexIterator;
		if (res.empty())
			res.push_back(playerIndex);
		else {
			cardTypeAndPoint curCardTypeAndPoint = this->getPlayer(playerIndex).getCardTypeAndPoint();
			cardTypeAndPoint resCardTypeAndPoint = this->getPlayer(res[0]).getCardTypeAndPoint();
			if (curCardTypeAndPoint > resCardTypeAndPoint) {
				res.clear();
				res.push_back(playerIndex);
			}
			else if (curCardTypeAndPoint == resCardTypeAndPoint) {
				res.push_back(playerIndex);
			}
		}
	}

	//���վ���dealer����Զ����ǰ����֪���Բ���
	sort(res.begin(), res.end(), [&](int playerIndex1, int playerIndex2) {return this->m_dealer + maxNumOfPlayers - playerIndex1 > this->m_dealer + maxNumOfPlayers - playerIndex2; });
	return res;
}

vector<int> game::getMaxPlayersIndex(vector<int> const & curPlayerIndexSet){
	vector<int> res;
	for (int playerIndex: curPlayerIndexSet) {
		if (res.empty())
			res.push_back(playerIndex);
		else {
			cardTypeAndPoint curCardTypeAndPoint = this->getPlayer(playerIndex).getCardTypeAndPoint();
			cardTypeAndPoint resCardTypeAndPoint = this->getPlayer(res[0]).getCardTypeAndPoint();
			if (curCardTypeAndPoint > resCardTypeAndPoint) {
				res.clear();
				res.push_back(playerIndex);
			}
			else if (curCardTypeAndPoint == resCardTypeAndPoint) {
				res.push_back(playerIndex);
			}
		}
	}

	//���վ���dealer����Զ����ǰ����֪���Բ���
	sort(res.begin(), res.end(), [&](int playerIndex1, int playerIndex2) {return this->m_dealer + maxNumOfPlayers - playerIndex1 > this->m_dealer + maxNumOfPlayers - playerIndex2; });
	return res;
}

void game::settle(){		//����
	if (this->m_calledPlayersIndex.size() > 1)
		this->calCardTypeAndPointForAll();
	//�׳ؽ��㣬��allin�Ĳ����㣬�ȱ߳�
	{
		vector<int> participateInPot;
		for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
			int playerIndex = *playerIndexIterator;
			player& curPlayer = this->getPlayer(playerIndex);
			
			if (!curPlayer.hasAllin())
				participateInPot.push_back(playerIndex);
		}
		if (!participateInPot.empty()) {
			vector<int> winPlayerIndexesInPot = this->getMaxPlayersIndex(participateInPot);
			int winMoney = this->m_pot / (winPlayerIndexesInPot.size());
			int remainderMoney = this->m_pot % (winPlayerIndexesInPot.size());
			for (auto winPlayerIndex : winPlayerIndexesInPot) {
				this->getPlayer(winPlayerIndex).addToWinMoney(winMoney);
				if (remainderMoney-- > 0)
					this->getPlayer(winPlayerIndex).addToWinMoney(1);
			}
		}
	}

	//�߳ؽ���
	for (auto const& sidePot : this->m_sidePots) {
		int sidePotMoney = sidePot.getSidePotMoney();									//�����У�Ҫ��Ҫ�ĳɺ����أ��Ǿ͵�дһ���߳���
		vector<int> const& participateInSidePot = sidePot.getSidePotParticipatIndex();	//������
		if (!participateInSidePot.empty()) {
			vector<int> winPlayerIndexesInSidePot = this->getMaxPlayersIndex(participateInSidePot);
			int winMoney = sidePotMoney / (winPlayerIndexesInSidePot.size());
			int remainderMoney = sidePotMoney % (winPlayerIndexesInSidePot.size());
			for (auto winPlayerIndex : winPlayerIndexesInSidePot) {
				this->getPlayer(winPlayerIndex).addToWinMoney(winMoney);
				if (remainderMoney-- > 0)
					this->getPlayer(winPlayerIndex).addToWinMoney(1);
			}
		}
	}
	//����Լ�����
	for (int i = 0; i < maxNumOfPlayers; ++i) {
		player& curPlayer = this->getPlayer(i);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			if (curPlayer.getWinMoney() > 0)
				this->playerWin(i);
			else
				this->playerLose(i);
			//�����޸ļƷֱ�
			this->updatePlayerScore(i);
		}
	}
}

void game::updatePots(){	//���µ׳���߳�
	//�Ȱ��յ�ǰ����ע���򣬴�С����
	vector<int> playerNowBetSortedIndexes;
	//for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
	//	int curPlayerIndex = *playerIndexIterator;
	//	playerNowBetSortedIndexes.push_back(curPlayerIndex);
	//}
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getNowBet() > 0) {				//ֻҪ��ע�˾ͼӽ�ȥ�������Ƿ�����
			playerNowBetSortedIndexes.push_back(i_player);
		}
	}
	sort(playerNowBetSortedIndexes.begin(), playerNowBetSortedIndexes.end(), [&](int index1, int index2) {
		if (this->getPlayer(index1).getNowBet() == this->getPlayer(index2).getNowBet())
			return this->getPlayer(index1).getChip() < this->getPlayer(index2).getChip();	//bet��ͬ�ģ�����С�ķ�ǰ�棬�����������ҵ�allin����
		return this->getPlayer(index1).getNowBet() < this->getPlayer(index2).getNowBet(); });
	int hasInSidePot = 0;
	for (int i = 0; i < (int)playerNowBetSortedIndexes.size(); ++i) {
		int curPlayerIndex = playerNowBetSortedIndexes[i];
		player& curPlayer = this->getPlayer(curPlayerIndex);
		if (curPlayer.isAllinThisRound()) {	//��ǰ��allin�ˣ�allin�ز�������fold
			int moneyPerPlayer = curPlayer.getNowBet();
			int newSidePotIndex = this->addNewSidePot((moneyPerPlayer - hasInSidePot)*(playerNowBetSortedIndexes.size() - i) + this->m_pot);//���ñ߳أ�һ����Ϸ����for�и���һ�μ���
			this->m_pot = 0;
			for (int j = i; j < (int)playerNowBetSortedIndexes.size(); ++j) {		//���Ϳ������fold���ˣ��籾��ûfold����fold��
				this->addPlayerToLastSidePot(newSidePotIndex, playerNowBetSortedIndexes[j]);												//��ӱ߳���Ա
			}
			hasInSidePot = moneyPerPlayer;

			this->showPlayerSidePot(curPlayerIndex, this->m_sidePots[newSidePotIndex].getSidePotMoney());		//��Ⱦ�߳أ����Բ��ӣ�����0���ж��ڵײ�
		}
		else {	//����һ������allin ����������allin������ע�Ķ���Ҫ���ǣ��������Ƶģ����Ժ���������allin������1 allin�� 2 raise�� 3 raise�� 2��fold��
			this->m_pot += curPlayer.getNowBet() - hasInSidePot;	//��allin�ķŽ��׳���
		}
		curPlayer.setNowBet(0);		//����Ҵ�����ע�ѽ���׳ػ�߳أ���Ϊ0
	}
	this->setMinBet(0);	//��СbetΪ0
}

void game::showPlayerChip(const int playerIndex)
{
	const int chip = this->getPlayer(playerIndex).getChip();
	this->m_ui->showPlayerChip(playerIndex, chip);
}

void game::showPlayerName(const int playerIndex) {
	string const& name = this->getPlayer(playerIndex).getName();
	this->m_ui->showPlayerName(playerIndex, name);
}

void game::showPlayerHandCards(const int playerIndex) {
	vector<card> const& cards = this->getPlayer(playerIndex).getHandCards();
	this->m_ui->showPlayerHandCards(playerIndex, cards);
}

void game::showPlayerActionMessage(const int playerIndex)
{
	player const& nowPlayer = this->getPlayer(playerIndex);
	if (nowPlayer.getPlayerAction() == actionType::Nothing) {
		this->m_ui->hidePlayerActionMessage(playerIndex);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Allin) {
		const int allinMoney = nowPlayer.getNowBet();
		string actionMessage = "allin";
		if (allinMoney > 0)
			actionMessage += ":" + to_string(allinMoney);
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Call) {
		const int callMoney = nowPlayer.getNowBet();
		string actionMessage = "��ע��" + to_string(callMoney);
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Check) {
		string actionMessage = "����";
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Raise) {
		const int raiseMoney = nowPlayer.getNowBet();
		string actionMessage = "��ע����" + to_string(raiseMoney);
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Fold) {
		string actionMessage = "����";
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::ErrorAction) {

	}
	else {}
}

void game::showPlayerSidePot(const int playerIndex, const int money)
{
	this->m_ui->showPlayerSidePot(playerIndex, money);
}

void game::showPlayerMessage(const int playerIndex, string const & message)
{
	this->m_ui->showPlayerActionMessage(playerIndex, message);
}

void game::showPlayerRaiseAction(const int playerIndex)
{
	player const& curPlayer = this->getPlayer(playerIndex);
	const int maxMoney = curPlayer.getChip();
	const int minMoney = this->m_minBet + game::bigBind;			//+��äΪ��С��ע
	this->m_ui->showPlayerRaiseAction(playerIndex, minMoney, maxMoney);
}

void game::showPlayerAllinAction(const int playerIndex)
{
	player const& curPlayer = this->getPlayer(playerIndex);
	const int maxMoney = curPlayer.getChip();
	this->m_ui->showPlayerAllinAction(playerIndex, maxMoney);
}

void game::showPlayerCheckAction(const int playerIndex)
{
	this->m_ui->showPlayerCheckAction(playerIndex);
}

void game::showPlayerCallAction(const int playerIndex)
{
	const int callMoney = this->m_minBet;
	this->m_ui->showPlayerCallAction(playerIndex, callMoney);
}

void game::showPlayerFoldAction(const int playerIndex)
{
	this->m_ui->showPlayerFoldAction(playerIndex);
}

void game::showPlayerNameCardsChip(const int playerIndex)
{
	this->showPlayerName(playerIndex);
	this->showPlayerHandCards(playerIndex);
	this->showPlayerChip(playerIndex);
}

void game::showPlayerCheckRaiseFoldAction(const int playerIndex)
{
	this->showPlayerCheckAction(playerIndex);
	this->showPlayerRaiseAction(playerIndex);
	this->showPlayerFoldAction(playerIndex);
}

void game::showPlayerAllinFoldAction(const int playerIndex)
{
	this->showPlayerAllinAction(playerIndex);
	this->showPlayerFoldAction(playerIndex);
}

void game::showPlayerCallRaiseFoldAction(const int playerIndex)
{
	this->showPlayerCallAction(playerIndex);
	this->showPlayerRaiseAction(playerIndex);
	this->showPlayerFoldAction(playerIndex);
}

void game::hidePlayerAllAction(const int playerIndex)
{
	this->hidePlayerRaiseAction(playerIndex);
	this->hidePlayerAllinAction(playerIndex);
	this->hidePlayerCheckAction(playerIndex);
	this->hidePlayerCallAction(playerIndex);
	this->hidePlayerFoldAction(playerIndex);
}

void game::hidePlayer(const int playerIndex)
{
	this->hidePlayerAllAction(playerIndex);
	this->hidePlayerName(playerIndex);
	this->hidePlayerChip(playerIndex);
	this->hidePlayerHandCards(playerIndex);
	this->hidePlayerActionMessage(playerIndex);
}

void virUI::showPlayer1CardBackOnPlayer2(const int player1Index, const int player2Index) const
{
	vector<card> cardBacks(player::numOfHandCards, card(cardColor::CardBackColor, cardNumber::CardBackNumber));
	this->showPlayer1HandCardOnPlayer2(player1Index, player2Index, cardBacks);
}
