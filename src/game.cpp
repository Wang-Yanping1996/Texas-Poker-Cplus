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
	//��д�ˣ�dealerǰһ����Ч��ҵĺ�һ��
	int preCalledPlayerIndex = this->getPreCalledPlayerIndex(this->m_dealer);
	return this->getNextCalledPlayerIndex(preCalledPlayerIndex);
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
	else if (this->m_round == Flop) {	//����
		this->m_commonCards[0] = this->m_cardHeap.getCard();
		this->m_commonCards[1] = this->m_cardHeap.getCard();
		this->m_commonCards[2] = this->m_cardHeap.getCard();
	}
	else if (this->m_round == Turn) {	//ת��
		this->m_commonCards[3] = this->m_cardHeap.getCard();
	}
	else if (this->m_round == River) {	//����
		this->m_commonCards[4] = this->m_cardHeap.getCard();
	}
}

//�ĸ���ǰ��ҵĶ��������Խ���Ұ���ȫ��������
void game::nowPlayerRaise(const int raiseTo) {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(raiseTo);
	//���µ�ǰ��Һͽ�����ң���Ϊraise�ˣ���ȻҪ��һ�֣����Բ��ж��Ƿ������һ��
	//this->afterPlayerAction();		//���Ա���ɾ��
	this->nowPlayerActionComplete();	//��ǰ��ҽ����ж���Ⱦ
	this->m_endPlayerIndex = this->m_nowPlayerIndex;				
	this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//��һλ���
	this->nowPlayerRender();															//��Ⱦ��һλ��ҵĽ���
}
void game::nowPlayerCall() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(this->m_minBet);		//������Ҽӵ���Сbet
	this->afterPlayerAction();
}
void game::nowPlayerCheck() {
	//player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	//nowPlayer.add(0);		//�ӵ�0������ɾ��
	this->afterPlayerAction();
}
void game::nowPlayerFold() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.fold();
	this->m_calledPlayersIndex.erase(this->m_nowPlayerIndex);	//����call������ɾ��
	this->afterPlayerAction();
}
void game::afterPlayerAction(){									//����ж���
	this->nowPlayerActionComplete();	//��ǰ��ҽ����ж���Ⱦ
	if (this->m_nowPlayerIndex == this->m_endPlayerIndex) {		//���ֽ���
		this->finishThisRound();								//���ֽ��������Ⱦ
		this->updatePots();										//���µ׳���߳�
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

void game::calCardTypeAndPointForPlayer(const int playerIndex) {
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
}


void game::nextRound() {
	if (this->m_calledPlayersIndex.size()<=1||this->m_round >= End) {	//û���˻������һ�ֽ������ˣ�����
		this->settle();
		//Ȼ����Ⱦ�������ֻ��棬����ÿλ��ҵ��ٴο�ʼ����show�ơ����ͣ��ٴο�ʼ������һ����Ϸ
		return;
	}
	this->m_round = gameRound(this->m_round + 1);
	this->sendCardsToTable();
	this->m_nowPlayerIndex = this->getRoundBeginPlayerIndex();			//��һ�ֿ�ʼ����ң����������Ϊdealer����������
	this->m_endPlayerIndex = this->getRoundEndPlayerIndex();			//��һ�ֽ��������
	if (this->m_round == PreFlop) {	//��ǰ��äע
		this->getPlayer(this->m_nowPlayerIndex).add(smallBind);			//Сä
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);
		this->getPlayer(this->m_nowPlayerIndex).add(bigBind);			//��ä
		this->m_endPlayerIndex = this->m_nowPlayerIndex;				//��һ�ֵ���ä���� !!!!!��û�����⣿
		this->m_minBet = bigBind;
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//��ä��һλ��ʼ
	}
	this->renderGame();				//��Ϊ�·����ˣ�����Ҫ������Ⱦ
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
		this->m_ui->showCommonCards(commonCards);
		this->m_ui->showRound(this->getGameRound());	//�ִ�
																	//if (commonCards[0].isCardVailid() && commonCards[1].isCardVailid() && commonCards[2].isCardVailid()) {	//����
																	//}
	}
	//ÿλ���
	{
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			player const& curPlayer = this->getPlayer(i_player);
			if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {	//����������ֳ�����ƶ���ʾ
				this->m_ui->hidePlayerAction(i_player);
				this->m_ui->hidePlayerActionMessage(i_player);
				this->m_ui->showPlayerNameCardsChip(i_player, curPlayer);
			}
			else if (curPlayer.getPlayerType() == playerType::Looker) {		//�Թ���ֻ��ʾ���ֺͳ���
				this->m_ui->hidePlayerAction(i_player);
				this->m_ui->hidePlayerActionMessage(i_player);
				this->m_ui->hidePlayerHandCards(i_player);
				this->m_ui->showPlayerName(i_player, curPlayer.getName());
				this->m_ui->showPlayerChip(i_player, curPlayer.getChip());
			}
			else if (curPlayer.getPlayerType() == playerType::Empty) {		//����������
				this->m_ui->hidePlayer(i_player);
			}
			else {}
		}
	}
}

void game::hideNowPlayerAction() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	this->m_ui->hidePlayerAction(nowPlayerIndex);
}

void game::showNowPlayerActionMessage(string const& actionMessage) {
	this->m_ui->showPlayerActionMessage(this->getNowPlayerIndex(), actionMessage);
}

void game::nowPlayerActionComplete() {
	this->hideNowPlayerAction();		//���ص�ǰ����ж�����
										//��ǰ����ж���Ϣ
	const int nowPlayerIndex = this->getNowPlayerIndex();
	player const& nowPlayer = this->getPlayer(nowPlayerIndex);
	if (nowPlayer.getPlayerAction() == actionType::Nothing) {
		this->m_ui->hidePlayerActionMessage(nowPlayerIndex);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Allin) {
		const int allinMoney = nowPlayer.getNowBet();
		string actionMessage = "allin";
		if (allinMoney > 0)
			actionMessage += ":" + to_string(allinMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Call) {
		const int callMoney = nowPlayer.getNowBet();
		string actionMessage = "��ע��" + to_string(callMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Check) {
		string actionMessage = "����";
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Raise) {
		const int raiseMoney = nowPlayer.getNowBet();
		string actionMessage = "��ע����" + to_string(raiseMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Fold) {
		string actionMessage = "����";
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::ErrorAction) {

	}
	else {}
}

void game::finishThisRound() {
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		player& curPlayer = this->getPlayer(i_player);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			this->m_ui->hidePlayerActionMessage(i_player);
			if (curPlayer.getPlayerAction() != actionType::Allin) {		//���ڷ�allin����ң��趨�ж�ΪʲôҲû�����������ж�
				curPlayer.setPlayerAction(actionType::Nothing);
			}
		}
	}
}

bool game::nowPlayerRender() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	player const& nowPlayer = this->getPlayer(nowPlayerIndex);
	if (nowPlayer.hasAllin()) {
		this->afterPlayerAction();
		return true;
	}
	const int minBet = this->getMinBet();
	const int playerNowBet = nowPlayer.getNowBet();
	const int playerChip = nowPlayer.getChip();
	if (minBet <= playerNowBet) {	//��ǰ��עС�ڱ�������ע
		this->m_ui->playerCheckRaiseFoldAction(nowPlayerIndex, game::bigBind, playerChip);
	}
	else if (minBet >= playerChip) {	//��Ҫallin
		this->m_ui->playerAllinFoldAction(nowPlayerIndex, playerChip);
	}
	else if (minBet < playerChip && minBet > playerNowBet) {	//��ǰ��С��ע��������ע
		this->m_ui->playerCallRaiseFoldAction(nowPlayerIndex, minBet - playerNowBet, minBet + this->game::bigBind, playerChip);
	}
	else {}
	return true;
}


bool game::begin() {
	this->m_calledPlayersIndex.clear();
	for (int playerIndex = 0; playerIndex < maxNumOfPlayers; ++playerIndex) {
		if (this->m_players[playerIndex].getPlayerType() == playerType::OnSitePlayer) {
			this->m_calledPlayersIndex.insert(playerIndex);
		}
		this->m_players[playerIndex].initPlayerState();		//��ʼ�����״̬
	}
	if (this->m_calledPlayersIndex.size() <= 1)	//С�ڵ���1���˲�����
		return false;

	this->m_round = Start;		//��ʼ
	this->updateDealer();		//����dealer
	//this->initPlayersState();	//��ʼ�����״̬
	this->clearCommonCards();	//������ϵ���
	this->clearSidePot();		//��ձ߳�
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
	
	//�׳ؽ��㣬��allin�Ĳ����㣬�ȱ߳�
	{
		vector<int> participateInPot;
		for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
			int playerIndex = *playerIndexIterator;
			player& curPlayer = this->getPlayer(playerIndex);
			if (!curPlayer.hasAllin())
				participateInPot.push_back(playerIndex);
		}
		vector<int> winPlayerIndexesInPot = this->getMaxPlayersIndex(participateInPot);
		int winMoney = this->m_pot / (winPlayerIndexesInPot.size());
		int remainderMoney = this->m_pot % (winPlayerIndexesInPot.size());
		for (auto winPlayerIndex : winPlayerIndexesInPot) {
			this->getPlayer(winPlayerIndex).addToWinMoney(winMoney);
			if (remainderMoney-- > 0)
				this->getPlayer(winPlayerIndex).addToWinMoney(1);
		}
	}

	//�߳ؽ���
	for (auto const& sidePot : this->m_sidePots) {
		int sidePotMoney = sidePot.getSidePotMoney();									//�����У�Ҫ��Ҫ�ĳɺ����أ��Ǿ͵�дһ���߳���
		vector<int> const& participateInSidePot = sidePot.getSidePotParticipatIndex();	//������
		vector<int> winPlayerIndexesInSidePot = this->getMaxPlayersIndex(participateInSidePot);
		int winMoney = sidePotMoney / (winPlayerIndexesInSidePot.size());
		int remainderMoney = sidePotMoney % (winPlayerIndexesInSidePot.size());
		for (auto winPlayerIndex : winPlayerIndexesInSidePot) {
			this->getPlayer(winPlayerIndex).addToWinMoney(winMoney);
			if (remainderMoney-- > 0)
				this->getPlayer(winPlayerIndex).addToWinMoney(1);
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
		}
	}
}

void game::updatePots(){	//���µ׳���߳�
	//�Ȱ��յ�ǰ����ע���򣬴�С����
	vector<int> playerNowBetSortedIndexes;
	for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
		int curPlayerIndex = *playerIndexIterator;
		playerNowBetSortedIndexes.push_back(curPlayerIndex);
	}
	sort(playerNowBetSortedIndexes.begin(), playerNowBetSortedIndexes.end(), [&](int index1, int index2) {
		if (this->getPlayer(index1).getNowBet() == this->getPlayer(index2).getNowBet())
			return this->getPlayer(index1).getChip() < this->getPlayer(index2).getChip();	//bet��ͬ�ģ�����С�ķ�ǰ�棬�����������ҵ�allin����
		return this->getPlayer(index1).getNowBet() < this->getPlayer(index2).getNowBet(); });
	int hasInSidePot = 0;
	for (int i = 0; i < (int)playerNowBetSortedIndexes.size(); ++i) {
		int curPlayerIndex = playerNowBetSortedIndexes[i];
		player& curPlayer = this->getPlayer(curPlayerIndex);
		if (curPlayer.isAllinThisRound()) {	//��ǰ��allin��
			int moneyPerPlayer = curPlayer.getNowBet();
			int newSidePotIndex = this->addNewSidePot((moneyPerPlayer - hasInSidePot)*(playerNowBetSortedIndexes.size() - i) + this->m_pot);//���ñ߳أ�һ����Ϸ����for�и���һ�μ���
			this->m_pot = 0;
			for (int j = i; j < (int)playerNowBetSortedIndexes.size(); ++j) {
				this->addPlayerToLastSidePot(newSidePotIndex, playerNowBetSortedIndexes[j]);												//��ӱ߳���Ա
			}
			hasInSidePot += moneyPerPlayer;
		}
		else {	//����һ������allin ����������allin
			this->m_pot += curPlayer.getNowBet() - hasInSidePot;	//��allin�ķŽ��׳���
		}
		curPlayer.setNowBet(0);		//����Ҵ�����ע�ѽ���׳ػ�߳أ���Ϊ0
	}
}
