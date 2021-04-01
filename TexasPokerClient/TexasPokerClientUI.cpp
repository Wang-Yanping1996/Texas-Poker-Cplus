#include "TexasPokerClientUI.h"

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
//CSDN找的函数
QString elidedLineText(QWidget *pWidget, QString strText)
{
	QFontMetrics fontMetrics(pWidget->font());
	QStringList strListLine;
	const int margin = fontMetrics.width("中");
	//满行的都加进列表
	for (int i = 0; i < strText.size(); i++) {
		if (fontMetrics.width(strText.left(i)) >= pWidget->width() - margin) { 
			strListLine.append(strText.left(i));
			strText = strText.right(strText.size() - i);
			i = 0;
		}
	}
	//最后不足一行的也加进去
	if (!strText.isEmpty()) {
		strListLine.append(strText);
	}
	QString strResult = strListLine.join("\n");

	return strResult;
}

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
	actionMessage->setGeometry(QRect(40 + x, 26 + y, 160, 40));
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
	//playerName->setAlignment(Qt::AlignCenter);
	playerChip->setText(QStringLiteral("筹码："));
	actionMessage->setText(QStringLiteral("玩家行为："));
	sidePot->setText(QStringLiteral("边池："));
}

void playerClient::showHandCards(vector<card> const& c) const {
	for (int i_card = 0; i_card < (int)c.size(); ++i_card) {
		if (c[i_card].isCardVailid()) {
			QString cardFileName = card::getCardFileName(c[i_card]);
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
		this->setObjectName(QStringLiteral("TexasPoker"));
	this->resize(1600, 720);					//宽，高，原1200*720，为了加聊天框扩展到1600*720
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

	QAction *showScoreChart = new QAction(menuBar);
	showScoreChart->setText(QStringLiteral("计分板"));
	menuBar->addAction(showScoreChart);
	connect(showScoreChart, SIGNAL(triggered()), this, SLOT(showScoreChartSlot()));

	for (int i_card = 0; i_card < game::maxNumOfCommonCards; ++i_card) {
		commonCards[i_card] = new QLabel(centralWidget);
		commonCards[i_card]->setObjectName(QStringLiteral("playerHandCard1_3"));
		commonCards[i_card]->setGeometry(QRect(commonCardsPosition[i_card][0], commonCardsPosition[i_card][1], 70, 100));
		QString cardFileName = card::getCardFileName(card(ErrorColor, ErrorNumber));
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

	dealer = new QLabel(centralWidget);
	dealer->setObjectName(QStringLiteral("dealer"));
	dealer->setText(QStringLiteral("dealer"));
	dealer->setFont(font);
	dealer->setTextFormat(Qt::AutoText);
	dealer->setScaledContents(false);
	dealer->setAlignment(Qt::AlignCenter);
	dealer->raise();
	dealer->hide();

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
	m_address->setPlaceholderText(QStringLiteral("请输入主机IPv4地址"));
	m_address->show();

	m_port = new QLineEdit(centralWidget);
	m_port->setObjectName(QStringLiteral("port"));
	m_port->setGeometry(QRect(680, 300, 60, 30));
	m_port->raise();
	m_port->setText(QStringLiteral("6060"));
	m_port->setPlaceholderText(QStringLiteral("端口号"));
	m_port->show();

	m_name = new QLineEdit(centralWidget);
	m_name->setObjectName(QStringLiteral("playerName"));
	m_name->setGeometry(QRect(550, 230, 100, 30));
	m_name->raise();
	//m_name->setText(QStringLiteral("姓名："));
	m_name->setMaxLength(8);
	m_name->setPlaceholderText(QStringLiteral("请输入姓名"));
	m_name->show();

	m_tcpClient = new QTcpSocket(this);
	m_tcpClient->abort();
	m_tcpPackageAnalyzer.clear();
	//m_headLen = 0;
	//m_buffer.clear();

	//打开客户端时获取本机第一个mac地址
	if (!this->getMacByGetAdaptersInfo()) {
		QMessageBox::about(this, QStringLiteral("错误"), QStringLiteral("获取本机地址失败"));
		QApplication::exit();
	}
	//定时器设置
	m_clock = new QTimer(this);
	m_clock->stop();			//默认是停止
	m_resTime = INT_MAX;
	m_nowActionPlayer = -1;		//默认当前玩家无效
	m_clientResTimeShow = new QLabel(this);
	m_clientResTimeShow->setGeometry(QRect(470, 440, 150, 23));
	m_clientResTimeShow->hide();
	connect(this->m_clock, SIGNAL(timeout()), this, SLOT(timerEventSlot()));

	connect(m_connect, SIGNAL(clicked()), this, SLOT(connectTcp()));
	connect(m_tcpClient, SIGNAL(disconnected()), this, SLOT(disconnectTcp()));	//从tcp连接成功处移到这里

	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(begin, SIGNAL(clicked()), this, SLOT(clientReady()));

	connect(raise, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
	connect(allin, SIGNAL(clicked()), this, SLOT(nowPlayerAllin()));
	connect(check, SIGNAL(clicked()), this, SLOT(nowPlayerCheck()));
	connect(call, SIGNAL(clicked()), this, SLOT(nowPlayerCall()));
	connect(fold, SIGNAL(clicked()), this, SLOT(nowPlayerFold()));

#ifdef PRINT_RECORD
	try {
		m_recorder = spdlog::rotating_logger_mt("GameRecord", "logs\\牌谱.txt", 1024 * 1024 * 5, 1);
	}
	catch (const spdlog::spdlog_ex &ex) {
		QMessageBox::about(this, QStringLiteral("错误"), QStringLiteral("创建日志失败"));
		QApplication::exit();
	}

	if (m_recorder != nullptr) {
		m_recorder->set_level(spdlog::level::info);
		m_recorder->info("日志创建成功！");
	}
#endif // PRINT_RECORD
}

bool TexasPokerClientUI::getMacByGetAdaptersInfo(){
	bool ret = false;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
		return false;
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
			return false;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			// 确保是以太网
			if (pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if (pAdapter->AddressLength != 6)
				continue;
			char acMAC[32];
			sprintf_s(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pAdapter->Address[0]),
				int(pAdapter->Address[1]),
				int(pAdapter->Address[2]),
				int(pAdapter->Address[3]),
				int(pAdapter->Address[4]),
				int(pAdapter->Address[5]));
			this->macAddress = acMAC;
			ret = true;
			break;
		}
	}

	free(pAdapterInfo);
	return ret;
}

void TexasPokerClientUI::startTimer(const int playerIndex, const int sec)
{
	this->m_nowActionPlayer = playerIndex;
	this->m_resTime = sec;
	if (this->m_resTime != infiniteTime) {		//不为-1才是有效设定，否则按无限时长处理
		this->timerEventSlot();					//先显示一下，似乎比设定长一秒？
		this->m_clock->start(1000);
	}
	else {										//否则就不带时间显示，本机如果是NowPlayer的话，Message会在showFold时隐藏，所以不必额外判断。
		this->showPlayerActionMessage(playerIndex, "玩家行动中...");
	}
}

//timer计时相关
void TexasPokerClientUI::stopTimer()
{
	this->m_clock->stop();
	this->m_resTime = INT_MAX;
	this->m_clientResTimeShow->hide();
}

//分支应该写成==判断
//好累。。。
void TexasPokerClientUI::analyzeCommand(QByteArray received)
{
	QByteArray commandArray = received.left(commandAndDataToClient::byteOfInt);
	QByteArray dataArray = received;
	dataArray.remove(0, commandAndDataToClient::byteOfInt);
	
	tcpCommandToClient receivedCommand = (tcpCommandToClient)bytes4ToInt(commandArray);

	if (receivedCommand == tcpCommandToClient::noCommandToClient) {
		return;
	}
	else if (receivedCommand == tcpCommandToClient::showCommonCardsCommand) {
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
	else if (receivedCommand == tcpCommandToClient::hideCommonCardsCommand) {
		this->hideCommonCards();
	}
	else if (receivedCommand == tcpCommandToClient::showRoundCommand) {
		gameRound round = (gameRound)bytes4ToInt(dataArray);
		this->showRound(round);
	}
	else if (receivedCommand == tcpCommandToClient::hideRoundCommand) {
		this->hideRound();
	}
	else if (receivedCommand == tcpCommandToClient::showPotCommand) {
		const int pot = bytes4ToInt(dataArray);
		this->showPot(pot);
	}
	else if (receivedCommand == tcpCommandToClient::hidePotCommand) {
		this->hidePot();
	}
	else if (receivedCommand == tcpCommandToClient::showBeginCommand) {
		this->showBegin();
	}
	else if (receivedCommand == tcpCommandToClient::hideBeginCommand) {
		this->hideBegin();
	}
	else if (receivedCommand == tcpCommandToClient::showPlayerHandCardsCommand) {	//playerIndex + cards
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		vector<card> handCards = QByteArrayToCards(dataArray);
		this->showPlayerHandCards(playerIndex, handCards);
	}
	else if (receivedCommand == tcpCommandToClient::showPlayerNameCommand) {//playerIndex + name
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const string playerName = dataArray.toStdString();
		this->showPlayerName(playerIndex, playerName);
	}
	else if (receivedCommand == tcpCommandToClient::showPlayerChipCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int chip = bytes4ToInt(dataArray);
		this->showPlayerChip(playerIndex, chip);
	}
	else if (receivedCommand == tcpCommandToClient::showPlayerActionMessageCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const string actionMessage = dataArray.toStdString();
		this->showPlayerActionMessage(playerIndex, actionMessage);
	}
	else if (receivedCommand == tcpCommandToClient::showPlayerSidePotCommand) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int sidePot = bytes4ToInt(dataArray);
		this->showPlayerSidePot(playerIndex, sidePot);
	}
	else if (receivedCommand == tcpCommandToClient::hidePlayerHandCardsCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerHandCards(playerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::hidePlayerNameCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerName(playerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::hidePlayerChipCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerChip(playerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::hidePlayerActionMessageCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerActionMessage(playerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::hidePlayerSidePotCommand) {
		const int playerIndex = bytes4ToInt(dataArray);
		this->hidePlayerSidePot(playerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::showClientPlayerRaiseActionCommand) {
		QByteArray minMoneyArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int minMoney = bytes4ToInt(minMoneyArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int maxMoney = bytes4ToInt(dataArray);
		this->showClientPlayerRaiseAction(minMoney, maxMoney);
	}
	else if (receivedCommand == tcpCommandToClient::showClientPlayerAllinActionCommand) {
		const int allMoney = bytes4ToInt(dataArray);
		this->showClientPlayerAllinAction(allMoney);
	}
	else if (receivedCommand == tcpCommandToClient::showClientPlayerCheckActionCommand) {
		this->showClientPlayerCheckAction();
	}
	else if (receivedCommand == tcpCommandToClient::showClientPlayerCallActionCommand) {
		const int callMoney = bytes4ToInt(dataArray);
		this->showClientPlayerCallAction(callMoney);
	}
	else if (receivedCommand == tcpCommandToClient::showClientPlayerFoldActionCommand) {
		this->showClientPlayerFoldAction();
	}
	else if (receivedCommand == tcpCommandToClient::hideClientPlayerRaiseActionCommand) {
		this->hideClientPlayerRaiseAction();
	}
	else if (receivedCommand == tcpCommandToClient::hideClientPlayerAllinActionCommand) {
		this->hideClientPlayerAllinAction();
	}
	else if (receivedCommand == tcpCommandToClient::hideClientPlayerCheckActionCommand) {
		this->hideClientPlayerCheckAction();
	}
	else if (receivedCommand == tcpCommandToClient::hideClientPlayerCallActionCommand) {
		this->hideClientPlayerCallAction();
	}
	else if (receivedCommand == tcpCommandToClient::hideClientPlayerFoldActionCommand) {
		this->hideClientPlayerFoldAction();
	}
	else if (receivedCommand == tcpCommandToClient::setClientPlayerIndex) {
		const int index = bytes4ToInt(dataArray);
		this->setClientPlayerIndex(index);
	}
	else if (receivedCommand == tcpCommandToClient::sendScoreChartData) {	//这个想抽出来，但是涉及到字节问题。。。犹豫
		const int row = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int col = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		if (col != 5) {
			//错误处理
			QMessageBox::about(this, QStringLiteral("获取失败"), QStringLiteral("获取失败"));
			return;
		}
		QStandardItemModel* scoreChart = new QStandardItemModel();
		scoreChart->setHorizontalHeaderLabels({ QStringLiteral("玩家名"), QStringLiteral("总带入"), QStringLiteral("当前总筹码"), QStringLiteral("场上筹码"), QStringLiteral("净胜") });
		scoreChart->setRowCount(row);
		scoreChart->setColumnCount(col);
		//先读名字
		for (int i = 0; i < row; i++) {
			const int nameDataLen = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
			dataArray.remove(0, commandAndDataToClient::byteOfInt);
			QString name = QString::fromLocal8Bit(dataArray.left(nameDataLen).toStdString().data());
			dataArray.remove(0, nameDataLen);
			scoreChart->setItem(i, 0, new QStandardItem(name));
		}
		//读数据
		for (int j = 1; j < col; j++) {
			for (int i = 0; i < row; i++) {
				const int num = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
				dataArray.remove(0, commandAndDataToClient::byteOfInt);
				scoreChart->setData(scoreChart->index(i, j), num);					//用setData设置，server还是setItem，没改，暂时还用不到
				//scoreChart->setItem(i, j, new QStandardItem(QString::number(num)));
			}
		}
		//显示

		QTableView *tableView = new QTableView;
		tableView->resize(550, 500);
		tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tableView->setWindowTitle(QStringLiteral("计分板"));
		tableView->setModel(scoreChart);
		tableView->show();
		tableView->setSortingEnabled(true);											//排序方法有问题，上面用setData设置就好了
		tableView->sortByColumn(4, Qt::SortOrder::DescendingOrder);
	}
	else if (receivedCommand == tcpCommandToClient::showDealer) {
		const int dealerIndex = bytes4ToInt(dataArray);
		this->showPlayerDealer(dealerIndex);
	}
	else if (receivedCommand == tcpCommandToClient::showClientChatMessage) {
		QString chatMessage = QString::fromLocal8Bit(dataArray.toStdString().data());
		chatMessage = elidedLineText(chatMessageOutput, chatMessage);
		this->chatMessageOutput->addItem(chatMessage);
		this->chatMessageOutput->setCurrentRow(chatMessageOutput->count() - 1);
	}
	else if (receivedCommand == tcpCommandToClient::setGameMode) {
		const string modeText = dataArray.toStdString();
		this->setWindowTitle(QStringLiteral("德州扑克——") + QString::fromLocal8Bit(modeText.data()));
	}
	else if (receivedCommand == tcpCommandToClient::sameMacAddress) {
		QMessageBox::about(this, QStringLiteral("连接失败"), QStringLiteral("不允许相同的mac地址！"));
		QApplication::exit();
	}
	else if (receivedCommand == tcpCommandToClient::startTimer) {
		QByteArray playerIndexArray = dataArray.left(commandAndDataToClient::byteOfInt);
		const int playerIndex = bytes4ToInt(playerIndexArray);
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int sec = bytes4ToInt(dataArray);

		this->startTimer(playerIndex, sec);
	}
	else if (receivedCommand == tcpCommandToClient::stopTimer) {
		this->stopTimer();
	}
}

void TexasPokerClientUI::sendCommandAndDataToServer(commandAndDataToServer toSend) const
{
	this->m_tcpClient->write(toSend.getTcpSend());
}

void TexasPokerClientUI::showCommonCards(vector<card> const & needShowCommonCards) const {
	for (int i_card = 0; i_card < (int)needShowCommonCards.size(); ++i_card) {
		if (needShowCommonCards[i_card].isCardVailid()) {
			QString cardFileName = card::getCardFileName(needShowCommonCards[i_card]);
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
#ifdef PRINT_RECORD
	std::string msg;
	for (int i_card = 0; i_card < (int)needShowCommonCards.size(); ++i_card) {
		if (needShowCommonCards[i_card].isCardVailid()) {
			msg += card::cardToString(needShowCommonCards[i_card]) + " ";
		}
	}
	if ((int)msg.size() > 0) {
		m_recorder->info("公共牌 = " + msg);
	}
#endif // PRINT_RECORD

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
#ifdef PRINT_RECORD
	if (nowRound == gameRound::Flop) {
		m_recorder->info("gameRound = 翻牌");
	}
	else if (nowRound == gameRound::Turn) {
		m_recorder->info("gameRound = 转牌");
	}
	else if (nowRound == gameRound::River) {
		m_recorder->info("gameRound = 河牌");
	}
	else {}
#endif // PRINT_RECORD
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
#ifdef PRINT_RECORD
	m_recorder->info("底池 = " + std::to_string(potNum));
#endif // PRINT_RECORD

}

void TexasPokerClientUI::hidePot() const {
	this->pot->hide();
}

//player show方法
//客户端下，player数组中，自己的编号是0，但是在服务器中的编号不一定是0
//考虑了当前client编号，假设当前x，需要显示y，则实际在本client显示的位置应该是(y-x+8)%8
void TexasPokerClientUI::showPlayerHandCards(const int playerIndex, vector<card> const& handCards) const {
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showHandCards(handCards);
#ifdef PRINT_RECORD
	std::string msg;
	for (int i_card = 0; i_card < (int)handCards.size(); ++i_card) {
		if (handCards[i_card].isCardVailid()) {
			msg += card::cardToString(handCards[i_card]) + " ";
		}
	}
	if ((int)msg.size() > 0) {
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，手牌 = " + msg);
		m_recorder->flush();
	}
#endif // PRINT_RECORD

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
	QString shownMessage = QString::fromLocal8Bit(actionMessage.data());
	this->players[playerIndexInClient]->showPlayerActionMessage(shownMessage);
#ifdef PRINT_RECORD
	//QString sss = QStringLiteral("看牌");	//for test
	//int a = sss.length();
	//sss += QStringLiteral("：200");
	//int b = QStringLiteral("看牌：").length();
	//sss = sss.remove(0, QStringLiteral("看牌：").length());
	if (shownMessage.left(QStringLiteral("allin").length()) == QStringLiteral("allin")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("allin:").length());
		const int allinMoney = shownMessage.toInt();
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，allin = " + std::to_string(allinMoney));
	}
	else if (shownMessage.left(QStringLiteral("跟注").length()) == QStringLiteral("跟注")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("跟注：").length());
		const int callMoney = shownMessage.toInt();
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，跟注 = " + std::to_string(callMoney));
	}
	else if (shownMessage.left(QStringLiteral("看牌").length()) == QStringLiteral("看牌")) {
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，看牌");
	}
	else if (shownMessage.left(QStringLiteral("加注至").length()) == QStringLiteral("加注至")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("加注至：").length());
		const int raiseMoney = shownMessage.toInt();
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，加注至 = " + std::to_string(raiseMoney));
	}
	else if (shownMessage.left(QStringLiteral("弃牌").length()) == QStringLiteral("弃牌")) {
		m_recorder->info("玩家编号=" + std::to_string(playerIndex) + "，弃牌" );
	}
	else if (shownMessage.left(QStringLiteral("牌型：").length()) == QStringLiteral("牌型：")) {
		std::string msg = "玩家编号 = " + std::to_string(playerIndex) + "，牌型 = ";
		
		shownMessage = shownMessage.remove(0, QStringLiteral("牌型：").length());
		QString cardTypeMsg;
		for (int i = 0; shownMessage[i] != '\n'; i++) {
			cardTypeMsg += shownMessage[i];
		}
		shownMessage = shownMessage.remove(0, cardTypeMsg.length() + 1);
		msg += string(cardTypeMsg.toLocal8Bit()) + "，";
		if (shownMessage.left(QStringLiteral("玩家赢得").length()) == QStringLiteral("玩家赢得")) {
			shownMessage = shownMessage.remove(0, QStringLiteral("玩家赢得：").length());
			msg += "玩家赢得 = " + string(shownMessage.toLocal8Bit());
		}
		else if (shownMessage.left(QStringLiteral("玩家输了").length()) == QStringLiteral("玩家输了")) {
			msg += "玩家输了";
		}
		else {}
		m_recorder->info(msg);
	}
	else if (shownMessage.left(QStringLiteral("玩家赢得").length()) == QStringLiteral("玩家赢得")) {
		//无牌型时获胜，先检查是不是没人了
		{
			if (playerIndex == this->m_clientPlayerIndex) {				//赢者非本机玩家，一定不是因为没人获胜，因为当前client在游戏中。 如果本机玩家赢了，则可能是因为其余人弃牌。
				bool isNoOne = true;

				for (int i_player = 1; i_player < game::maxNumOfPlayers; i_player++) {		//除client以外，是否有人在场。 需要从1开始，因为0是client本人。
					auto p = this->players[i_player];
					if (p->playerName->isVisible() && p->playerChip->isVisible()) {
						isNoOne = false;
						break;
					}
				}
				if (isNoOne) {
					this->m_recorder->info("玩家逃跑，游戏中只剩 1 人，剩余玩家编号 = " + std::to_string(this->m_clientPlayerIndex));
				}
			}
		}
		shownMessage = shownMessage.remove(0, QStringLiteral("玩家赢得：").length());
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + "，赢得 = " + string(shownMessage.toLocal8Bit()));
	}
	else if (shownMessage.left(QStringLiteral("玩家输了").length()) == QStringLiteral("玩家输了")) {
		m_recorder->info("玩家编号 = " + std::to_string(playerIndex) + " 输了");
	}
	else {}
	m_recorder->flush();
#endif // PRINT_RECORD

}

void TexasPokerClientUI::showPlayerSidePot(const int playerIndex, const int money) const
{
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->players[playerIndexInClient]->showSidePot(money);
#ifdef PRINT_RECORD
	m_recorder->info("玩家编号=" + std::to_string(playerIndex) + "边池=" + std::to_string(money));
#endif 

}

void TexasPokerClientUI::showPlayerDealer(const int playerIndex) const
{
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->dealer->setGeometry(QRect(80 + playerPosition[playerIndexInClient][0], 180 + playerPosition[playerIndexInClient][1], 70, 23));
	this->dealer->show();

#ifdef PRINT_RECORD
	m_recorder->info("-------------------游戏开始！-------------------");
	m_recorder->info("参与玩家为：");
	for (int i = 0; i < game::maxNumOfPlayers; i++) {
		auto const& p = this->players[i];
		if (p == nullptr)
			continue;
		QString chipMsg = p->playerChip->text();
		chipMsg = chipMsg.remove(0, QStringLiteral("筹码：").length());
		const int chip = chipMsg.toInt();
		if (p->playerName->isVisible() && chip > 0) {
			m_recorder->info("玩家编号 = " + std::to_string((i + this->m_clientPlayerIndex) % game::maxNumOfPlayers) +
				"，姓名 = " + std::string(p->playerName->text().toLocal8Bit()) +
				"，带入 = " + std::to_string(chip));
		}
	}
	m_recorder->info("dealer编号 = " + std::to_string(playerIndex));
	//m_recorder->flush();
#endif // PRINT_RECORD

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

void TexasPokerClientUI::hideClientPlayerAllAction() const
{
	this->hideClientPlayerAllinAction();
	this->hideClientPlayerCallAction();
	this->hideClientPlayerCheckAction();
	this->hideClientPlayerFoldAction();
	this->hideClientPlayerRaiseAction();
}

//槽函数
void TexasPokerClientUI::connectTcp() {
	string playerName(m_name->text().toLocal8Bit());				//防止乱码
	if ((int)playerName.size() <= 0) {
		QMessageBox::about(this, QStringLiteral("错误"), QStringLiteral("名字不能为空！"));
		return;
	}

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
	commandAndDataToServer toSend(tcpCommandToServer::setPlayerNameCommand, playerName);
	this->sendCommandAndDataToServer(toSend);	//设置自己的名字

	//发送本机mac地址
	this->sendCommandAndDataToServer(commandAndDataToServer(tcpCommandToServer::setClientMacAddressCommand, this->macAddress));

	//关闭ip地址框和connect
	this->m_address->hide();
	this->m_port->hide();
	this->m_connect->hide();
	this->m_name->hide();

	//新增聊天框
	chatMessageOutput = new QListWidget(centralWidget);
	chatMessageInput = new QComboBox(centralWidget);
	chatSendMessage = new QPushButton(centralWidget);

	chatMessageOutput->setGeometry(QRect(1250, 0, 330, 500));
	chatMessageInput->setGeometry(QRect(1250, 520, 270, 30));
	chatSendMessage->setGeometry(QRect(1250 + 285, 520, 40, 30));

	chatMessageOutput->show();
	chatMessageInput->show();
	chatSendMessage->show();

	//chatMessageOutput->setWordWrap(true);
	chatMessageOutput->setTextElideMode(Qt::ElideNone);
	//chatMessageOutput->setViewMode(QListView::IconMode);

	chatMessageInput->setEditable(true);
	chatMessageInput->addItem(QStringLiteral("不要走，决战到天亮。"));
	chatMessageInput->addItem(QStringLiteral("和你合作真是太愉快啦！"));
	chatMessageInput->addItem(QStringLiteral("再见了，我会想念大家的。"));
	chatMessageInput->addItem(QStringLiteral("下次再玩吧，我要走了。"));
	chatMessageInput->addItem(QStringLiteral("你是MM，还是GG？"));
	chatMessageInput->addItem(QStringLiteral("不要吵了，有什么好吵的？"));
	chatMessageInput->addItem(QStringLiteral("又断线了，网络怎么这么差？"));
	chatMessageInput->addItem(QStringLiteral("各位，真不好意思，我要离开一会。"));
	chatMessageInput->addItem(QStringLiteral("我们交个朋友吧，能告诉我你的联系方式吗？"));
	chatMessageInput->addItem(QStringLiteral("你的牌打得太好了！"));
	chatMessageInput->addItem(QStringLiteral("快点吧，我等得花都谢了！"));
	chatMessageInput->addItem(QStringLiteral("大家好，很高兴见到各位！"));

	chatMessageInput->setCurrentText("");
	chatMessageInput->setInsertPolicy(QComboBox::NoInsert);
	chatMessageInput->lineEdit()->setMaxLength(100);

	chatSendMessage->setText(QStringLiteral("发送"));

	connect(chatSendMessage, SIGNAL(clicked()), this, SLOT(chatSendMessageSlot()));
	connect(chatMessageInput, SIGNAL(activated(QString)), this, SLOT(chatSendMessageSlot()));
	connect(chatMessageInput->lineEdit(), SIGNAL(returnPressed()), this, SLOT(chatSendMessageSlot()));		//绑定自身QLineEdit的回车信号到发送函数上
}
void TexasPokerClientUI::nowPlayerRaise()
{
	this->hideClientPlayerAllAction();
	const int raiseMoney = this->raiseMoney->text().toInt();
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerRaiseCommand, raiseMoney);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerAllin()
{
	this->hideClientPlayerAllAction();
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerAllinCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerCheck()
{
	this->hideClientPlayerAllAction();
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerCheckCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerCall()
{
	this->hideClientPlayerAllAction();
	commandAndDataToServer toSend(tcpCommandToServer::nowPlayerCallCommand);
	this->sendCommandAndDataToServer(toSend);
}

void TexasPokerClientUI::nowPlayerFold()
{
	this->hideClientPlayerAllAction();
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
	QByteArray newData = m_tcpClient->readAll();	//用readyRead触发已经读空
	vector<QByteArray> tcpPackages = this->m_tcpPackageAnalyzer.getTcpPackages(newData);
	for (auto const& commandAndDataArray : tcpPackages) {
		this->analyzeCommand(commandAndDataArray);
	}
	//this->m_buffer = this->m_buffer.append(buffer);	
	//
	//while (1) {		//可能一次读了多个包，加了个循环，不知道对不对
	//	if (this->m_headLen <= 0) {	//包体长度无意义，需要读取包体长度
	//		if (this->m_buffer.length() < 4)		//前4字节是包体长度,不够就等下次
	//			return;
	//		QByteArray head = this->m_buffer.left(4);
	//		this->m_buffer.remove(0, 4);
	//		this->m_headLen = bytes4ToInt(head);
	//	}
	//	const int len = this->m_buffer.length();
	//	if (len < this->m_headLen) {	//数据长度不够，等下次
	//		return;
	//	}
	//	QByteArray commandAndDataArray = this->m_buffer.left(this->m_headLen);
	//	this->m_buffer.remove(0, this->m_headLen);
	//	this->m_headLen = 0;						//读完数据，重置包体长度

	//	//根据命令进行操作了
	//	this->analyzeCommand(commandAndDataArray);
	//}
}

void TexasPokerClientUI::disconnectTcp() {
	QMessageBox::about(this, QStringLiteral("连接失败"), QStringLiteral("服务器已断开连接"));
	//this->close();
	QApplication::exit();
}


void TexasPokerClientUI::aboutSlot() {
	QMessageBox::about(this
		, QStringLiteral("德州扑克")
		, QStringLiteral("<p>Version 1.0<p> <b>Copyright &copy; 2021</b> wyp zju")
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

	QLabel *imageShow = new QLabel(centralWidget);
	QString path = "image/tubiao/cardType.jpg";
	imageShow->setPixmap(path);
	imageShow->raise();
	imageShow->show();
	
	newWindow->show();

}

void TexasPokerClientUI::showScoreChartSlot() {
	commandAndDataToServer toSend(tcpCommandToServer::showScoreChartCommand);
	this->sendCommandAndDataToServer(toSend);
}


void TexasPokerClientUI::chatSendMessageSlot() {
	string clientChatMessage(this->chatMessageInput->currentText().toLocal8Bit());
	this->chatMessageInput->setCurrentText("");
	if (clientChatMessage.size() == 0) {
		return;
	}
	string clientPlayerName(this->players[0]->playerName->text().toLocal8Bit());
	string totalMessage = clientPlayerName + "：" + clientChatMessage;

	QString totalMessageQString = QString::fromLocal8Bit(totalMessage.data());
	//insertReturn(totalMessageQString, chatMessageOutput->width());
	totalMessageQString = elidedLineText(chatMessageOutput, totalMessageQString);
	this->chatMessageOutput->addItem(totalMessageQString);
	this->chatMessageOutput->setCurrentRow(chatMessageOutput->count() - 1);

	commandAndDataToServer toSend(tcpCommandToServer::showChatMessage, totalMessage);
	this->sendCommandAndDataToServer(toSend);
	return;
}

void TexasPokerClientUI::timerEventSlot() {
	//每秒一次
	m_resTime--;
	if (this->m_nowActionPlayer == this->m_clientPlayerIndex) {
		m_clientResTimeShow->setText(QStringLiteral("剩余思考时间：") + QString::number(m_resTime) + QStringLiteral("秒"));
		m_clientResTimeShow->show();
		if (m_resTime <= 0) {								//到时间了
			const bool canCheck = this->check->isVisible();	//先看能不能check，要不然被下面hideClientPlayerAllAction隐藏了
			this->hideClientPlayerAllAction();				//先隐藏，以免重复按------------感觉如果卡在中间按也会出问题，要不要加锁？
			if (canCheck) {									//如果能check就check，不能就fold
				this->nowPlayerCheck();
			}
			else {
				this->nowPlayerFold();
			}
		}
	}
	else {
		this->showPlayerActionMessage(this->m_nowActionPlayer, "玩家行动中... 剩余" + std::to_string(m_resTime) + std::string("秒"));
	}
}