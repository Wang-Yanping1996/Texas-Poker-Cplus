#include "TexasPokerClientUI.h"

QString getCardFileName(card const& c) {
	cardColor color = c.getColor();
	cardNumber num = c.getNumber();
	QString cardFileName = QString::fromUtf8("image/poker/") + QString::number((int)color) + QString::fromUtf8("_") + QString::number((int)(num / 10)) + QString::number((int)(num % 10)) + QString::fromUtf8(".jpg");
	return cardFileName;
}
vector<card> QByteArrayToCards(QByteArray const & qArray)
{
	QByteArray dataArray = qArray;
	vector<card> cards(0);
	while (dataArray.length() >= 2 * commandAndData::byteOfInt) {	//1张牌是颜色+数字，2个字节
		cardColor color = (cardColor)bytes4ToInt(dataArray.left(commandAndData::byteOfInt));
		cardNumber number = (cardNumber)bytes4ToInt(dataArray.mid(commandAndData::byteOfInt, commandAndData::byteOfInt));
		dataArray.remove(0, 2 * commandAndData::byteOfInt);
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

	begin = new QPushButton(centralWidget);
	begin->setObjectName(QStringLiteral("begin"));
	begin->setGeometry(QRect(580, 370, 50, 30));
	begin->raise();
	begin->setFont(font);
	begin->hide();

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

	m_tcpClient = new QTcpSocket(this);
	m_tcpClient->abort();
	m_headLen = 0;
	m_buffer.clear();

	connect(m_connect, SIGNAL(clicked()), this, SLOT(connectTcp()));
	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));

}

//好累。。。
void TexasPokerClientUI::analyzeCommand(QByteArray received)
{
	QByteArray commandArray = received.left(commandAndData::byteOfInt);
	QByteArray dataArray = received;
	dataArray.remove(0, commandAndData::byteOfInt);
	
	tcpCommand receivedCommand = (tcpCommand)bytes4ToInt(commandArray);

	if (receivedCommand&tcpCommand::noCommand) {
		return;
	}
	else if (receivedCommand&tcpCommand::showCommonCardsCommand) {
		//vector<card> commonCards(0);
		//while (dataArray.length() >= 2*commandAndData::byteOfInt) {	//1张牌是颜色+数字，2个字节
		//	cardColor color = (cardColor)bytes4ToInt(dataArray.left(commandAndData::byteOfInt));
		//	cardNumber number = (cardNumber)bytes4ToInt(dataArray.mid(commandAndData::byteOfInt, commandAndData::byteOfInt));
		//	dataArray.remove(0, 2 * commandAndData::byteOfInt);
		//	commonCards.push_back(card(color, number));
		//}
		vector<card> commonCards = QByteArrayToCards(dataArray);
		this->showCommonCards(commonCards);
	}
	else if (receivedCommand&tcpCommand::hideCommonCardsCommand) {
		this->hideCommonCards();
	}
	else if (receivedCommand&tcpCommand::showRoundCommand) {
		gameRound round = (gameRound)bytes4ToInt(dataArray);
		this->showRound(round);
	}
	else if (receivedCommand&tcpCommand::hideRoundCommand) {
		this->hideRound();
	}
	else if (receivedCommand&tcpCommand::showPotCommand) {
		const int pot = bytes4ToInt(dataArray);
		this->showPot(pot);
	}
	else if (receivedCommand&tcpCommand::hidePotCommand) {
		this->hidePot();
	}
	else if (receivedCommand&tcpCommand::showBeginCommand) {
		this->showBegin();
	}
	else if (receivedCommand&tcpCommand::hideBeginCommand) {
		this->hideBegin();
	}
	else if (receivedCommand&tcpCommand::showPlayerHandCardsCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndData::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		vector<card> handCards = QByteArrayToCards(dataArray);
		this->showPlayerHandCards(playerIndex, handCards);
	}
	else if (receivedCommand&tcpCommand::showPlayerNameCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndData::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		const string playerName = dataArray.toStdString();
		this->showPlayerName(playerIndex, playerName);
	}
	else if (receivedCommand&tcpCommand::showPlayerChipCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndData::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		const int chip = bytes4ToInt(dataArray);
		this->showPlayerChip(playerIndex, chip);
	}
	else if (receivedCommand&tcpCommand::showPlayerActionMessageCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndData::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		const string actionMessage = dataArray.toStdString();
		this->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (receivedCommand&tcpCommand::showPlayerSidePotCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndData::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		const int sidePot = bytes4ToInt(dataArray);
		this->showPlayerSidePot(playerIndex, sidePot);
	}
	else if (receivedCommand&tcpCommand::hidePlayerHandCardsCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerHandCards(playerIndex);
	}
	else if (receivedCommand&tcpCommand::hidePlayerNameCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerName(playerIndex);
	}
	else if (receivedCommand&tcpCommand::hidePlayerChipCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerChip(playerIndex);
	}
	else if (receivedCommand&tcpCommand::hidePlayerActionMessageCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerActionMessage(playerIndex);
	}
	else if (receivedCommand&tcpCommand::hidePlayerSidePotCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerSidePot(playerIndex);
	}
	else if (receivedCommand&tcpCommand::showClientPlayerRaiseActionCommand) {
		QByteArray minMoneyArray = dataArray.left(commandAndData::byteOfInt);
		const int minMoney = bytes4ToInt(minMoneyArray);
		dataArray.remove(0, commandAndData::byteOfInt);
		const int maxMoney = bytes4ToInt(dataArray);
		this->showClientPlayerRaiseAction(minMoney, maxMoney);
	}
	else if (receivedCommand&tcpCommand::showClientPlayerAllinActionCommand) {
		const int allMoney = bytes4ToInt(dataArray);
		this->showClientPlayerAllinAction(allMoney);
	}
	else if (receivedCommand&tcpCommand::showClientPlayerCheckActionCommand) {
		this->showClientPlayerCheckAction();
	}
	else if (receivedCommand&tcpCommand::showClientPlayerCallActionCommand) {
		const int callMoney = bytes4ToInt(dataArray);
		this->showClientPlayerCallAction(callMoney);
	}
	else if (receivedCommand&tcpCommand::showClientPlayerFoldActionCommand) {
		this->showClientPlayerFoldAction();
	}
	else if (receivedCommand&tcpCommand::hideClientPlayerRaiseActionCommand) {
		this->hideClientPlayerRaiseAction();
	}
	else if (receivedCommand&tcpCommand::hideClientPlayerAllinActionCommand) {
		this->hideClientPlayerAllinAction();
	}
	else if (receivedCommand&tcpCommand::hideClientPlayerCheckActionCommand) {
		this->hideClientPlayerCheckAction();
	}
	else if (receivedCommand&tcpCommand::hideClientPlayerCallActionCommand) {
		this->hideClientPlayerCallAction();
	}
	else if (receivedCommand&tcpCommand::hideClientPlayerFoldActionCommand) {
		this->hideClientPlayerFoldAction();
	}
	else if (receivedCommand&tcpCommand::setClientPlayerIndex) {
		const int index = bytes4ToInt(dataArray);
		this->setClientPlayerIndex(index);
	}
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
}
void TexasPokerClientUI::showClientPlayerAllinAction(const int allinMoney) const
{
	this->allin->show();
	this->raiseMoney->setRange(allinMoney, allinMoney);
	this->raiseMoney->show();
}
void TexasPokerClientUI::showClientPlayerCheckAction()const {
	this->check->show();
}
void TexasPokerClientUI::showClientPlayerCallAction(const int callMoney)const {
	QString showText = QStringLiteral("跟注:") + QString::number(callMoney);
	this->call->setText(showText);
	this->call->show();
}
void TexasPokerClientUI::showClientPlayerFoldAction()const {
	this->fold->show();
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
		statusBar->showMessage(QStringLiteral("无效的IP地址！"));
		return;
	}
	//auto a = testAddress.toIPv4Address();
	quint16 hostPort = 6060;
	m_tcpClient->connectToHost(honstAddress, hostPort);
	const bool connected = m_tcpClient->waitForConnected();
	if (!connected) {
		statusBar->showMessage(QStringLiteral("连接失败！"));
		return;
	}
	//连接成功，出现自己名字和开始按键，应该由服务器发送
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

