#include "singleGameUI.h"
//
//void singleGameWindow::testShow() {
//	int a = 0;
//	a += 10;
//}

QString getCardFileName(card const& c) {
	cardColor color = c.getColor();
	cardNumber num = c.getNumber();
	QString cardFileName = QString::fromUtf8("image/poker/") + QString::number((int)color) + QString::fromUtf8("_") + QString::number((int)(num / 10)) + QString::number((int)(num % 10)) + QString::fromUtf8(".jpg");
	return cardFileName;
};
playerWindow::playerWindow(QWidget *centralWidget, int x, int y, int playerIndex) {
	for (int i = 0; i < player::numOfHandCards; ++i) {
		playerHandCards[i] = new QLabel(centralWidget);
		playerHandCards[i]->setObjectName(QStringLiteral("playerHandCard1"));
		playerHandCards[i]->setGeometry(QRect(handCardPosition[i][0] + x, handCardPosition[i][1] + y, 70, 100));
		playerHandCards[i]->setPixmap(QPixmap(QString::fromUtf8("image/poker/0_00.jpg")));
		playerHandCards[i]->raise();
		playerHandCards[i]->hide();
	}
	raise = new QPushButton(centralWidget);
	raise->setObjectName(QStringLiteral("raise"));
	raise->setGeometry(QRect(80 + x, 40 + y, 70, 23));
	allin = new QPushButton(centralWidget);
	allin->setObjectName(QStringLiteral("raise"));
	allin->setGeometry(QRect(80 + x, 40 + y, 70, 23));
	check = new QPushButton(centralWidget);
	check->setObjectName(QStringLiteral("check"));
	check->setGeometry(QRect(0 + x, 40 + y, 70, 23));
	call = new QPushButton(centralWidget);
	call->setObjectName(QStringLiteral("check"));
	call->setGeometry(QRect(0 + x, 40 + y, 70, 23));
	fold = new QPushButton(centralWidget);
	fold->setObjectName(QStringLiteral("fold"));
	fold->setGeometry(QRect(160 + x, 40 + y, 70, 23));
	raiseMoney = new QSpinBox(centralWidget);
	raiseMoney->setObjectName(QStringLiteral("spinBox"));
	raiseMoney->setGeometry(QRect(80 + x, 10 + y, 47, 22));

	QFont font1;
	font1.setPointSize(10);
	font1.setBold(true);
	font1.setWeight(75);
	playerName = new QLabel(centralWidget);
	playerName->setObjectName(QStringLiteral("label_4"));
	playerName->setGeometry(QRect(0 + x, 0 + y, 100, 31));
	playerName->setFont(font1);
	playerName->setLayoutDirection(Qt::LeftToRight);
	playerName->setAlignment(Qt::AlignCenter);

	playerChip = new QLabel(centralWidget);
	playerChip->setObjectName(QStringLiteral("chip"));
	playerChip->setGeometry(QRect(130 + x, 0 + y, 100, 31));
	playerChip->setFont(font1);
	playerChip->setLayoutDirection(Qt::LeftToRight);
	playerChip->setAlignment(Qt::AlignCenter);

	actionMessage = new QLabel(centralWidget);
	actionMessage->setObjectName(QStringLiteral("actionMessage"));
	actionMessage->setGeometry(QRect(40 + x, 26 + y, 121, 40));
	actionMessage->setFont(font1);
	actionMessage->setLayoutDirection(Qt::LeftToRight);
	actionMessage->setAlignment(Qt::AlignCenter);

	sidePot = new QLabel(centralWidget);
	sidePot->setObjectName(QStringLiteral("sidePot"));
	sidePot->setGeometry(QRect(60 + x, 170 + y, 90, 20));
	sidePot->setFont(font1);
	sidePot->setLayoutDirection(Qt::LeftToRight);
	sidePot->setAlignment(Qt::AlignCenter);

	begin = new QPushButton(centralWidget);
	begin->setObjectName(QStringLiteral("begin"));
	begin->setGeometry(QRect(50 + x, 60 + y, 50, 30));
	begin->raise();
	begin->setFont(font1);
	begin->setText(QStringLiteral("开始"));

	raise->raise();
	allin->raise();
	check->raise();
	fold->raise();
	call->raise();
	raiseMoney->raise();
	playerName->raise();
	playerChip->raise();
	actionMessage->raise();
	sidePot->raise();

	raise->hide();
	allin->hide();
	check->hide();
	fold->hide();
	call->hide();
	raiseMoney->hide();
	//playerName->hide();
	//playerChip->hide();
	actionMessage->hide();
	sidePot->hide();

	raise->setText(QStringLiteral("加注"));
	allin->setText(QStringLiteral("allin"));
	check->setText(QStringLiteral("看牌"));
	fold->setText(QStringLiteral("弃牌"));
	call->setText(QStringLiteral("跟注"));
	playerName->setText(QStringLiteral("玩家1"));
	playerChip->setText(QStringLiteral("筹码："));
	actionMessage->setText(QStringLiteral("玩家行为："));
	sidePot->setText(QStringLiteral("边池："));

	connect(this->begin, SIGNAL(clicked()), this, SLOT(playerReady()));
}

void playerWindow::showHandCards(vector<card> const& c) const {
	for (int i_card = 0; i_card < (int)c.size(); ++i_card) {
		if (c[i_card].isCardVailid()) {
			QString cardFileName = getCardFileName(c[i_card]);
			this->playerHandCards[i_card]->setPixmap(cardFileName);
			this->playerHandCards[i_card]->show();
		}
		else {
			this->playerHandCards[i_card]->hide();
		}
	}

}

void playerWindow::hideHandCards() const {
	for (int i_card = 0; i_card < player::numOfHandCards; ++i_card) {
		this->playerHandCards[i_card]->hide();
	}
}

void playerWindow::showPlayerName(string const& name) const {
	QString showName = QString::fromLocal8Bit(name.data());
	this->playerName->setText(showName);
	this->playerName->show();
}

void playerWindow::hidePlayerName() const {
	this->playerName->hide();
}

void playerWindow::showPlayerChip(const int chip) const {
	this->playerChip->setText(QStringLiteral("筹码：") + QString::number(chip));
	this->playerChip->show();
}

void playerWindow::hidePlayerChip() const {
	this->playerChip->hide();
}

void playerWindow::showPlayerActionMessage(QString const & actionMessage) const {
	this->actionMessage->setText(actionMessage);
	this->actionMessage->show();
}

void playerWindow::hidePlayerActionMessage() const {
	this->actionMessage->hide();
}

void playerWindow::showSidePot(const int money) const
{
	if (money > 0) {
		QString message = QStringLiteral("边池：");
		message += QString::number(money);
		this->sidePot->setText(message);
		this->sidePot->show();
	}
	else {
		this->sidePot->hide();
	}
}

void playerWindow::hideSidePot() const
{
	this->sidePot->hide();
}

void playerWindow::showRaise(const int minRaiseMoney, const int maxRaiseMoney) const {
	this->raise->show();		//显示按键
	this->raiseMoney->setRange(minRaiseMoney, maxRaiseMoney);	//设置范围
	this->raiseMoney->setValue(minRaiseMoney);					//初值设为最小值
	this->raiseMoney->setSingleStep(game::bigBind);				//设置最少一次改变多少
	this->raiseMoney->show();
}

void playerWindow::hideRaise() const {
	this->raise->hide();
	this->raiseMoney->hide();
}

void playerWindow::showAllin(const int allMoney) const {
	this->allin->show();
	this->raiseMoney->setRange(allMoney, allMoney);
	this->raiseMoney->show();
}

void playerWindow::hideAllin() const
{
	this->allin->hide();
}

void playerWindow::showCheck() const {
	this->check->show();
}

void playerWindow::showCall(const int callMoney) const {
	QString showText = QStringLiteral("跟注:") + QString::number(callMoney);
	this->call->setText(showText);
	this->call->show();
}

void playerWindow::hideCall() const {
	this->call->hide();
}

void playerWindow::hideCheck() const {
	this->check->hide();
}

void playerWindow::showFold() const {
	this->fold->show();
}

void playerWindow::hideFold() const {
	this->fold->hide();
}

void playerWindow::hideAllAction() const {
	this->hideRaise();
	this->hideAllin();
	this->hideCheck();
	this->hideCall();
	this->hideFold();
}

void playerWindow::allinFoldAction(const int allmoney) const {
	this->showAllin(allmoney);
	this->showFold();
}

void playerWindow::callRaiseFoldAction(const int callMoney, const int minRaiseMoney, const int maxRaiseMoney) const {
	this->showCall(callMoney);
	this->showRaise(minRaiseMoney, maxRaiseMoney);
	this->showFold();
}

void playerWindow::checkRaiseFoldAction(const int minRaiseMoney, const int maxRaiseMoney) const {
	this->showCheck();
	this->showRaise(minRaiseMoney, maxRaiseMoney);
	this->showFold();
}

void playerWindow::hideAll() const {
	this->hideHandCards();
	this->hidePlayerName();
	this->hidePlayerChip();
	this->hidePlayerActionMessage();
	this->hideAllAction();
}
//player的槽函数
void playerWindow::playerReady() {
	this->begin->hide();
	this->showPlayerActionMessage(QStringLiteral("准备！"));
	emit beginClicked();
}

singleGameWindow::singleGameWindow(QWidget *parent, game* g)
	:QMainWindow(parent), m_game(g)
{
	setWindowTitle(QStringLiteral("德州扑克"));
	//this->m_game = g;
	if (this->objectName().isEmpty())
		this->setObjectName(QStringLiteral("TexasPokerNewClass"));
	this->resize(1200, 680);
	menuBar = new QMenuBar(this);
	menuBar->setObjectName(QStringLiteral("menuBar"));
	this->setMenuBar(menuBar);
	mainToolBar = new QToolBar(this);
	mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
	this->addToolBar(mainToolBar);
	centralWidget = new QWidget(this);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	this->setCentralWidget(centralWidget);
	statusBar = new QStatusBar(this);
	statusBar->setObjectName(QStringLiteral("statusBar"));
	this->setStatusBar(statusBar);

	QMenu* helpMenu = menuBar->addMenu(QStringLiteral("帮助(&H)"));

	QAction* aboutAct = new QAction(QIcon(QStringLiteral("image/tubiao/about.png")), QStringLiteral("关于..."), this);
	aboutAct->setStatusTip(QStringLiteral("关于"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	helpMenu->addAction(aboutAct);

	for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
		commonCards[i_card] = new QLabel(centralWidget);
		commonCards[i_card]->setObjectName(QStringLiteral("playerHandCard1_3"));
		commonCards[i_card]->setGeometry(QRect(commonCardsPosition[i_card][0], commonCardsPosition[i_card][1], 70, 100));
		QString cardFileName = getCardFileName(card(Club, Three));
		commonCards[i_card]->setPixmap(QPixmap(cardFileName));
		commonCards[i_card]->raise();
		commonCards[i_card]->hide();
	}

	QFont font;
	font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
	font.setPointSize(10);
	font.setBold(true);
	font.setWeight(75);

	flop = new QLabel(centralWidget);
	flop->setObjectName(QStringLiteral("flop"));
	flop->setGeometry(QRect(390, 230, 71, 31));
	flop->setFont(font);
	flop->setTextFormat(Qt::AutoText);
	flop->setScaledContents(false);
	flop->setAlignment(Qt::AlignCenter);
	flop->raise();
	flop->hide();

	turn = new QLabel(centralWidget);
	turn->setObjectName(QStringLiteral("turn"));
	turn->setGeometry(QRect(590, 230, 71, 31));
	turn->setFont(font);
	turn->setTextFormat(Qt::AutoText);
	turn->setScaledContents(false);
	turn->setAlignment(Qt::AlignCenter);
	turn->raise();
	turn->hide();

	river = new QLabel(centralWidget);
	river->setObjectName(QStringLiteral("river"));
	river->setGeometry(QRect(700, 230, 71, 31));
	river->setFont(font);
	river->setTextFormat(Qt::AutoText);
	river->setScaledContents(false);
	river->setAlignment(Qt::AlignCenter);
	river->raise();
	river->hide();

	pot = new QLabel(centralWidget);
	pot->setObjectName(QStringLiteral("底池"));
	pot->setGeometry(QRect(480, 370, 100, 30));
	pot->setFont(font);
	pot->setTextFormat(Qt::AutoText);
	pot->setScaledContents(false);
	pot->setAlignment(Qt::AlignCenter);
	pot->raise();
	pot->hide();

	//begin = new QPushButton(centralWidget);
	//begin->setObjectName(QStringLiteral("begin"));
	//begin->setGeometry(QRect(580, 370, 50, 30));
	//begin->raise();
	//begin->setFont(font);
	//begin->hide();

	flop->setText(QApplication::translate("QtWidgetsApplication1Class", "\347\277\273\347\211\214", Q_NULLPTR));
	turn->setText(QApplication::translate("QtWidgetsApplication1Class", "\350\275\254\347\211\214", Q_NULLPTR));
	river->setText(QApplication::translate("QtWidgetsApplication1Class", "\346\262\263\347\211\214", Q_NULLPTR));
	pot->setText(QStringLiteral("底池：") + QString::number(10));
	//begin->setText(QStringLiteral("开始"));
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		players[i_player] = new playerWindow(centralWidget, playerPosition[i_player][0], playerPosition[i_player][1], i_player);

		//connect(this->players[i_player]->raise, SIGNAL(clicked()), this, SLOT(testShow()));
		connect(this->players[i_player]->raise, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
		connect(this->players[i_player]->allin, SIGNAL(clicked()), this, SLOT(nowPlayerAllin()));
		connect(this->players[i_player]->check, SIGNAL(clicked()), this, SLOT(nowPlayerCheck()));
		connect(this->players[i_player]->call, SIGNAL(clicked()), this, SLOT(nowPlayerCall()));
		connect(this->players[i_player]->fold, SIGNAL(clicked()), this, SLOT(nowPlayerFold()));
		connect(this->players[i_player], SIGNAL(beginClicked()), this, SLOT(gameBegin()));
	}

	//connect(this->begin, SIGNAL(clicked()), this, SLOT(gameBegin()));	
}

void singleGameWindow::showCommonCards(vector<card> const & needShowCommonCards) const {
	for (int i_card = 0; i_card < (int)needShowCommonCards.size(); ++i_card) {
		if (needShowCommonCards[i_card].isCardVailid()) {
			QString cardFileName = getCardFileName(needShowCommonCards[i_card]);
			this->commonCards[i_card]->setPixmap(QPixmap(cardFileName));
			this->commonCards[i_card]->show();
		}
		else {
			//this->commonCards[i_card]->setPixmap(QPixmap());
			this->commonCards[i_card]->hide();
		}
	}
	for (int i_card = (int)needShowCommonCards.size(); i_card < game::maxNumOfCommonCards; ++i_card) {
		//this->commonCards[i_card]->setPixmap(QPixmap());
		this->commonCards[i_card]->hide();
	}
}

void singleGameWindow::hideCommonCards() const {
	for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
		this->commonCards[i_card]->hide();
	}
}

void singleGameWindow::showRound(gameRound nowRound) const {
	if (nowRound == gameRound::Flop) {
		this->flop->show();
	}
	else if (nowRound == gameRound::Turn) {
		this->turn->show();
	}
	else if (nowRound == gameRound::River) {
		this->river->show();
	}
	else {
		this->hideRound();
	}
}

void singleGameWindow::hideRound() const {
	this->flop->hide();
	this->turn->hide();
	this->river->hide();
}

void singleGameWindow::showPot(const int potNum) const {
	QString showText = QStringLiteral("底池：") + QString::number(potNum);
	this->pot->setText(showText);
	this->pot->show();
}

void singleGameWindow::hidePot() const {
	this->pot->hide();
}

//player show方法
void singleGameWindow::showPlayerHandCards(const int playerIndex, vector<card> const& handCards) const {
	this->players[playerIndex]->showHandCards(handCards);
}
void singleGameWindow::showPlayerName(const int playerIndex, string const& playerName) const {
	this->players[playerIndex]->showPlayerName(playerName);
}
void singleGameWindow::showPlayerChip(const int playerIndex, const int chip) const {
	this->players[playerIndex]->showPlayerChip(chip);
}
void singleGameWindow::showPlayerActionMessage(const int playerIndex, string const & actionMessage) const {
	this->players[playerIndex]->showPlayerActionMessage(QString::fromLocal8Bit(actionMessage.data()));
}

void singleGameWindow::showPlayerSidePot(const int playerIndex, const int money) const
{
	this->players[playerIndex]->showSidePot(money);
}

//void singleGameWindow::showPlayerNameCardsChip(const int playerIndex, player const& needShowPlayer) const {
//	this->showPlayerName(playerIndex, needShowPlayer.getName());
//	this->showPlayerHandCards(playerIndex, needShowPlayer.getHandCards());
//	this->showPlayerChip(playerIndex, needShowPlayer.getChip());
//}
//void singleGameWindow::playerCheckRaiseFoldAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney) const {
//	this->players[nowPlayerIndex]->checkRaiseFoldAction(minRaiseMoney, maxRaiseMoney);
//}
//
//void singleGameWindow::playerAllinFoldAction(const int nowPlayerIndex, const int allmoney) const {
//	this->players[nowPlayerIndex]->allinFoldAction(allmoney);
//}
//
//void singleGameWindow::playerCallRaiseFoldAction(const int nowPlayerIndex, const int callMoney, const int minRaiseMoney, const int maxRaiseMoney) const {
//	this->players[nowPlayerIndex]->callRaiseFoldAction(callMoney, minRaiseMoney, maxRaiseMoney);
//}
void singleGameWindow::showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney)const {
	this->players[nowPlayerIndex]->showRaise(minRaiseMoney, maxRaiseMoney);
}
void singleGameWindow::showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney) const
{
	this->players[nowPlayerIndex]->showAllin(allinMoney);
}
void singleGameWindow::showPlayerCheckAction(const int nowPlayerIndex)const {
	this->players[nowPlayerIndex]->showCheck();
}
void singleGameWindow::showPlayerCallAction(const int nowPlayerIndex, const int callMoney)const {
	this->players[nowPlayerIndex]->showCall(callMoney);
}
void singleGameWindow::showPlayerFoldAction(const int nowPlayerIndex)const {
	this->players[nowPlayerIndex]->showFold();
}

//player hide方法
void singleGameWindow::hidePlayerHandCards(const int playerIndex) const {
	this->players[playerIndex]->hideHandCards();
}
void singleGameWindow::hidePlayerName(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerName();
}
void singleGameWindow::hidePlayerChip(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerChip();
}
void singleGameWindow::hidePlayerActionMessage(const int playerIndex) const {
	this->players[playerIndex]->hidePlayerActionMessage();
}
void singleGameWindow::hidePlayerSidePot(const int playerIndex) const
{
	this->players[playerIndex]->hideSidePot();
}
//void singleGameWindow::hidePlayerNameCardsChip(const int playerIndex) const {
//	this->hidePlayerName(playerIndex);
//	this->hidePlayerHandCards(playerIndex);
//	this->hidePlayerChip(playerIndex);
//}
//void singleGameWindow::hidePlayerAction(const int playerIndex) const {
//	this->players[playerIndex]->hideAllAction();
//}
//void singleGameWindow::hidePlayer(const int playerIndex) const {
//	this->hidePlayerNameCardsChip(playerIndex);
//	this->hidePlayerActionMessage(playerIndex);
//	this->hidePlayerAction(playerIndex);
//}
void singleGameWindow::hidePlayerRaiseAction(const int playerIndex) const {
	this->players[playerIndex]->hideRaise();
}
void singleGameWindow::hidePlayerAllinAction(const int nowPlayerIndex) const
{
	this->players[nowPlayerIndex]->hideAllin();
}
void singleGameWindow::hidePlayerCheckAction(const int playerIndex) const {
	this->players[playerIndex]->hideCheck();
}
void singleGameWindow::hidePlayerCallAction(const int playerIndex) const {
	this->players[playerIndex]->hideCall();
}
void singleGameWindow::hidePlayerFoldAction(const int playerIndex) const {
	this->players[playerIndex]->hideFold();
}



//槽函数
void singleGameWindow::about() {
	QMessageBox::about(this
		, QStringLiteral("德州扑克")
		, QStringLiteral("<p>Version 1.0<p>Copyright &copy; 2021 <b>王砚平，浙江大学</b>")
	);
}
void singleGameWindow::nowPlayerRaise() {
	const int nowPlayerIndex = this->m_game->getNowPlayerIndex();
	const int raiseMoney = this->players[nowPlayerIndex]->raiseMoney->text().toInt();
	this->m_game->nowPlayerRaise(raiseMoney);
}
void singleGameWindow::nowPlayerAllin() {
	//const int nowPlayerIndex = this->m_game->getNowPlayerIndex();
	//const int allinMoney = this->players[nowPlayerIndex]->raiseMoney->text().toInt();
	this->m_game->nowPlayerAllin();
}
void singleGameWindow::nowPlayerCheck() {
	this->m_game->nowPlayerCheck();
}
void singleGameWindow::nowPlayerCall() {
	this->m_game->nowPlayerCall();
}
void singleGameWindow::nowPlayerFold() {
	this->m_game->nowPlayerFold();
}
void singleGameWindow::gameBegin() {
	this->m_game->addNumOfReadyPlayer();
	this->m_game->begin();
}