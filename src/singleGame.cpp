#include "singleGame.h"
#include "..\include\singleGame.h"

void singleGame::renderGame(){
	//������Ⱦ����
	{
		vector<card> const& commonCards = this->getCommonCards();	//������
		this->m_singleGameWindow->showCommonCards(commonCards);
		this->m_singleGameWindow->showRound(this->getGameRound());	//�ִ�
		//if (commonCards[0].isCardVailid() && commonCards[1].isCardVailid() && commonCards[2].isCardVailid()) {	//����
		//}
	}
	//ÿλ���
	{
		for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
			player const& curPlayer = this->getPlayer(i_player);
			if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {	//����������ֳ�����ƶ���ʾ
				this->m_singleGameWindow->hidePlayerAction(i_player);
				this->m_singleGameWindow->hidePlayerActionMessage(i_player);
				this->m_singleGameWindow->showPlayerNameCardsChip(i_player, curPlayer);
			}
			else if (curPlayer.getPlayerType() == playerType::Looker) {		//�Թ���ֻ��ʾ���ֺͳ���
				this->m_singleGameWindow->hidePlayerAction(i_player);
				this->m_singleGameWindow->hidePlayerActionMessage(i_player);
				this->m_singleGameWindow->hidePlayerHandCards(i_player);
				this->m_singleGameWindow->showPlayerName(i_player, curPlayer.getName());
				this->m_singleGameWindow->showPlayerChip(i_player, curPlayer.getChip());
			}
			else if (curPlayer.getPlayerType() == playerType::Empty) {		//����������
				this->m_singleGameWindow->hidePlayer(i_player);
			}
			else {}
		}
	}
}

void singleGame::hideNowPlayerAction() {
	const int nowPlayerIndex = this->getNowPlayerIndex();
	this->m_singleGameWindow->hidePlayerAction(nowPlayerIndex);
}

void singleGame::showNowPlayerActionMessage(QString const& actionMessage) {
	this->m_singleGameWindow->showPlayerActionMessage(this->getNowPlayerIndex(), actionMessage);
}

void singleGame::nowPlayerActionComplete() {
	this->hideNowPlayerAction();		//���ص�ǰ����ж�����
	//��ǰ����ж���Ϣ
	const int nowPlayerIndex = this->getNowPlayerIndex();
	player const& nowPlayer = this->getPlayer(nowPlayerIndex);
	if (nowPlayer.getPlayerAction() == actionType::Nothing) {
		this->m_singleGameWindow->hidePlayerActionMessage(nowPlayerIndex);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Allin) {
		const int allinMoney = nowPlayer.getNowBet();
		QString actionMessage = "allin";
		if (allinMoney > 0)
			actionMessage += QString::fromStdString(":") + QString::number(allinMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Call) {
		const int callMoney = nowPlayer.getNowBet();
		QString actionMessage = QStringLiteral("��ע��") + QString::number(callMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Check) {
		QString actionMessage = QStringLiteral("����");
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Raise) {
		const int raiseMoney = nowPlayer.getNowBet();
		QString actionMessage = QStringLiteral("��ע����") + QString::number(raiseMoney);
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::Fold) {
		QString actionMessage = QStringLiteral("����");
		this->showNowPlayerActionMessage(actionMessage);
	}
	else if (nowPlayer.getPlayerAction() == actionType::ErrorAction) {

	}
	else {}
}

void singleGame::finishThisRound() {
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		player& curPlayer = this->getPlayer(i_player);
		if (curPlayer.getPlayerType() == playerType::OnSitePlayer) {
			this->m_singleGameWindow->hidePlayerActionMessage(i_player);
			if (curPlayer.getPlayerAction() != actionType::Allin) {		//���ڷ�allin����ң��趨�ж�ΪʲôҲû�����������ж�
				curPlayer.setPlayerAction(actionType::Nothing);
			}
		}
	}
}

bool singleGame::nowPlayerRender(){
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
		this->m_singleGameWindow->players[nowPlayerIndex]->checkRaiseFoldAction(game::bigBind, playerChip);
	}
	else if (minBet >= playerChip) {	//��Ҫallin
		this->m_singleGameWindow->players[nowPlayerIndex]->allinFoldAction(playerChip);
	}
	else if (minBet < playerChip && minBet > playerNowBet) {	//��ǰ��С��ע��������ע
		this->m_singleGameWindow->players[nowPlayerIndex]->callRaiseFoldAction(minBet - playerNowBet, minBet + this->game::bigBind, playerChip);
	}
	else {}
	return true;
}
