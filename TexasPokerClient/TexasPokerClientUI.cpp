#include "TexasPokerClientUI.h"

QString getCardFileName(card const& c) {
	cardColor color = c.getColor();
	cardNumber num = c.getNumber();
	if (color == cardColor::CardBackColor&&num == cardNumber::CardBackNumber) {
		QString cardFileName = QString::fromUtf8("image/poker/") + QString::fromUtf8("back") + QString::fromUtf8(".jpg");
		return cardFileName;
	}
	QString cardFileName = QString::fromUtf8("image/poker/") + QString::number((int)color) + QString::fromUtf8("_") + QString::number((int)(num / 10)) + QString::number((int)(num % 10)) + QString::fromUtf8(".jpg");
	return cardFileName;
}
vector<card> QByteArrayToCards(QByteArray const & qArray)
{
	QByteArray dataArray = qArray;
	vector<card> cards(0);
	while (dataArray.length() >= 2 * commandAndDataToClient::byteOfInt) {	//1张牌是颜色+数字，2个字节
		cardColor color = (cardColor)bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		cardNumber number = (cardNumber)bytes4ToInt(dataArray.mid(commandAndDataToClient::byteOfInt, commandAndDataToClient::byteOfInt));
		dataArray.remove(0, 2 * commandAndDataToClient::byteOfInt);
		cards.push_back(card(color, number));
	}
	return cards;
}
;

playerClient::playerClient(QWidget *centralWidget, int x, int y, int playerIndex) {
	for (int i = 0; i < player::numOfHandCards; ++i) {
		playerHandCards[i] = new QLabel(centralWidget);
		playerHandCards[i]->setObjectName(QStringLiteral("playerHandCard1"));
		playerHandCards[i]->setGeometry(QRect(handCardPosition[i][0] + x, handCardPosition[i][1] + y, 70, 100));
		playerHandCards[i]->setPixmap(QPixmap(QString::fromUtf8("image/poker/0_00.jpg")));
		playerHandCards[i]->raise();
		playerHandCards[i]->hide();
	}

	QFont font1;
	font1.setPointSize(10);
	font1.setBold(true);
	font1.setWeight(75);
	playerName = new QLabel(centralWidget);
	playerName->setObjectName(QStringLiteral("label_4"));
	playerName->setGeometry(QRect(0 + x, 0 + y, 100, 31));
	playerName->setFont(font1);
	playerName->setLayoutDirection(Qt::LeftToRight);
	//playerName->setAlignment(Qt::AlignCenter);

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

	playerName->raise();
	playerChip->raise();
	actionMessage->raise();
	sidePot->raise();

	playerName->hide();
	playerChip->hide();
	actionMessage->hide();
	sidePot->hide();

	playerName->setText(QStringLiteral("玩家1"));
	playerChip->setText(QStringLiteral("筹码："));
	actionMessage->setText(QStringLiteral("玩家行为："));
	sidePot->setText(QStringLiteral("边池："));
}

void playerClient::showHandCards(vector<card> const& c) const {
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

void playerClient::hideHandCards() const {
	for (int i_card = 0; i_card < player::numOfHandCards; ++i_card) {
		this->playerHandCards[i_card]->hide();
	}
}

void playerClient::showPlayerName(string const& name) const {
	QString showName = QString::fromLocal8Bit(name.data());
	this->playerName->setText(showName);
	this->playerName->show();
}

void playerClient::hidePlayerName() const {
	this->playerName->hide();
}

void playerClient::showPlayerChip(const int chip) const {
	this->playerChip->setText(QStringLiteral("筹码：") + QString::number(chip));
	this->playerChip->show();
}

void playerClient::hidePlayerChip() const {
	this->playerChip->hide();
}

void playerClient::showPlayerActionMessage(QString const & actionMessage) const {
	this->actionMessage->setText(actionMessage);
	this->actionMessage->show();
}

void playerClient::hidePlayerActionMessage() const {
	this->actionMessage->hide();
}

void playerClient::showSidePot(const int money) const
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

void playerClient::hideSidePot() const
{
	this->sidePot->hide();
}


TexasPokerClientUI::TexasPokerClientUI(QWidget *parent)
	:QMainWindow(parent)
{


	setWindowTitle(QStringLiteral("德州扑克"));
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

	QIcon icon;
	icon.addFile(QStringLiteral("image/tubiao/poker_window_tubiao.ico"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);

	QMenu* helpMenu = menuBar->addMenu(QStringLiteral("帮助(&H)"));

	QAction* aboutAct = new QAction(QIcon(QStringLiteral("image/tubiao/about.png")), QStringLiteral("关于..."), this);
	aboutAct->setStatusTip(QStringLiteral("关于"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutSlot()));
	helpMenu->addAction(aboutAct);

	QAction* aboutCardType = new QAction(QIcon(QStringLiteral("image/tubiao/about.png")), QStringLiteral("牌型"), this);
	aboutCardType->setStatusTip(QStringLiteral("关于"));
	connect(aboutCardType, SIGNAL(triggered()), this, SLOT(aboutCardTypeSlot()));
	helpMenu->addAction(aboutCardType);

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

	begin = new QPushButton(centralWidget);
	begin->setObjectName(QStringLiteral("begin"));
	begin->setGeometry(QRect(580, 370, 50, 30));
	begin->raise();
	begin->setFont(font);
	begin->hide();

	raise = new QPushButton(centralWidget);
	raise->setObjectName(QStringLiteral("raise"));
	raise->setGeometry(QRect(550, 470, 70, 23));
	allin = new QPushButton(centralWidget);
	allin->setObjectName(QStringLiteral("raise"));
	allin->setGeometry(QRect(550, 470, 70, 23));
	check = new QPushButton(centralWidget);
	check->setObjectName(QStringLiteral("check"));
	check->setGeometry(QRect(470, 470, 70, 23));
	call = new QPushButton(centralWidget);
	call->setObjectName(QStringLiteral("check"));
	call->setGeometry(QRect(470, 470, 70, 23));
	fold = new QPushButton(centralWidget);
	fold->setObjectName(QStringLiteral("fold"));
	fold->setGeometry(QRect(630, 470, 70, 23));
	raiseMoney = new QSpinBox(centralWidget);
	raiseMoney->setObjectName(QStringLiteral("spinBox"));
	raiseMoney->setGeometry(QRect(550, 440, 47, 22));
	raise->raise();
	allin->raise();
	check->raise();
	fold->raise();
	call->raise();
	raiseMoney->raise();
	raise->hide();
	allin->hide();
	check->hide();
	fold->hide();
	call->hide();
	raiseMoney->hide();

	raise->setText(QStringLiteral("加注"));
	allin->setText(QStringLiteral("allin"));
	check->setText(QStringLiteral("看牌"));
	fold->setText(QStringLiteral("弃牌"));
	call->setText(QStringLiteral("跟注"));
	flop->setText(QApplication::translate("QtWidgetsApplication1Class", "\347\277\273\347\211\214", Q_NULLPTR));
	turn->setText(QApplication::translate("QtWidgetsApplication1Class", "\350\275\254\347\211\214", Q_NULLPTR));
	river->setText(QApplication::translate("QtWidgetsApplication1Class", "\346\262\263\347\211\214", Q_NULLPTR));
	pot->setText(QStringLiteral("底池：") + QString::number(10));
	begin->setText(QStringLiteral("开始"));
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		players[i_player] = new playerClient(centralWidget, playerPosition[i_player][0], playerPosition[i_player][1], i_player);		
	}

	m_connect = new QPushButton(centralWidget);
	m_connect->setObjectName(QStringLiteral("connect"));
	m_connect->setGeometry(QRect(580, 370, 50, 30));
	m_connect->raise();
	m_connect->setText(QStringLiteral("连接"));
	m_connect->show();

	m_address = new QLineEdit(centralWidget);
	m_address->setObjectName(QStringLiteral("address"));
	m_address->setGeometry(QRect(500, 300, 150, 30));
	m_address->raise();
	m_address->setText(QStringLiteral("10.12.222.73"));
	m_address->show();

	m_port = new QLineEdit(centralWidget);
	m_port->setObjectName(QStringLiteral("port"));
	m_port->setGeometry(QRect(680, 300, 40, 30));
	m_port->raise();
	m_port->setText(QStringLiteral("6060"));
	m_port->show();

	m_name = new QLineEdit(centralWidget);
	m_name->setObjectName(QStringLiteral("playerName"));
	m_name->setGeometry(QRect(550, 230, 100, 30));
	m_name->raise();
	m_name->setText(QStringLiteral("姓名："));
	m_name->show();

	m_tcpClient = new QTcpSocket(this);
	m_tcpClient->abort();
	m_headLen = 0;
	m_buffer.clear();

	connect(m_connect, SIGNAL(clicked()), this, SLOT(connectTcp()));
	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(begin, SIGNAL(clicked()), this, SLOT(clientReady()));

	connect(raise, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
	connect(allin, SIGNAL(clicked()), this, SLOT(nowPlayerAllin()));
	connect(check, SIGNAL(clicked()), this, SLOT(nowPlayerCheck()));
	connect(call, SIGNAL(clicked()), this, SLOT(nowPlayerCall()));
	connect(fold, SIGNAL(clicked()), this, SLOT(nowPlayerFold()));
}

//好累。。。
void TexasPokerClientUI::analyzeCommand(QByteArray received)
{
	QByteArray commandArray = received.left(commandAndDataToClient::byteOfInt);
	QByteArray dataArray = received;
	dataArray.remove(0, commandAndDataToClient::byteOfInt);
	
	tcpCommandToClient receivedCommand = (tcpCommandToClient)bytes4ToInt(commandArray);

	if (receivedCommand&tcpCommandToClient::noCommandToClient) {
		return;
	}
	else if (receivedCommand&tcpCommandToClient::showCommonCardsCommand) {
		//vector<card> commonCards(0);
		//while (dataArray.length() >= 2*commandAndDataToClient::byteOfInt) {	//1张牌是颜色+数字，2个字节
		//	cardColor color = (cardColor)bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		//	cardNumber number = (cardNumber)bytes4ToInt(dataArray.mid(commandAndDataToClient::byteOfInt, commandAndDataToClient::byteOfInt));
		//	dataArray.remove(0, 2 * commandAndDataToClient::byteOfInt);
		//	commonCards.push_back(card(color, number));
		//}
		vector<card> commonCards = QByteArrayToCards(dataArray);
		this->showCommonCards(commonCards);
	}
	else if (receivedCommand&tcpCommandToClient::hideCommonCardsCommand) {
		this->hideCommonCards();
	}
	else if (receivedCommand&tcpCommandToClient::showRoundCommand) {
		gameRound round = (gameRound)bytes4ToInt(dataArray);
		this->showRound(round);
	}
	else if (receivedCommand&tcpCommandToClient::hideRoundCommand) {
		this->hideRound();
	}
	else if (receivedCommand&tcpCommandToClient::showPotCommand) {
		const int pot = bytes4ToInt(dataArray);
		this->showPot(pot);
	}
	else if (receivedCommand&tcpCommandToClient::hidePotCommand) {
		this->hidePot();
	}
	else if (receivedCommand&tcpCommandToClient::showBeginCommand) {
		this->showBegin();
	}
	else if (receivedCommand&tcpCommandToClient::hideBeginCommand) {
		this->hideBegin();
	}
	else if (receivedCommand&tcpCommandToClient::showPlayerHandCardsCommand) {	//playerIndex + cards
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		vector<card> handCards = QByteArrayToCards(dataArray);
		this->showPlayerHandCards(playerIndex, handCards);
	}
	else if (receivedCommand&tcpCommandToClient::showPlayerNameCommand) {//playerIndex + name
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const string playerName = dataArray.toStdString();
		this->showPlayerName(playerIndex, playerName);
	}
	else if (receivedCommand&tcpCommandToClient::showPlayerChipCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int chip = bytes4ToInt(dataArray);
		this->showPlayerChip(playerIndex, chip);
	}
	else if (receivedCommand&tcpCommandToClient::showPlayerActionMessageCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const string actionMessage = dataArray.toStdString();
		this->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (receivedCommand&tcpCommandToClient::showPlayerSidePotCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int sidePot = bytes4ToInt(dataArray);
		this->showPlayerSidePot(playerIndex, sidePot);
	}
	else if (receivedCommand&tcpCommandToClient::hidePlayerHandCardsCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerHandCards(playerIndex);
	}
	else if (receivedCommand&tcpCommandToClient::hidePlayerNameCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerName(playerIndex);
	}
	else if (receivedCommand&tcpCommandToClient::hidePlayerChipCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerChip(playerIndex);
	}
	else if (receivedCommand&tcpCommandToClient::hidePlayerActionMessageCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerActionMessage(playerIndex);
	}
	else if (receivedCommand&tcpCommandToClient::hidePlayerSidePotCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerSidePot(playerIndex);
	}
	else if (receivedCommand&tcpCommandToClient::showClientPlayerRaiseActionCommand) {
		QByteArray minMoneyArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int minMoney = bytes4ToInt(minMoneyArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int maxMoney = bytes4ToInt(dataArray);
		this->showClientPlayerRaiseAction(minMoney, maxMoney);
	}
	else if (receivedCommand&tcpCommandToClient::showClientPlayerAllinActionCommand) {
		const int allMoney = bytes4ToInt(dataArray);
		this->showClientPlayerAllinAction(allMoney);
	}
	else if (receivedCommand&tcpCommandToClient::showClientPlayerCheckActionCommand) {
		this->showClientPlayerCheckAction();
	}
	else if (receivedCommand&tcpCommandToClient::showClientPlayerCallActionCommand) {
		const int callMoney = bytes4ToInt(dataArray);
		this->showClientPlayerCallAction(callMoney);
	}
	else if (receivedCommand&tcpCommandToClient::showClientPlayerFoldActionCommand) {
		this->showClientPlayerFoldAction();
	}
	else if (receivedCommand&tcpCommandToClient::hideClientPlayerRaiseActionCommand) {
		this->hideClientPlayerRaiseAction();
	}
	else if (receivedCommand&tcpCommandToClient::hideClientPlayerAllinActionCommand) {
		this->hideClientPlayerAllinAction();
	}
	else if (receivedCommand&tcpCommandToClient::hideClientPlayerCheckActionCommand) {
		this->hideClientPlayerCheckAction();
	}
	else if (receivedCommand&tcpCommandToClient::hideClientPlayerCallActionCommand) {
		this->hideClientPlayerCallAction();
	}
	else if (receivedCommand&tcpCommandToClient::hideClientPlayerFoldActionCommand) {
		this->hideClientPlayerFoldAction();
	}
	else if (receivedCommand&tcpCommandToClient::setClientPlayerIndex) {
		const int index = bytes4ToInt(dataArray);
		this->setClientPlayerIndex(index);
	}
}

void TexasPokerClientUI::sendCommandAndDataToServer(commandAndDataToServer toSend) const
{
	this->m_tcpClient->write(toSend.getTcpSend());
}

void TexasPokerClientUI::showCommonCards(vector<card> const & needShowCommonCards) const {
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

void TexasPokerClientUI::hideCommonCards() const {
	for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
		this->commonCards[i_card]->hide();
	}
}

void TexasPokerClientUI::showRound(gameRound nowRound) const {
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

void TexasPokerClientUI::hideRound() const {
	this->flop->hide();
	this->turn->hide();
	this->river->hide();
}

void TexasPokerClientUI::showPot(const int potNum) const {
	QString showText = QStringLiteral("底池：") + QString::number(potNum);
	this->pot->setText(showText);
	this->pot->show();
}

void TexasPokerClientUI::hidePot() const {
	this->pot->hide();
}

//player show方法
//考虑了当前client编号，假设当前x，需要显示y，则实际在本client显示的位置应该是(y-x+8)%8
void TexasPokerClientUI::showPlayerHandCards(const int playerIndex, vector<card> const& handCards) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showHandCards(handCards);
}
void TexasPokerClientUI::showPlayerName(const int playerIndex, string const& playerName) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showPlayerName(playerName);
}
void TexasPokerClientUI::showPlayerChip(const int playerIndex, const int chip) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showPlayerChip(chip);
}
void TexasPokerClientUI::showPlayerActionMessage(const int playerIndex, string const & actionMessage) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showPlayerActionMessage(QString::fromLocal8Bit(actionMessage.data()));
}

void TexasPokerClientUI::showPlayerSidePot(const int playerIndex, const int money) const
{
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showSidePot(money);
}

//player hide方法
void TexasPokerClientUI::hidePlayerHandCards(const int playerIndex) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->hideHandCards();
}
void TexasPokerClientUI::hidePlayerName(const int playerIndex) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->hidePlayerName();
}
void TexasPokerClientUI::hidePlayerChip(const int playerIndex) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->hidePlayerChip();
}
void TexasPokerClientUI::hidePlayerActionMessage(const int playerIndex) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->hidePlayerActionMessage();
}
void TexasPokerClientUI::hidePlayerSidePot(const int playerIndex) const
{
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->hideSidePot();
}

void TexasPokerClientUI::showClientPlayerRaiseAction(const int minRaiseMoney, const int maxRaiseMoney)const {
	this->raise->show();		//显示按键
	this->raiseMoney->setRange(minRaiseMoney, maxRaiseMoney);	//设置范围
	this->raiseMoney->setValue(minRaiseMoney);					//初值设为最小值
	this->raiseMoney->setSingleStep(game::bigBind);				//设置最少一次改变多少
	this->raiseMoney->show();
	//this->hidePlayerActionMessage(this->m_clientPlayerIndex);
}
void TexasPokerClientUI::showClientPlayerAllinAction(const int allinMoney) const
{
	this->allin->show();
	this->raiseMoney->setRange(allinMoney, allinMoney);
	this->raiseMoney->show();
	//this->hidePlayerActionMessage(this->m_clientPlayerIndex);
}
void TexasPokerClientUI::showClientPlayerCheckAction()const {
	this->check->show();
	//this->hidePlayerActionMessage(this->m_clientPlayerIndex);
}
void TexasPokerClientUI::showClientPlayerCallAction(const int callMoney)const {
	QString showText = QStringLiteral("跟注:") + QString::number(callMoney);
	this->call->setText(showText);
	this->call->show();
	//this->hidePlayerActionMessage(this->m_clientPlayerIndex);
}
void TexasPokerClientUI::showClientPlayerFoldAction()const {
	this->fold->show();
	this->hidePlayerActionMessage(this->m_clientPlayerIndex);
}

void TexasPokerClientUI::hideClientPlayerRaiseAction() const {
	this->raise->hide();
	this->raiseMoney->hide();
}
void TexasPokerClientUI::hideClientPlayerAllinAction() const
{
	this->allin->hide();
}
void TexasPokerClientUI::hideClientPlayerCheckAction() const {
	this->check->hide();
}
void TexasPokerClientUI::hideClientPlayerCallAction() const {
	this->call->hide();
}
void TexasPokerClientUI::hideClientPlayerFoldAction() const {
	this->fold->hide();
}

//槽函数
void TexasPokerClientUI::connectTcp() {
	QString receivedAddress = m_address->text();
	QHostAddress honstAddress;
	if (!honstAddress.setAddress(receivedAddress)){
		QMessageBox::about(this, QStringLiteral("连接失败"), QStringLiteral("无效的IP地址！"));
		return;
	}
	//auto a = testAddress.toIPv4Address();
	QString receivedPort = m_port->text();
	quint32 hostPort = receivedPort.toInt();
	//quint16 hostPort = 6060;
	m_tcpClient->connectToHost(honstAddress, hostPort);
	const bool connected = m_tcpClient->waitForConnected(1000);
	if (!connected) {
		QMessageBox::about(this, QStringLiteral("连接失败"), QStringLiteral("服务器未响应"));
		return;
	}
	//连接成功，出现自己名字和开始按键，应该由服务器发送
	connect(m_tcpClient, SIGNAL(disconnected()), this, SLOT(disconnectTcp()));	
	string playerName(m_name->text().toLocal8Bit());	//防止乱码
	commandAndDataToServer toSend(tcpCommandToServer::setPlayerNameCommand, playerName);
	this->sendCommandAndDataToServer(toSend);	//设置自己的名字
	//关闭ip地址框和connect
	this->m_address->hide();
	this->m_port->hide();
	this->m_connect->hide();
	this->m_name->hide();
}
void TexasPokerClientUI::nowPlayerRaise()
{
	const int raiseMoney = this->raiseMoney->text().toInt();
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerRaiseCommand, raiseMoney);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerAllin()
{
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerAllinCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerCheck()
{
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerCheckCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerCall()
{
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerCallCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerFold()
{
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerFoldCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::clientReady()
{
	commandAndDataToServer toSend(tcpCommandToServer::playerReadyCommand);
	this->sendCommandAndDataToServer(toSend);
	this->hideBegin();		//自己隐藏了
}

void TexasPokerClientUI::readData() {			//可能得加个while循环
	QByteArray buffer = m_tcpClient->readAll();	//用readyRead触发已经读空
	this->m_buffer = this->m_buffer.append(buffer);	
	
	while (1) {		//可能一次读了多个包，加了个循环，不知道对不对
		if (this->m_headLen <= 0) {	//包体长度无意义，需要读取包体长度
			if (this->m_buffer.length() < 4)		//前4字节是包体长度,不够就等下次
				return;
			QByteArray head = this->m_buffer.left(4);
			this->m_buffer.remove(0, 4);
			this->m_headLen = bytes4ToInt(head);
		}
		const int len = this->m_buffer.length();
		if (len < this->m_headLen) {	//数据长度不够，等下次
			return;
		}
		QByteArray commandAndDataArray = this->m_buffer.left(this->m_headLen);
		this->m_buffer.remove(0, this->m_headLen);
		this->m_headLen = 0;						//读完数据，重置包体长度

		//根据命令进行操作了
		this->analyzeCommand(commandAndDataArray);
	}
}

void TexasPokerClientUI::disconnectTcp() {
	QMessageBox::about(this, QStringLiteral("连接失败"), QStringLiteral("服务器已断开连接"));
	//this->close();
	QApplication::exit();
}


void TexasPokerClientUI::aboutSlot() {
	QMessageBox::about(this
		, QStringLiteral("德州扑克")
		, QStringLiteral("<p>Version 1.0<p>Copyright &copy; 2021 <b>wyp zju</b>")
	);
}

void TexasPokerClientUI::aboutCardTypeSlot() {
	QMainWindow *newWindow = new QMainWindow(this);

	newWindow->setWindowTitle(QStringLiteral("牌型"));
	if (newWindow->objectName().isEmpty())
		newWindow->setObjectName(QStringLiteral("牌型"));
	newWindow->resize(700, 600);
	menuBar = new QMenuBar(newWindow);
	menuBar->setObjectName(QStringLiteral("menuBar"));
	newWindow->setMenuBar(menuBar);
	mainToolBar = new QToolBar(newWindow);
	mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
	newWindow->addToolBar(mainToolBar);
	centralWidget = new QWidget(newWindow);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	newWindow->setCentralWidget(centralWidget);
	statusBar = new QStatusBar(newWindow);
	statusBar->setObjectName(QStringLiteral("statusBar"));
	newWindow->setStatusBar(statusBar);
	
	QPushButton * test = new QPushButton(centralWidget);
	test->show();

	QLabel *imageShow = new QLabel(centralWidget);
	QString path = "image/tubiao/cardType.jpg";
	imageShow->setPixmap(path);
	imageShow->raise();
	imageShow->show();
	
	newWindow->show();

}

