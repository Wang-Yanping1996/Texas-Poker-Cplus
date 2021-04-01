#include "TexasPokerClientUI.h"

vector<card> QByteArrayToCards(QByteArray const & qArray)
{
	QByteArray dataArray = qArray;
	vector<card> cards(0);
	while (dataArray.length() >= 2 * commandAndDataToClient::byteOfInt) {	//1��������ɫ+���֣�2���ֽ�
		cardColor color = (cardColor)bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		cardNumber number = (cardNumber)bytes4ToInt(dataArray.mid(commandAndDataToClient::byteOfInt, commandAndDataToClient::byteOfInt));
		dataArray.remove(0, 2 * commandAndDataToClient::byteOfInt);
		cards.push_back(card(color, number));
	}
	return cards;
}
//CSDN�ҵĺ���
QString elidedLineText(QWidget *pWidget, QString strText)
{
	QFontMetrics fontMetrics(pWidget->font());
	QStringList strListLine;
	const int margin = fontMetrics.width("��");
	//���еĶ��ӽ��б�
	for (int i = 0; i < strText.size(); i++) {
		if (fontMetrics.width(strText.left(i)) >= pWidget->width() - margin) { 
			strListLine.append(strText.left(i));
			strText = strText.right(strText.size() - i);
			i = 0;
		}
	}
	//�����һ�е�Ҳ�ӽ�ȥ
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

	playerName->setText(QStringLiteral("���1"));
	//playerName->setAlignment(Qt::AlignCenter);
	playerChip->setText(QStringLiteral("���룺"));
	actionMessage->setText(QStringLiteral("�����Ϊ��"));
	sidePot->setText(QStringLiteral("�߳أ�"));
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
	this->playerChip->setText(QStringLiteral("���룺") + QString::number(chip));
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
		QString message = QStringLiteral("�߳أ�");
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

	setWindowTitle(QStringLiteral("�����˿�"));
	if (this->objectName().isEmpty())
		this->setObjectName(QStringLiteral("TexasPoker"));
	this->resize(1600, 720);					//���ߣ�ԭ1200*720��Ϊ�˼��������չ��1600*720
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

	QMenu* helpMenu = menuBar->addMenu(QStringLiteral("����(&H)"));

	QAction* aboutAct = new QAction(QIcon(QStringLiteral("image/tubiao/about.png")), QStringLiteral("����..."), this);
	aboutAct->setStatusTip(QStringLiteral("����"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutSlot()));
	helpMenu->addAction(aboutAct);

	QAction* aboutCardType = new QAction(QIcon(QStringLiteral("image/tubiao/about.png")), QStringLiteral("����"), this);
	aboutCardType->setStatusTip(QStringLiteral("����"));
	connect(aboutCardType, SIGNAL(triggered()), this, SLOT(aboutCardTypeSlot()));
	helpMenu->addAction(aboutCardType);

	QAction *showScoreChart = new QAction(menuBar);
	showScoreChart->setText(QStringLiteral("�Ʒְ�"));
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
	pot->setObjectName(QStringLiteral("�׳�"));
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

	raise->setText(QStringLiteral("��ע"));
	allin->setText(QStringLiteral("allin"));
	check->setText(QStringLiteral("����"));
	fold->setText(QStringLiteral("����"));
	call->setText(QStringLiteral("��ע"));
	flop->setText(QApplication::translate("QtWidgetsApplication1Class", "\347\277\273\347\211\214", Q_NULLPTR));
	turn->setText(QApplication::translate("QtWidgetsApplication1Class", "\350\275\254\347\211\214", Q_NULLPTR));
	river->setText(QApplication::translate("QtWidgetsApplication1Class", "\346\262\263\347\211\214", Q_NULLPTR));
	pot->setText(QStringLiteral("�׳أ�") + QString::number(10));
	begin->setText(QStringLiteral("��ʼ"));
	for (int i_player = 0; i_player < game::maxNumOfPlayers; ++i_player) {
		players[i_player] = new playerClient(centralWidget, playerPosition[i_player][0], playerPosition[i_player][1], i_player);		
	}

	m_connect = new QPushButton(centralWidget);
	m_connect->setObjectName(QStringLiteral("connect"));
	m_connect->setGeometry(QRect(580, 370, 50, 30));
	m_connect->raise();
	m_connect->setText(QStringLiteral("����"));
	m_connect->show();

	m_address = new QLineEdit(centralWidget);
	m_address->setObjectName(QStringLiteral("address"));
	m_address->setGeometry(QRect(500, 300, 150, 30));
	m_address->raise();
	m_address->setText(QStringLiteral("10.12.222.73"));
	m_address->setPlaceholderText(QStringLiteral("����������IPv4��ַ"));
	m_address->show();

	m_port = new QLineEdit(centralWidget);
	m_port->setObjectName(QStringLiteral("port"));
	m_port->setGeometry(QRect(680, 300, 60, 30));
	m_port->raise();
	m_port->setText(QStringLiteral("6060"));
	m_port->setPlaceholderText(QStringLiteral("�˿ں�"));
	m_port->show();

	m_name = new QLineEdit(centralWidget);
	m_name->setObjectName(QStringLiteral("playerName"));
	m_name->setGeometry(QRect(550, 230, 100, 30));
	m_name->raise();
	//m_name->setText(QStringLiteral("������"));
	m_name->setMaxLength(8);
	m_name->setPlaceholderText(QStringLiteral("����������"));
	m_name->show();

	m_tcpClient = new QTcpSocket(this);
	m_tcpClient->abort();
	m_tcpPackageAnalyzer.clear();
	//m_headLen = 0;
	//m_buffer.clear();

	//�򿪿ͻ���ʱ��ȡ������һ��mac��ַ
	if (!this->getMacByGetAdaptersInfo()) {
		QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("��ȡ������ַʧ��"));
		QApplication::exit();
	}
	//��ʱ������
	m_clock = new QTimer(this);
	m_clock->stop();			//Ĭ����ֹͣ
	m_resTime = INT_MAX;
	m_nowActionPlayer = -1;		//Ĭ�ϵ�ǰ�����Ч
	m_clientResTimeShow = new QLabel(this);
	m_clientResTimeShow->setGeometry(QRect(470, 440, 150, 23));
	m_clientResTimeShow->hide();
	connect(this->m_clock, SIGNAL(timeout()), this, SLOT(timerEventSlot()));

	connect(m_connect, SIGNAL(clicked()), this, SLOT(connectTcp()));
	connect(m_tcpClient, SIGNAL(disconnected()), this, SLOT(disconnectTcp()));	//��tcp���ӳɹ����Ƶ�����

	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(begin, SIGNAL(clicked()), this, SLOT(clientReady()));

	connect(raise, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
	connect(allin, SIGNAL(clicked()), this, SLOT(nowPlayerAllin()));
	connect(check, SIGNAL(clicked()), this, SLOT(nowPlayerCheck()));
	connect(call, SIGNAL(clicked()), this, SLOT(nowPlayerCall()));
	connect(fold, SIGNAL(clicked()), this, SLOT(nowPlayerFold()));

#ifdef PRINT_RECORD
	try {
		m_recorder = spdlog::rotating_logger_mt("GameRecord", "logs\\����.txt", 1024 * 1024 * 5, 1);
	}
	catch (const spdlog::spdlog_ex &ex) {
		QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("������־ʧ��"));
		QApplication::exit();
	}

	if (m_recorder != nullptr) {
		m_recorder->set_level(spdlog::level::info);
		m_recorder->info("��־�����ɹ���");
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
			// ȷ������̫��
			if (pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// ȷ��MAC��ַ�ĳ���Ϊ 00-00-00-00-00-00
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
	if (this->m_resTime != infiniteTime) {		//��Ϊ-1������Ч�趨����������ʱ������
		this->timerEventSlot();					//����ʾһ�£��ƺ����趨��һ�룿
		this->m_clock->start(1000);
	}
	else {										//����Ͳ���ʱ����ʾ�����������NowPlayer�Ļ���Message����showFoldʱ���أ����Բ��ض����жϡ�
		this->showPlayerActionMessage(playerIndex, "����ж���...");
	}
}

//timer��ʱ���
void TexasPokerClientUI::stopTimer()
{
	this->m_clock->stop();
	this->m_resTime = INT_MAX;
	this->m_clientResTimeShow->hide();
}

//��֧Ӧ��д��==�ж�
//���ۡ�����
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
		//while (dataArray.length() >= 2*commandAndDataToClient::byteOfInt) {	//1��������ɫ+���֣�2���ֽ�
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
	else if (receivedCommand == tcpCommandToClient::sendScoreChartData) {	//����������������漰���ֽ����⡣������ԥ
		const int row = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		const int col = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
		dataArray.remove(0, commandAndDataToClient::byteOfInt);
		if (col != 5) {
			//������
			QMessageBox::about(this, QStringLiteral("��ȡʧ��"), QStringLiteral("��ȡʧ��"));
			return;
		}
		QStandardItemModel* scoreChart = new QStandardItemModel();
		scoreChart->setHorizontalHeaderLabels({ QStringLiteral("�����"), QStringLiteral("�ܴ���"), QStringLiteral("��ǰ�ܳ���"), QStringLiteral("���ϳ���"), QStringLiteral("��ʤ") });
		scoreChart->setRowCount(row);
		scoreChart->setColumnCount(col);
		//�ȶ�����
		for (int i = 0; i < row; i++) {
			const int nameDataLen = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
			dataArray.remove(0, commandAndDataToClient::byteOfInt);
			QString name = QString::fromLocal8Bit(dataArray.left(nameDataLen).toStdString().data());
			dataArray.remove(0, nameDataLen);
			scoreChart->setItem(i, 0, new QStandardItem(name));
		}
		//������
		for (int j = 1; j < col; j++) {
			for (int i = 0; i < row; i++) {
				const int num = bytes4ToInt(dataArray.left(commandAndDataToClient::byteOfInt));
				dataArray.remove(0, commandAndDataToClient::byteOfInt);
				scoreChart->setData(scoreChart->index(i, j), num);					//��setData���ã�server����setItem��û�ģ���ʱ���ò���
				//scoreChart->setItem(i, j, new QStandardItem(QString::number(num)));
			}
		}
		//��ʾ

		QTableView *tableView = new QTableView;
		tableView->resize(550, 500);
		tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tableView->setWindowTitle(QStringLiteral("�Ʒְ�"));
		tableView->setModel(scoreChart);
		tableView->show();
		tableView->setSortingEnabled(true);											//���򷽷������⣬������setData���þͺ���
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
		this->setWindowTitle(QStringLiteral("�����˿ˡ���") + QString::fromLocal8Bit(modeText.data()));
	}
	else if (receivedCommand == tcpCommandToClient::sameMacAddress) {
		QMessageBox::about(this, QStringLiteral("����ʧ��"), QStringLiteral("��������ͬ��mac��ַ��"));
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
		m_recorder->info("������ = " + msg);
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
		m_recorder->info("gameRound = ����");
	}
	else if (nowRound == gameRound::Turn) {
		m_recorder->info("gameRound = ת��");
	}
	else if (nowRound == gameRound::River) {
		m_recorder->info("gameRound = ����");
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
	QString showText = QStringLiteral("�׳أ�") + QString::number(potNum);
	this->pot->setText(showText);
	this->pot->show();
#ifdef PRINT_RECORD
	m_recorder->info("�׳� = " + std::to_string(potNum));
#endif // PRINT_RECORD

}

void TexasPokerClientUI::hidePot() const {
	this->pot->hide();
}

//player show����
//�ͻ����£�player�����У��Լ��ı����0�������ڷ������еı�Ų�һ����0
//�����˵�ǰclient��ţ����赱ǰx����Ҫ��ʾy����ʵ���ڱ�client��ʾ��λ��Ӧ����(y-x+8)%8
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
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "������ = " + msg);
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
	//QString sss = QStringLiteral("����");	//for test
	//int a = sss.length();
	//sss += QStringLiteral("��200");
	//int b = QStringLiteral("���ƣ�").length();
	//sss = sss.remove(0, QStringLiteral("���ƣ�").length());
	if (shownMessage.left(QStringLiteral("allin").length()) == QStringLiteral("allin")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("allin:").length());
		const int allinMoney = shownMessage.toInt();
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "��allin = " + std::to_string(allinMoney));
	}
	else if (shownMessage.left(QStringLiteral("��ע").length()) == QStringLiteral("��ע")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("��ע��").length());
		const int callMoney = shownMessage.toInt();
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "����ע = " + std::to_string(callMoney));
	}
	else if (shownMessage.left(QStringLiteral("����").length()) == QStringLiteral("����")) {
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "������");
	}
	else if (shownMessage.left(QStringLiteral("��ע��").length()) == QStringLiteral("��ע��")) {
		shownMessage = shownMessage.remove(0, QStringLiteral("��ע����").length());
		const int raiseMoney = shownMessage.toInt();
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "����ע�� = " + std::to_string(raiseMoney));
	}
	else if (shownMessage.left(QStringLiteral("����").length()) == QStringLiteral("����")) {
		m_recorder->info("��ұ��=" + std::to_string(playerIndex) + "������" );
	}
	else if (shownMessage.left(QStringLiteral("���ͣ�").length()) == QStringLiteral("���ͣ�")) {
		std::string msg = "��ұ�� = " + std::to_string(playerIndex) + "������ = ";
		
		shownMessage = shownMessage.remove(0, QStringLiteral("���ͣ�").length());
		QString cardTypeMsg;
		for (int i = 0; shownMessage[i] != '\n'; i++) {
			cardTypeMsg += shownMessage[i];
		}
		shownMessage = shownMessage.remove(0, cardTypeMsg.length() + 1);
		msg += string(cardTypeMsg.toLocal8Bit()) + "��";
		if (shownMessage.left(QStringLiteral("���Ӯ��").length()) == QStringLiteral("���Ӯ��")) {
			shownMessage = shownMessage.remove(0, QStringLiteral("���Ӯ�ã�").length());
			msg += "���Ӯ�� = " + string(shownMessage.toLocal8Bit());
		}
		else if (shownMessage.left(QStringLiteral("�������").length()) == QStringLiteral("�������")) {
			msg += "�������";
		}
		else {}
		m_recorder->info(msg);
	}
	else if (shownMessage.left(QStringLiteral("���Ӯ��").length()) == QStringLiteral("���Ӯ��")) {
		//������ʱ��ʤ���ȼ���ǲ���û����
		{
			if (playerIndex == this->m_clientPlayerIndex) {				//Ӯ�߷Ǳ�����ң�һ��������Ϊû�˻�ʤ����Ϊ��ǰclient����Ϸ�С� ����������Ӯ�ˣ����������Ϊ���������ơ�
				bool isNoOne = true;

				for (int i_player = 1; i_player < game::maxNumOfPlayers; i_player++) {		//��client���⣬�Ƿ������ڳ��� ��Ҫ��1��ʼ����Ϊ0��client���ˡ�
					auto p = this->players[i_player];
					if (p->playerName->isVisible() && p->playerChip->isVisible()) {
						isNoOne = false;
						break;
					}
				}
				if (isNoOne) {
					this->m_recorder->info("������ܣ���Ϸ��ֻʣ 1 �ˣ�ʣ����ұ�� = " + std::to_string(this->m_clientPlayerIndex));
				}
			}
		}
		shownMessage = shownMessage.remove(0, QStringLiteral("���Ӯ�ã�").length());
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + "��Ӯ�� = " + string(shownMessage.toLocal8Bit()));
	}
	else if (shownMessage.left(QStringLiteral("�������").length()) == QStringLiteral("�������")) {
		m_recorder->info("��ұ�� = " + std::to_string(playerIndex) + " ����");
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
	m_recorder->info("��ұ��=" + std::to_string(playerIndex) + "�߳�=" + std::to_string(money));
#endif 

}

void TexasPokerClientUI::showPlayerDealer(const int playerIndex) const
{
	const int playerIndexInClient = (playerIndex - this->m_clientPlayerIndex + game::maxNumOfPlayers) % game::maxNumOfPlayers;
	this->dealer->setGeometry(QRect(80 + playerPosition[playerIndexInClient][0], 180 + playerPosition[playerIndexInClient][1], 70, 23));
	this->dealer->show();

#ifdef PRINT_RECORD
	m_recorder->info("-------------------��Ϸ��ʼ��-------------------");
	m_recorder->info("�������Ϊ��");
	for (int i = 0; i < game::maxNumOfPlayers; i++) {
		auto const& p = this->players[i];
		if (p == nullptr)
			continue;
		QString chipMsg = p->playerChip->text();
		chipMsg = chipMsg.remove(0, QStringLiteral("���룺").length());
		const int chip = chipMsg.toInt();
		if (p->playerName->isVisible() && chip > 0) {
			m_recorder->info("��ұ�� = " + std::to_string((i + this->m_clientPlayerIndex) % game::maxNumOfPlayers) +
				"������ = " + std::string(p->playerName->text().toLocal8Bit()) +
				"������ = " + std::to_string(chip));
		}
	}
	m_recorder->info("dealer��� = " + std::to_string(playerIndex));
	//m_recorder->flush();
#endif // PRINT_RECORD

}

//player hide����
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
	this->raise->show();		//��ʾ����
	this->raiseMoney->setRange(minRaiseMoney, maxRaiseMoney);	//���÷�Χ
	this->raiseMoney->setValue(minRaiseMoney);					//��ֵ��Ϊ��Сֵ
	this->raiseMoney->setSingleStep(game::bigBind);				//��������һ�θı����
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
	QString showText = QStringLiteral("��ע:") + QString::number(callMoney);
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

//�ۺ���
void TexasPokerClientUI::connectTcp() {
	string playerName(m_name->text().toLocal8Bit());				//��ֹ����
	if ((int)playerName.size() <= 0) {
		QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("���ֲ���Ϊ�գ�"));
		return;
	}

	QString receivedAddress = m_address->text();
	QHostAddress honstAddress;
	if (!honstAddress.setAddress(receivedAddress)){
		QMessageBox::about(this, QStringLiteral("����ʧ��"), QStringLiteral("��Ч��IP��ַ��"));
		return;
	}
	//auto a = testAddress.toIPv4Address();
	QString receivedPort = m_port->text();
	quint32 hostPort = receivedPort.toInt();
	//quint16 hostPort = 6060;
	m_tcpClient->connectToHost(honstAddress, hostPort);
	const bool connected = m_tcpClient->waitForConnected(1000);
	if (!connected) {
		QMessageBox::about(this, QStringLiteral("����ʧ��"), QStringLiteral("������δ��Ӧ"));
		return;
	}
	//���ӳɹ��������Լ����ֺͿ�ʼ������Ӧ���ɷ���������
	commandAndDataToServer toSend(tcpCommandToServer::setPlayerNameCommand, playerName);
	this->sendCommandAndDataToServer(toSend);	//�����Լ�������

	//���ͱ���mac��ַ
	this->sendCommandAndDataToServer(commandAndDataToServer(tcpCommandToServer::setClientMacAddressCommand, this->macAddress));

	//�ر�ip��ַ���connect
	this->m_address->hide();
	this->m_port->hide();
	this->m_connect->hide();
	this->m_name->hide();

	//���������
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
	chatMessageInput->addItem(QStringLiteral("��Ҫ�ߣ���ս��������"));
	chatMessageInput->addItem(QStringLiteral("�����������̫�������"));
	chatMessageInput->addItem(QStringLiteral("�ټ��ˣ��һ������ҵġ�"));
	chatMessageInput->addItem(QStringLiteral("�´�����ɣ���Ҫ���ˡ�"));
	chatMessageInput->addItem(QStringLiteral("����MM������GG��"));
	chatMessageInput->addItem(QStringLiteral("��Ҫ���ˣ���ʲô�ó��ģ�"));
	chatMessageInput->addItem(QStringLiteral("�ֶ����ˣ�������ô��ô�"));
	chatMessageInput->addItem(QStringLiteral("��λ���治����˼����Ҫ�뿪һ�ᡣ"));
	chatMessageInput->addItem(QStringLiteral("���ǽ������Ѱɣ��ܸ����������ϵ��ʽ��"));
	chatMessageInput->addItem(QStringLiteral("����ƴ��̫���ˣ�"));
	chatMessageInput->addItem(QStringLiteral("���ɣ��ҵȵû���л�ˣ�"));
	chatMessageInput->addItem(QStringLiteral("��Һã��ܸ��˼�����λ��"));

	chatMessageInput->setCurrentText("");
	chatMessageInput->setInsertPolicy(QComboBox::NoInsert);
	chatMessageInput->lineEdit()->setMaxLength(100);

	chatSendMessage->setText(QStringLiteral("����"));

	connect(chatSendMessage, SIGNAL(clicked()), this, SLOT(chatSendMessageSlot()));
	connect(chatMessageInput, SIGNAL(activated(QString)), this, SLOT(chatSendMessageSlot()));
	connect(chatMessageInput->lineEdit(), SIGNAL(returnPressed()), this, SLOT(chatSendMessageSlot()));		//������QLineEdit�Ļس��źŵ����ͺ�����
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
	this->hideBegin();		//�Լ�������
}

void TexasPokerClientUI::readData() {			//���ܵüӸ�whileѭ��
	QByteArray newData = m_tcpClient->readAll();	//��readyRead�����Ѿ�����
	vector<QByteArray> tcpPackages = this->m_tcpPackageAnalyzer.getTcpPackages(newData);
	for (auto const& commandAndDataArray : tcpPackages) {
		this->analyzeCommand(commandAndDataArray);
	}
	//this->m_buffer = this->m_buffer.append(buffer);	
	//
	//while (1) {		//����һ�ζ��˶���������˸�ѭ������֪���Բ���
	//	if (this->m_headLen <= 0) {	//���峤�������壬��Ҫ��ȡ���峤��
	//		if (this->m_buffer.length() < 4)		//ǰ4�ֽ��ǰ��峤��,�����͵��´�
	//			return;
	//		QByteArray head = this->m_buffer.left(4);
	//		this->m_buffer.remove(0, 4);
	//		this->m_headLen = bytes4ToInt(head);
	//	}
	//	const int len = this->m_buffer.length();
	//	if (len < this->m_headLen) {	//���ݳ��Ȳ��������´�
	//		return;
	//	}
	//	QByteArray commandAndDataArray = this->m_buffer.left(this->m_headLen);
	//	this->m_buffer.remove(0, this->m_headLen);
	//	this->m_headLen = 0;						//�������ݣ����ð��峤��

	//	//����������в�����
	//	this->analyzeCommand(commandAndDataArray);
	//}
}

void TexasPokerClientUI::disconnectTcp() {
	QMessageBox::about(this, QStringLiteral("����ʧ��"), QStringLiteral("�������ѶϿ�����"));
	//this->close();
	QApplication::exit();
}


void TexasPokerClientUI::aboutSlot() {
	QMessageBox::about(this
		, QStringLiteral("�����˿�")
		, QStringLiteral("<p>Version 1.0<p> <b>Copyright &copy; 2021</b> wyp zju")
	);
}

void TexasPokerClientUI::aboutCardTypeSlot() {
	QMainWindow *newWindow = new QMainWindow(this);

	newWindow->setWindowTitle(QStringLiteral("����"));
	if (newWindow->objectName().isEmpty())
		newWindow->setObjectName(QStringLiteral("����"));
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
	string totalMessage = clientPlayerName + "��" + clientChatMessage;

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
	//ÿ��һ��
	m_resTime--;
	if (this->m_nowActionPlayer == this->m_clientPlayerIndex) {
		m_clientResTimeShow->setText(QStringLiteral("ʣ��˼��ʱ�䣺") + QString::number(m_resTime) + QStringLiteral("��"));
		m_clientResTimeShow->show();
		if (m_resTime <= 0) {								//��ʱ����
			const bool canCheck = this->check->isVisible();	//�ȿ��ܲ���check��Ҫ��Ȼ������hideClientPlayerAllAction������
			this->hideClientPlayerAllAction();				//�����أ������ظ���------------�о���������м䰴Ҳ������⣬Ҫ��Ҫ������
			if (canCheck) {									//�����check��check�����ܾ�fold
				this->nowPlayerCheck();
			}
			else {
				this->nowPlayerFold();
			}
		}
	}
	else {
		this->showPlayerActionMessage(this->m_nowActionPlayer, "����ж���... ʣ��" + std::to_string(m_resTime) + std::string("��"));
	}
}