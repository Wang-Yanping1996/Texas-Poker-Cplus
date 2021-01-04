#include "TexasPokerSingleUi.h"

QString getCardFileName(card const& c) {
	cardColor color = c.getColor();
	cardNumber num = c.getNumber();
	QString cardFileName = QString::fromUtf8("image/") + QString::number((int)color) + QString::fromUtf8("_") + QString::number((int)(num / 10)) + QString::number((int)(num % 10)) + QString::fromUtf8(".jpg");
	return cardFileName;
};

void Ui_singleGameWindow::showCommonCards(vector<card> const & needShowCommonCards) const {
	for (int i_card = 0; i_card < game::maxNumOfCommonCards && needShowCommonCards[i_card].isCardVailid(); ++i_card) {
		QString cardFileName = getCardFileName(needShowCommonCards[i_card]);
		this->commonCards[i_card]->setPixmap(QPixmap(cardFileName));
		this->commonCards[i_card]->raise();
	}
}

void Ui_singleGameWindow::hideCommonCards() const {
	for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
		this->commonCards[i_card]->hide();
	}
}

void Ui_singleGameWindow::showRound(gameRound nowRound) const {
	if (nowRound == gameRound::Flop) {
		this->flop->raise();
	}
	else if (nowRound == gameRound::Turn) {
		this->turn->raise();
	}
	else if (nowRound == gameRound::River) {
		this->river->raise();
	}
	else {
		this->hideRound();
	}
}

void Ui_singleGameWindow::hideRound() const {
	this->flop->hide();
	this->turn->hide();
	this->river->hide();
}

void Ui_singleGameWindow::showPot(const int potNum) const {
	QString showText = QStringLiteral("底池：") + QString::number(potNum);
	this->pot->setText(showText);
	this->pot->raise();
}

void Ui_singleGameWindow::hidePot() const {
	this->pot->hide();
}


void Ui_singleGameWindow::showPlayerHandCards(const int playerIndex, vector<card> const& handCards) const {
	this->players[playerIndex]->showHandCards(handCards);
}

void Ui_singleGameWindow::showPlayerName(const int playerIndex, string const& playerName) const {
	this->players[playerIndex]->showPlayerName(playerName);
}

void Ui_singleGameWindow::showPlayerChip(const int playerIndex, const int chip) const {
	this->players[playerIndex]->showPlayerChip(chip);
}

void Ui_singleGameWindow::showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer) const {
	this->showPlayerName(playerIndex, needShowPlayer.getName());
	this->showPlayerHandCards(playerIndex, needShowPlayer.getHandCards());
	this->showPlayerChip(playerIndex, needShowPlayer.getChip());
}

void Ui_singleGameWindow::showPlayerActionMessage(const int playerIndex, QString const & actionMessage) const {
	this->players[playerIndex]->showPlayerActionMessage(actionMessage);
}

void Ui_singleGameWindow::hidePlayerHandCards(const int playerIndex) const {
	this->players[playerIndex]->hideHandCards();
}

void Ui_singleGameWindow::hidePlayerName(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerName();
}

void Ui_singleGameWindow::hidePlayerChip(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerChip();
}

void Ui_singleGameWindow::hidePlayerNameCardsChip(const int playerIndex) const {
	this->hidePlayerName(playerIndex);
	this->hidePlayerHandCards(playerIndex);
	this->hidePlayerChip(playerIndex);
}

void Ui_singleGameWindow::hidePlayerActionMessage(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerActionMessage();
}
void Ui_singleGameWindow::hidePlayerAction(const int playerIndex) const {
	this->players[playerIndex]->hideAllAction();
}
void Ui_singleGameWindow::hidePlayer(const int playerIndex) const {
	this->hidePlayerNameCardsChip(playerIndex);
	this->hidePlayerActionMessage(playerIndex);
	this->hidePlayerAction(playerIndex);
}



void Ui_playerWindow::showHandCards(vector<card> const& c) const {
	for (int i_card = 0; i_card < player::numOfHandCards; ++i_card) {
		if (c[i_card].isCardVailid()) {
			QString cardFileName = getCardFileName(c[i_card]);
			this->playerHandCards[i_card]->setPixmap(cardFileName);
			this->playerHandCards[i_card]->raise();
		}
		else {
			this->playerHandCards[i_card]->hide();
		}
	}

}

void Ui_playerWindow::hideHandCards() const {
	for (int i_card = 0; i_card < player::numOfHandCards; ++i_card) {
		this->playerHandCards[i_card]->hide();
	}
}

void Ui_playerWindow::showPlayerName(string const& name) const {
	QString showName = QString::fromStdString(name);
	this->playerName->setText(showName);
	this->playerName->raise();
}

void Ui_playerWindow::hidePlayerName() const {
	this->playerName->hide();
}

void Ui_playerWindow::showPlayerChip(const int chip) const {
	this->playerChip->setText(QStringLiteral("筹码：") + QString::number(chip));
	this->playerChip->raise();
}

void Ui_playerWindow::hidePlayerChip() const {
	this->playerChip->hide();
}

void Ui_playerWindow::showPlayerActionMessage(QString const & actionMessage) const {
	this->actionMessage->setText(actionMessage);
	this->actionMessage->raise();
}

void Ui_playerWindow::hidePlayerActionMessage() const {
	this->actionMessage->hide();
}

void Ui_playerWindow::showRaise(const int minRaiseMoney, const int maxRaiseMoney) const {
	this->raise->raise();		//显示按键
	this->raiseMoney->setRange(minRaiseMoney, maxRaiseMoney);	//设置范围
	this->raiseMoney->setSingleStep(game::smallBind);			//设置最少一次改变多少
	this->raiseMoney->raise();
}

void Ui_playerWindow::hideRaise() const {
	this->raise->hide();
	this->raiseMoney->hide();
}

void Ui_playerWindow::showAllin(const int allMoney) const {
	this->showRaise(allMoney, allMoney);
}

void Ui_playerWindow::showCheck() const {
	this->check->raise();
}

void Ui_playerWindow::showCall(const int callMoney) const {
	QString showText = QStringLiteral("跟注:") + QString::number(callMoney);
	this->call->setText(showText);
	this->call->raise();
}

void Ui_playerWindow::hideCall() const {
	this->call->hide();
}

void Ui_playerWindow::hideCheck() const {
	this->check->hide();
}

void Ui_playerWindow::showFold() const {
	this->fold->raise();
}

void Ui_playerWindow::hideFold() const {
	this->fold->hide();
}

void Ui_playerWindow::hideAllAction() const {
	this->hideRaise();
	this->hideCheck();
	this->hideCall();
	this->hideFold();
}

void Ui_playerWindow::allinFoldAction(const int allmoney) const {
	this->showAllin(allmoney);
	this->showFold();
}

void Ui_playerWindow::callRaiseFoldAction(const int callMoney, const int minRaiseMoney, const int maxRaiseMoney) const {
	this->showCall(callMoney);
	this->showRaise(minRaiseMoney, maxRaiseMoney);
	this->showFold();
}

void Ui_playerWindow::checkRaiseFoldAction(const int minRaiseMoney, const int maxRaiseMoney) const {
	this->showCheck();
	this->showRaise(minRaiseMoney, maxRaiseMoney);
	this->showFold();
}

void Ui_playerWindow::hideAll() const {
	this->hideHandCards();
	this->hidePlayerName();
	this->hidePlayerChip();
	this->hidePlayerActionMessage();
	this->hideAllAction();
}
