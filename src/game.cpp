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
	//重写了，dealer前一个有效玩家的后一个
	int preCalledPlayerIndex = this->getPreCalledPlayerIndex(this->m_dealer);
	return this->getNextCalledPlayerIndex(preCalledPlayerIndex);
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
	else if (this->m_round == Flop) {	//翻牌
		this->m_commonCards[0] = this->m_cardHeap.getCard();
		this->m_commonCards[1] = this->m_cardHeap.getCard();
		this->m_commonCards[2] = this->m_cardHeap.getCard();
	}
	else if (this->m_round == Turn) {	//转牌
		this->m_commonCards[3] = this->m_cardHeap.getCard();
	}
	else if (this->m_round == River) {	//河牌
		this->m_commonCards[4] = this->m_cardHeap.getCard();
	}
}

//四个当前玩家的动作，可以将玩家按键全部绑定至此
void game::nowPlayerRaise(const int raiseTo) {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(raiseTo);
	//更新当前玩家和结束玩家，因为raise了，必然要走一轮，所以不判断是否进入下一轮
	//this->afterPlayerAction();		//所以本行删掉
	this->nowPlayerActionComplete();	//当前玩家结束行动渲染
	this->m_endPlayerIndex = this->m_nowPlayerIndex;				
	this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//下一位玩家
	this->nowPlayerRender();															//渲染下一位玩家的界面
}
void game::nowPlayerCall() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.add(this->m_minBet);		//就是玩家加到最小bet
	this->afterPlayerAction();
}
void game::nowPlayerCheck() {
	//player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	//nowPlayer.add(0);		//加到0，可以删了
	this->afterPlayerAction();
}
void game::nowPlayerFold() {
	player& nowPlayer = this->getPlayer(this->m_nowPlayerIndex);
	nowPlayer.fold();
	this->m_calledPlayersIndex.erase(this->m_nowPlayerIndex);	//从已call集合中删除
	this->afterPlayerAction();
}
void game::afterPlayerAction(){									//玩家行动后
	this->nowPlayerActionComplete();	//当前玩家结束行动渲染
	if (this->m_nowPlayerIndex == this->m_endPlayerIndex) {		//本轮结束
		this->finishThisRound();								//本轮结束相关渲染
		this->updatePots();										//更新底池与边池
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

void game::calCardTypeAndPointForPlayer(const int playerIndex) {
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
}


void game::nextRound() {
	if (this->m_calledPlayersIndex.size()<=1||this->m_round >= End) {	//没人了或是最后一轮进行完了，结算
		this->settle();
		//然后渲染结束本局画面，包括每位玩家的再次开始键和show牌、牌型，再次开始绑定至下一局游戏
		return;
	}
	this->m_round = gameRound(this->m_round + 1);
	this->sendCardsToTable();
	this->m_nowPlayerIndex = this->getRoundBeginPlayerIndex();			//这一轮开始的玩家，用这个是因为dealer可能弃牌了
	this->m_endPlayerIndex = this->getRoundEndPlayerIndex();			//这一轮结束的玩家
	if (this->m_round == PreFlop) {	//翻前下盲注
		this->getPlayer(this->m_nowPlayerIndex).add(smallBind);			//小盲
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);
		this->getPlayer(this->m_nowPlayerIndex).add(bigBind);			//大盲
		this->m_endPlayerIndex = this->m_nowPlayerIndex;				//第一轮到大盲结束 !!!!!有没有问题？
		this->m_minBet = bigBind;
		this->m_nowPlayerIndex = this->getNextCalledPlayerIndex(this->m_nowPlayerIndex);	//大盲下一位开始
	}
	this->renderGame();				//因为新发牌了，所以要重新渲染
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
		this->m_ui->showCommonCards(commonCards);
		this->m_ui->showRound(this->getGameRound());	//轮次
																	//if (commonCards[0].isCardVailid() && commonCards[1].isCardVailid() && commonCards[2].isCardVailid()) {	//翻牌
																	//}
	}
	//每位玩家
	{
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			player const& curPlayer = this->getPlayer(i_player);
			if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {	//场上玩家名字筹码和牌都显示
				this->m_ui->hidePlayerAction(i_player);
				this->m_ui->hidePlayerActionMessage(i_player);
				this->m_ui->showPlayerNameCardsChip(i_player, curPlayer);
			}
			else if (curPlayer.getPlayerType() == playerType::Looker) {		//旁观者只显示名字和筹码
				this->m_ui->hidePlayerAction(i_player);
				this->m_ui->hidePlayerActionMessage(i_player);
				this->m_ui->hidePlayerHandCards(i_player);
				this->m_ui->showPlayerName(i_player, curPlayer.getName());
				this->m_ui->showPlayerChip(i_player, curPlayer.getChip());
			}
			else if (curPlayer.getPlayerType() == playerType::Empty) {		//无人则隐藏
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
	this->hideNowPlayerAction();		//隐藏当前玩家行动界面
										//当前玩家行动信息
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
		string actionMessage = "跟注：" + to_string(callMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Check) {
		string actionMessage = "看牌";
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Raise) {
		const int raiseMoney = nowPlayer.getNowBet();
		string actionMessage = "加注至：" + to_string(raiseMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Fold) {
		string actionMessage = "弃牌";
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
			if (curPlayer.getPlayerAction() != actionType::Allin) {		//对于非allin的玩家，设定行动为什么也没做，待下轮行动
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
	if (minBet <= playerNowBet) {	//当前下注小于本人已下注
		this->m_ui->playerCheckRaiseFoldAction(nowPlayerIndex, game::bigBind, playerChip);
	}
	else if (minBet >= playerChip) {	//需要allin
		this->m_ui->playerAllinFoldAction(nowPlayerIndex, playerChip);
	}
	else if (minBet < playerChip && minBet > playerNowBet) {	//当前最小下注大于已下注
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
		this->m_players[playerIndex].initPlayerState();		//初始化玩家状态
	}
	if (this->m_calledPlayersIndex.size() <= 1)	//小于等于1个人不能玩
		return false;

	this->m_round = Start;		//开始
	this->updateDealer();		//更新dealer
	//this->initPlayersState();	//初始化玩家状态
	this->clearCommonCards();	//清空桌上的牌
	this->clearSidePot();		//清空边池
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
	
	//底池结算，已allin的不计算，等边池
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

	//边池结算
	for (auto const& sidePot : this->m_sidePots) {
		int sidePotMoney = sidePot.getSidePotMoney();									//这两行，要不要改成函数呢，那就得写一个边池类
		vector<int> const& participateInSidePot = sidePot.getSidePotParticipatIndex();	//这两行
		vector<int> winPlayerIndexesInSidePot = this->getMaxPlayersIndex(participateInSidePot);
		int winMoney = sidePotMoney / (winPlayerIndexesInSidePot.size());
		int remainderMoney = sidePotMoney % (winPlayerIndexesInSidePot.size());
		for (auto winPlayerIndex : winPlayerIndexesInSidePot) {
			this->getPlayer(winPlayerIndex).addToWinMoney(winMoney);
			if (remainderMoney-- > 0)
				this->getPlayer(winPlayerIndex).addToWinMoney(1);
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
		}
	}
}

void game::updatePots(){	//更新底池与边池
	//先按照当前轮下注排序，从小到大
	vector<int> playerNowBetSortedIndexes;
	for (auto playerIndexIterator = this->m_calledPlayersIndex.begin(); playerIndexIterator != this->m_calledPlayersIndex.end(); ++playerIndexIterator) {
		int curPlayerIndex = *playerIndexIterator;
		playerNowBetSortedIndexes.push_back(curPlayerIndex);
	}
	sort(playerNowBetSortedIndexes.begin(), playerNowBetSortedIndexes.end(), [&](int index1, int index2) {
		if (this->getPlayer(index1).getNowBet() == this->getPlayer(index2).getNowBet())
			return this->getPlayer(index1).getChip() < this->getPlayer(index2).getChip();	//bet相同的，筹码小的放前面，这样就能先找到allin的了
		return this->getPlayer(index1).getNowBet() < this->getPlayer(index2).getNowBet(); });
	int hasInSidePot = 0;
	for (int i = 0; i < (int)playerNowBetSortedIndexes.size(); ++i) {
		int curPlayerIndex = playerNowBetSortedIndexes[i];
		player& curPlayer = this->getPlayer(curPlayerIndex);
		if (curPlayer.isAllinThisRound()) {	//当前轮allin了
			int moneyPerPlayer = curPlayer.getNowBet();
			int newSidePotIndex = this->addNewSidePot((moneyPerPlayer - hasInSidePot)*(playerNowBetSortedIndexes.size() - i) + this->m_pot);//设置边池，一局游戏仅在for中更新一次即可
			this->m_pot = 0;
			for (int j = i; j < (int)playerNowBetSortedIndexes.size(); ++j) {
				this->addPlayerToLastSidePot(newSidePotIndex, playerNowBetSortedIndexes[j]);												//添加边池人员
			}
			hasInSidePot += moneyPerPlayer;
		}
		else {	//出现一次无人allin 后续必无人allin
			this->m_pot += curPlayer.getNowBet() - hasInSidePot;	//非allin的放进底池里
		}
		curPlayer.setNowBet(0);		//此玩家此轮下注已进入底池或边池，置为0
	}
}
