#include "game.h"
#include <iostream>
#include <algorithm>
//int game::getNextPlayerIndex(int nowPlayerIndex)const {
//	auto nowPlayerIterator = this->m_calledPlayersIndex.find(nowPlayerIndex);
//	if (nowPlayerIterator == this->m_calledPlayersIndex.end()) {	//没找到当前玩家
//		return -1;
//	}
//	auto nextPlayerIterator = nowPlayerIterator++;
//	if (nextPlayerIterator == this->m_calledPlayersIndex.end()) {
//		nextPlayerIterator = this->m_calledPlayersIndex.begin();
//	}
//	return *nextPlayerIterator;
//}
int game::getNextCalledPlayerIndex(const int nowPlayerIndex)const {	//输入可以弃牌，输出还没弃牌的
	if (this->m_calledPlayersIndex.size() < 1)		//没有人
		return -1;
	if (this->m_calledPlayersIndex.size() == 1)		//仅有一个返回这个
		return *(this->m_calledPlayersIndex.begin());
	//包含了nowPlayerIndex弃牌的情况
	int nextCalledPlayerIndex = nowPlayerIndex;
	do {
		nextCalledPlayerIndex++;
		if (nextCalledPlayerIndex >= maxNumOfPlayers)
			nextCalledPlayerIndex -= maxNumOfPlayers;
	} while (this->m_calledPlayersIndex.find(nextCalledPlayerIndex) == this->m_calledPlayersIndex.end());	//找不到一直循环
	return nextCalledPlayerIndex;
}
int game::getPreCalledPlayerIndex(const int nowPlayerIndex) const{	//输入可以弃牌，输出还没弃牌的
	if (this->m_calledPlayersIndex.size() < 1)		//没有人
		return -1;
	if (this->m_calledPlayersIndex.size() == 1)		//仅有一个返回这个
		return *(this->m_calledPlayersIndex.begin());
	int preCalledPlayerIndex = nowPlayerIndex;
	do {
		preCalledPlayerIndex--;
		if (preCalledPlayerIndex < 0)
			preCalledPlayerIndex += maxNumOfPlayers;
	} while (this->m_calledPlayersIndex.find(preCalledPlayerIndex) == this->m_calledPlayersIndex.end());	//找不到一直循环
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
	//int beginPlayerIndex = this->m_dealer;				//从dealer后一位开始
	//do {
	//	beginPlayerIndex++;								//先++
	//	if (beginPlayerIndex >= maxNumOfPlayers)		//循环	
	//		beginPlayerIndex -= maxNumOfPlayers;
	//	if (this->m_calledPlayersIndex.find(beginPlayerIndex) != this->m_calledPlayersIndex.end()) 
	//		return beginPlayerIndex;					//找到了，退出
	//} while (beginPlayerIndex != this->m_dealer);		//找了一圈还没找到，错误
	//return -1;

	//重写了
	return this->getNextCalledPlayerIndex(this->m_dealer);	//直接获取dealer下一个
}
int game::getRoundEndPlayerIndex() const {
	//重写了，dealer在就是dealer，不在就是前一个有效的
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
	if (this->m_hasStarted)	//游戏已开始，进入者为旁观者
		this->m_players[addPlayerIndex] = player(playerType::Looker, playerName, { card(), card() }, this->initChip);
	else					//游戏未开始，进入者直接上场
		this->m_players[addPlayerIndex] = player(playerType::OnSitePlayer, playerName, { card(), card() }, this->initChip);
	return addPlayerIndex;
}

//bool game::playersAction() {			//要不要考虑把m_calledPlayersIndex换成引用或指针的set，感觉更合理一点？
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
//	//以后要改
//	int actionType = -1;	
//	cout << "玩家" << playerIndex << "行动：(1 raise, 2 check, 3 fold)" << endl;
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
	if (this->m_round == PreFlop) {	//翻前
		this->sendCardsToPlayers();
	}
	else if (this->m_round == Flop) {	//翻牌，发三张
		//this->m_commonCards[0] = this->m_cardHeap.getCard();
		//this->m_commonCards[1] = this->m_cardHeap.getCard();
		//this->m_commonCards[2] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
	else if (this->m_round == Turn) {	//转牌，发一张
		//this->m_commonCards[3] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
	else if (this->m_round == River) {	//河牌，发一张
		//this->m_commonCards[4] = this->m_cardHeap.getCard();
		this->m_commonCards.push_back(this->m_cardHeap.getCard());
	}
}

//四个当前玩家的动作，可以将玩家按键全部绑定至此
void game::nowPlayerRaise(const int raiseTo) {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	if (raiseTo >= nowPlayer.getChip() + nowPlayer.getNowBet()) {	//是否是allin
		this->nowPlayerAllin();
		return;
	}
	nowPlayer.add(raiseTo);
	nowPlayer.setPlayerAction(actionType::Raise);
	this->setMinBet(raiseTo);			//更新现在最小下注
	//更新当前玩家和结束玩家，因为raise了，必然要走一轮，所以不判断是否进入下一轮
	//this->afterPlayerAction();		//所以本行删掉
	//this->nowPlayerActionComplete();	//当前玩家结束行动渲染
	this->m_endPlayerIndex = this->getPreCalledPlayerIndex(this->m_nowPlayerIndex);
	this->afterPlayerAction();
	//this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//下一位玩家
	//this->nowPlayerRender();															//渲染下一位玩家的界面
}
void game::nowPlayerAllin() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	const int remainderChip = nowPlayer.getChip();
	const int nowBet = nowPlayer.getNowBet();
	const int allMoney = remainderChip + nowBet;
	nowPlayer.add(allMoney);			//加注至所有钱
	nowPlayer.setPlayerAction(actionType::Allin);

	if (this->m_minBet < allMoney) {	//allin后钱多于已下注，需要更新结束玩家
		this->m_endPlayerIndex = this->getPreCalledPlayerIndex(this->m_nowPlayerIndex);
		this->setMinBet(allMoney);		//更新现在最小下注
	}
	this->afterPlayerAction();

}
void game::nowPlayerCall() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(this->m_minBet);		//就是玩家加到最小bet
	nowPlayer.setPlayerAction(actionType::Call);
	this->afterPlayerAction();
}
void game::nowPlayerCheck() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	//nowPlayer.add(0);		//加到0，可以删了
	nowPlayer.setPlayerAction(actionType::Check);
	this->afterPlayerAction();
}
void game::nowPlayerFold() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.fold();
	this->hideNowPlayerHandCards();			//隐藏手牌，已在fold中设置为错误
	this->m_calledPlayersIndex.erase(this->m_nowPlayerIndex);	//从已call集合中删除
	//nowPlayer.setPlayerAction(actionType::Fold);	//已在player.fold()里
	this->afterPlayerAction();
}
void game::playerEscape(const int playerIndex)	//仿照fold处理
{
	this->hidePlayer(playerIndex);				//直接隐藏

	if (this->getEndPlayerIndex() == playerIndex) {
		const int newEndPlayerIndex = this->getPreCalledPlayerIndex(playerIndex);
		this->setEndPlayerIndex(newEndPlayerIndex);
	}
	player& escapedPlayer = this->getPlayer(playerIndex);
	this->addToPot(escapedPlayer.getNowBet());		//已下注加入底池		
	this->updateEscapedPlayerScore(playerIndex);	//更新逃跑玩家的计分表

	this->hidePlayerHandCards(playerIndex);			//隐藏手牌，已在fold中设置为错误
	this->m_calledPlayersIndex.erase(playerIndex);	//从已call集合中删除

	escapedPlayer = player();		//建立一个空player代替

	if (this->m_calledPlayersIndex.size() <= 1) {	//没人了，结算
		this->updatePots();										//更新底池与边池

		this->settle();
		//然后渲染结束本局画面，包括每位玩家的再次开始键和show牌、牌型，再次开始绑定至下一局游戏
		this->showGameEnd();
		this->setGameHasStarted(false);	//游戏结束，游戏状态设置为未开始
		return;
	}
	//this->afterPlayerAction();	//不渲染了吧
}
void game::afterPlayerAction(){									//玩家行动后
	this->nowPlayerActionComplete();	//当前玩家结束行动渲染
	if (this->m_nowPlayerIndex == this->m_endPlayerIndex || this->m_calledPlayersIndex.size() <= 1) {		//本轮结束
		this->updatePots();										//更新底池与边池
		this->finishThisRound();								//本轮结束相关渲染
		this->nextRound();										//下一轮
	}
	else {
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//下一位玩家
		this->nowPlayerRender();															//渲染下一位玩家的界面
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
			//获取组合
			if (combineMap[i][j] >= 5)
				combine[j] = handCards[combineMap[i][j] - 5];
			else
				combine[j] = commonCards[combineMap[i][j]];
			//计算
			cardTypeAndPoint nowCardTypeAndPoint = cardTypeAndPoint::calTypeAndPoint(combine);
			if (nowCardTypeAndPoint > player.getCardTypeAndPoint())
				player.setCardTypeAndPoint(nowCardTypeAndPoint);
		}
	}
	return player.getCardTypeAndPoint();
}


void game::nextRound() {
	if (this->m_calledPlayersIndex.size()<=1||this->m_round >= gameRound::River) {	//没人了或是最后一轮进行完了，结算
		this->settle();
		//然后渲染结束本局画面，包括每位玩家的再次开始键和show牌、牌型，再次开始绑定至下一局游戏
		this->showGameEnd();
		this->setGameHasStarted(false);	//游戏结束，游戏状态设置为未开始
		return;
	}
	this->m_round = gameRound(this->m_round + 1);
	this->sendCardsToTable();
	this->m_nowPlayerIndex = this->getRoundBeginPlayerIndex();			//这一轮开始的玩家，用这个是因为dealer可能弃牌了
	this->m_endPlayerIndex = this->getRoundEndPlayerIndex();			//这一轮结束的玩家
	this->renderGame();													//因为新发牌了，所以要重新渲染
	if (this->m_round == PreFlop) {										//翻前下盲注
		//翻前，每个客户端都需要show所有除自己以外在场玩家的卡背
		this->showOthersCardBackOnAllClient();

		this->getPlayer(this->m_nowPlayerIndex).add(smallBind);			//小盲
		this->getPlayer(this->m_nowPlayerIndex).setPlayerAction(actionType::Raise);
		this->nowPlayerActionComplete();
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);

		this->getPlayer(this->m_nowPlayerIndex).add(bigBind);			//大盲
		this->getPlayer(this->m_nowPlayerIndex).setPlayerAction(actionType::Raise);
		this->nowPlayerActionComplete();
		this->m_endPlayerIndex = this->m_nowPlayerIndex;				//第一轮到大盲结束 !!!!!有没有问题？
		this->m_minBet = bigBind;
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//大盲下一位开始
	}
	this->nowPlayerRender();		//渲染当前行动玩家
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
//	//当前玩家已经allin，则什么都不做（不进行渲染），直接当做行动完毕
//	if (this->m_players[this->m_nowPlayerIndex].hasAllin()) {
//		this->afterPlayerAction();
//		return true;
//	}
//	//因为暂时是单机测试，所以这里就是渲染出当前player的按键
//	{
//
//	}
//	//后续用server的话就是这里
//	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
//	//nowPlayer.enableOperation();	//client界面显示按键
//	return true;
//}

void game::renderGame() {
	//首先渲染桌子
	{
		vector<card> const& commonCards = this->getCommonCards();	//公共牌
		this->showCommonCards();
		this->showRound();	//轮次
		this->showPot();
	}
	//每位玩家
	{
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			player const& curPlayer = this->getPlayer(i_player);
			if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {	//场上玩家名字筹码和牌都显示
				this->hidePlayerAllAction(i_player);
				this->hidePlayerActionMessage(i_player);
				this->showPlayerNameCardsChip(i_player);
			}
			else if (curPlayer.getPlayerType() == playerType::Looker) {		//旁观者只显示名字和筹码
				this->hidePlayerAllAction(i_player);
				this->hidePlayerActionMessage(i_player);
				this->hidePlayerHandCards(i_player);
				this->showPlayerName(i_player);
				this->showPlayerChip(i_player);
			}
			else if (curPlayer.getPlayerType() == playerType::Empty) {		//无人则隐藏
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
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {	//在场的人需要show卡背，该函数只应在begin时调用
		if (this->m_players[i_player].getPlayerType() == playerType::OnSitePlayer && i_player != playerIndex) {
			this->m_ui->showPlayer1CardBackOnPlayer2(i_player, playerIndex);
		}
	}
}

void game::showOthersCardBackOnAllClient()
{
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getPlayerType() != playerType::Empty) {	//旁观者也要show卡背
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
	//游戏结束
	//玩家赢得结算
	for (int i_player : this->m_calledPlayersIndex) {
		player const& curPlayer = this->getPlayer(i_player);
		const int winMoney = curPlayer.getWinMoney();
		string outputMessage = "";
		if (this->m_calledPlayersIndex.size() > 1) {
			const cardType cardType = curPlayer.getCardTypeAndPoint().getCardType();
			outputMessage += "牌型：" + to_string(cardType) + "\n";
		}
		if (winMoney > 0) {		//玩家赢了
			outputMessage += "玩家赢得：" + to_string(winMoney);
		}
		else {
			outputMessage += "玩家输了";
		}
		this->showPlayerMessage(i_player, outputMessage);

		if (this->m_calledPlayersIndex.size() > 1) {	//因弃牌获胜不用show
			//show在场玩家的牌，非空就show
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
	//开始按键出现
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getPlayerType() == playerType::Looker&&this->m_players[i_player].getChip() > 0) {
			this->m_players[i_player].setPlayerType(playerType::OnSitePlayer);	//在场的looker，有钱的变为onsite
		}
	}
	this->showAllBegin();
}

void game::updatePlayerScore(const int playerIndex) {
	const string macAddress = this->getPlayerMacAddress(playerIndex);
	auto iter = this->m_macAddressToScoreChartIndex.find(macAddress);
	if (iter == this->m_macAddressToScoreChartIndex.end()) {		//没找到
		this->m_ui->showPlayerActionMessage(this->getNowPlayerIndex(), "更新计分表失败");
		return;
	}
	const int playerRow = iter->second;													//玩家在表中行号
	const int playerChip = this->getPlayer(playerIndex).getChip();						//玩家当前桌上筹码
	const int totalBuyIn = this->m_scoreChart->item(playerRow, 1)->text().toInt();		//玩家总买入
	int totalChip = this->m_scoreChart->item(playerRow, 2)->text().toInt();				//玩家总筹码
	const int onTableChip = this->m_scoreChart->item(playerRow, 3)->text().toInt();		//上次更新的，在桌上的筹码

	const int modifiedChip = playerChip - onTableChip;									//当前桌上筹码，与上次桌上筹码之差
	totalChip += modifiedChip;															//更新总筹码
	const int totalWin = totalChip - totalBuyIn;										//更新总盈利
	this->m_scoreChart->setItem(playerRow, 2, new QStandardItem(QString::number(totalChip)));
	this->m_scoreChart->setItem(playerRow, 3, new QStandardItem(QString::number(playerChip)));
	this->m_scoreChart->setItem(playerRow, 4, new QStandardItem(QString::number(totalWin)));
}
//与上面一样，只有桌上筹码修改不一样。。。
void game::updateEscapedPlayerScore(const int playerIndex) {
	const string macAddress = this->getPlayerMacAddress(playerIndex);
	auto iter = this->m_macAddressToScoreChartIndex.find(macAddress);
	if (iter == this->m_macAddressToScoreChartIndex.end()) {		//没找到
		this->m_ui->showPlayerActionMessage(this->getNowPlayerIndex(), "更新计分表失败");
		return;
	}
	const int playerRow = iter->second;													//玩家在表中行号
	const int playerChip = this->getPlayer(playerIndex).getChip();						//玩家当前桌上筹码
	const int totalBuyIn = this->m_scoreChart->item(playerRow, 1)->text().toInt();		//玩家总买入
	int totalChip = this->m_scoreChart->item(playerRow, 2)->text().toInt();				//玩家总筹码
	const int onTableChip = this->m_scoreChart->item(playerRow, 3)->text().toInt();		//上次更新的，在桌上的筹码

	const int modifiedChip = playerChip - onTableChip;									//当前桌上筹码，与上次桌上筹码之差
	totalChip += modifiedChip;															//更新总筹码
	const int totalWin = totalChip - totalBuyIn;										//更新总盈利
	this->m_scoreChart->setItem(playerRow, 2, new QStandardItem(QString::number(totalChip)));
	this->m_scoreChart->setItem(playerRow, 3, new QStandardItem(QString::number(0)));	//桌上筹码改为0
	this->m_scoreChart->setItem(playerRow, 4, new QStandardItem(QString::number(totalWin)));
}
void game::nowPlayerActionComplete() {
	this->hideNowPlayerAllAction();		//隐藏当前玩家行动界面
	this->showNowPlayerChip();			//显示当前玩家筹码信息
	this->showNowPlayerActionMessage();	//当前玩家行动信息
}

void game::finishThisRound() {
	this->showPot();	//展示底池
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		player& curPlayer = this->getPlayer(i_player);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			this->hidePlayerActionMessage(i_player);
			if (curPlayer.getPlayerAction() != actionType::Allin) {		//对于非allin的玩家，设定行动为什么也没做，待下轮行动
				curPlayer.setPlayerAction(actionType::Nothing);
			}
		}
	}
}

bool game::nowPlayerRender() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	player const& nowPlayer = this->getPlayer(nowPlayerIndex);
	//this->hidePlayerActionMessage(nowPlayerIndex);		//应该单独拿出去作为一个函数
	this->showPlayerMessage(nowPlayerIndex, "玩家行动中...");	//显示当前行动玩家

	if (nowPlayer.hasAllin()) {
		this->afterPlayerAction();
		return true;
	}
	const int minBet = this->getMinBet();
	const int playerNowBet = nowPlayer.getNowBet();
	const int playerChip = nowPlayer.getChip();
	if (minBet <= playerNowBet) {	//当前下注小于本人已下注
		this->showPlayerCheckRaiseFoldAction(nowPlayerIndex);
	}
	else if (minBet >= playerChip) {	//需要allin
		this->showPlayerAllinFoldAction(nowPlayerIndex);
	}
	else if (minBet < playerChip && minBet > playerNowBet) {	//当前最小下注大于已下注
		this->showPlayerCallRaiseFoldAction(nowPlayerIndex);
	}
	else {}
	return true;
}


bool game::begin() {
	//先计算是不是所有玩家都准备了
	const int numOfPlayer = this->getNumOfPlayers();
	if (this->m_readyNumOfPlayer < numOfPlayer) {
		return false;
	}

	//this->hideBegin();
	this->m_calledPlayersIndex.clear();
	for (int playerIndex = 0; playerIndex < maxNumOfPlayers; ++playerIndex) {
		//if (this->m_players[playerIndex].getPlayerType() == playerType::Looker && this->m_players[playerIndex].getChip() > 0) {
		//	this->m_players[playerIndex].setPlayerType(playerType::OnSitePlayer);	//有钱的旁观者上场
		//}
		if (this->m_players[playerIndex].getPlayerType() == playerType::OnSitePlayer) {
			this->m_calledPlayersIndex.insert(playerIndex);
		}
		this->m_players[playerIndex].initPlayerState();		//初始化玩家状态
	}
	if (this->m_calledPlayersIndex.size() <= 1)	//小于等于1个人不能玩
		return false;

	this->m_readyNumOfPlayer = 0;

	this->m_round = Start;		//开始
	this->setGameHasStarted(true);	//游戏状态设为开始
	this->updateDealer();		//更新dealer
	this->showDealer();			//在客户端显示dealer

	//this->initPlayersState();	//初始化玩家状态
	this->clearCommonCards();	//清空桌上的牌
	this->clearPot();
	this->clearSidePot();		//清空边池
	this->hideAllPlayerSidePot();	//隐藏边池，可以不写？
	this->shuffleCardHeap();	//洗牌
	this->renderGame();			//渲染游戏界面
	this->nextRound();			//进行下一轮
	return true;
}


//bool game::begin() {
//	const int numOfPlayers = this->getCalledPlayersIndex().size();
//	if (numOfPlayers <= 1) {
//		return false;
//	}
//	
//	//开始游戏
//	this->initPlayersState();	//初始化玩家状态
//	this->m_commonCards.clear();//清空桌上的牌
//	this->shuffleCardHeap();	//洗牌
//	//sendCardsToPlayers();		//给玩家发牌
//	for (this->m_round = PreFlop; this->m_round <= River; this->m_round = gameRound(this->m_round + 1)) {
//		this->sendCardsToTable();	//发牌，若是首轮 给玩家发牌
//		this->playersAction();		//玩家行动
//		this->updatePot();			//更新底池
//		if (this->getCalledPlayersIndex().size() == 1) {
//			break;
//		}
//	}
//	this->settle();	//结算
//
//	return true;
//}
void game::playerWin(int winnerIndex){	//可以用作渲染赢了的场景
	//在这

	player& winnerPlayer = this->getPlayer(winnerIndex);
	winnerPlayer.win();
}

void game::playerLose(int loserIndex){	//可以用作渲染赢了的场景
	//在这
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

	//按照距离dealer排序，远的在前，不知道对不对
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

	//按照距离dealer排序，远的在前，不知道对不对
	sort(res.begin(), res.end(), [&](int playerIndex1, int playerIndex2) {return this->m_dealer + maxNumOfPlayers - playerIndex1 > this->m_dealer + maxNumOfPlayers - playerIndex2; });
	return res;
}

void game::settle(){		//结算
	if (this->m_calledPlayersIndex.size() > 1)
		this->calCardTypeAndPointForAll();
	//底池结算，已allin的不计算，等边池
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

	//边池结算
	for (auto const& sidePot : this->m_sidePots) {
		int sidePotMoney = sidePot.getSidePotMoney();									//这两行，要不要改成函数呢，那就得写一个边池类
		vector<int> const& participateInSidePot = sidePot.getSidePotParticipatIndex();	//这两行
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
	//玩家自己结算
	for (int i = 0; i < maxNumOfPlayers; ++i) {
		player& curPlayer = this->getPlayer(i);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			if (curPlayer.getWinMoney() > 0)
				this->playerWin(i);
			else
				this->playerLose(i);
			//正常修改计分表
			this->updatePlayerScore(i);
		}
	}
}

void game::updatePots(){	//更新底池与边池
	//先按照当前轮下注排序，从小到大
	vector<int> playerNowBetSortedIndexes;
	//for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
	//	int curPlayerIndex = *playerIndexIterator;
	//	playerNowBetSortedIndexes.push_back(curPlayerIndex);
	//}
	for (int i_player = 0; i_player < (int)this->m_players.size(); ++i_player) {
		if (this->m_players[i_player].getNowBet() > 0) {				//只要下注了就加进去，不论是否弃牌
			playerNowBetSortedIndexes.push_back(i_player);
		}
	}
	sort(playerNowBetSortedIndexes.begin(), playerNowBetSortedIndexes.end(), [&](int index1, int index2) {
		if (this->getPlayer(index1).getNowBet() == this->getPlayer(index2).getNowBet())
			return this->getPlayer(index1).getChip() < this->getPlayer(index2).getChip();	//bet相同的，筹码小的放前面，这样就能先找到allin的了
		return this->getPlayer(index1).getNowBet() < this->getPlayer(index2).getNowBet(); });
	int hasInSidePot = 0;
	for (int i = 0; i < (int)playerNowBetSortedIndexes.size(); ++i) {
		int curPlayerIndex = playerNowBetSortedIndexes[i];
		player& curPlayer = this->getPlayer(curPlayerIndex);
		if (curPlayer.isAllinThisRound()) {	//当前轮allin了，allin必不可能是fold
			int moneyPerPlayer = curPlayer.getNowBet();
			int newSidePotIndex = this->addNewSidePot((moneyPerPlayer - hasInSidePot)*(playerNowBetSortedIndexes.size() - i) + this->m_pot);//设置边池，一局游戏仅在for中更新一次即可
			this->m_pot = 0;
			for (int j = i; j < (int)playerNowBetSortedIndexes.size(); ++j) {		//本就可能添加fold的人，如本轮没fold后续fold了
				this->addPlayerToLastSidePot(newSidePotIndex, playerNowBetSortedIndexes[j]);												//添加边池人员
			}
			hasInSidePot = moneyPerPlayer;

			this->showPlayerSidePot(curPlayerIndex, this->m_sidePots[newSidePotIndex].getSidePotMoney());		//渲染边池，可以不加，大于0的判断在底层
		}
		else {	//出现一次无人allin 后续必无人allin，（下注的都需要考虑，包括弃牌的，所以后续可能有allin，比如1 allin， 2 raise， 3 raise， 2再fold）
			this->m_pot += curPlayer.getNowBet() - hasInSidePot;	//非allin的放进底池里
		}
		curPlayer.setNowBet(0);		//此玩家此轮下注已进入底池或边池，置为0
	}
	this->setMinBet(0);	//最小bet为0
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
		string actionMessage = "跟注：" + to_string(callMoney);
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Check) {
		string actionMessage = "看牌";
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Raise) {
		const int raiseMoney = nowPlayer.getNowBet();
		string actionMessage = "加注至：" + to_string(raiseMoney);
		this->m_ui->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Fold) {
		string actionMessage = "弃牌";
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
	const int minMoney = this->m_minBet + game::bigBind;			//+大盲为最小加注
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
