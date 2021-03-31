#include "TexasPokerServer.h"

emptyServerUI::emptyServerUI(QWidget *parent, game* g)
	:QMainWindow(parent), m_game(g)
{
	setWindowTitle(QStringLiteral("�����˿˷�����"));
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

	QIcon icon;
	icon.addFile(QStringLiteral("image/tubiao/poker_window_tubiao.ico"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);

	//�趨�˿ں�
	m_setPort = new QPushButton(centralWidget);
	m_setPort->setObjectName(QStringLiteral("setPort"));
	m_setPort->setGeometry(QRect(500, 370, 100, 30));
	m_setPort->raise();
	m_setPort->setText(QStringLiteral("���ö˿�"));
	m_setPort->show();

	m_port = new QLineEdit(centralWidget);
	m_port->setObjectName(QStringLiteral("port"));
	m_port->setGeometry(QRect(500, 300, 100, 30));
	m_port->raise();
	m_port->setText(QStringLiteral("6060"));
	m_port->show();

	m_portDisplay = new QLabel(centralWidget);
	m_portDisplay->setObjectName(QStringLiteral("portDisplay"));
	m_portDisplay->setGeometry(QRect(500, 230, 150, 31));
	m_portDisplay->setTextFormat(Qt::AutoText);
	m_portDisplay->setScaledContents(false);
	//m_portDisplay->setAlignment(Qt::AlignCenter);
	m_portDisplay->setText(QStringLiteral("�˿ں�Ϊ��"));
	m_portDisplay->raise();
	m_portDisplay->show();

	//��Ϸģʽ������or����
	m_gameMode = new QButtonGroup(centralWidget);
	m_gameMode->setExclusive(true);					//���ڰ�������

	nomalMode = new QRadioButton(centralWidget);
	nomalMode->setText(QStringLiteral("����ģʽ"));
	nomalMode->setGeometry(QRect(650, 300, 100, 30));
	nomalMode->show();
	m_gameMode->addButton(nomalMode);
	shortDeckMode = new QRadioButton(centralWidget);
	shortDeckMode->setText(QStringLiteral("����ģʽ"));
	shortDeckMode->setGeometry(QRect(780, 300, 100, 30));
	shortDeckMode->show();
	m_gameMode->addButton(shortDeckMode);
	
	nomalMode->setChecked(true);

	m_gameModeDisplay = new QLabel(this->centralWidget);		//ģʽ��ʾ
	m_gameModeDisplay->setGeometry(QRect(500, 270, 200, 30));
	m_gameModeDisplay->hide();
	
	//ÿ��˼��ʱ������
	m_timePerActionInput = new QLineEdit(this);
	m_timePerActionInput->setGeometry(QRect(270, 310, 180, 30));
	//m_timePerActionInput->setValidator(new QRegExpValidator(QRegExp("^$|[5-9]|[1-9][0-9]{1}"), this));	//���뷶Χֻ��5~90s
	m_timePerActionInput->show();

	m_timePerActionShow = new QLabel(this);
	m_timePerActionShow->setGeometry(QRect(270, 243, 210, 30));			//��ʾ˼��ʱ��
	m_timePerActionShow->setText(QStringLiteral("˼��ʱ��(5~99s��������=����ʱ)��"));
	m_timePerActionShow->show();

	m_timePerAction = infiniteTime;		//Ĭ��-1

	//tcp������س�ʼ��
	m_arrayClient = vector<QTcpSocket*>(game::maxNumOfPlayers, nullptr);

	m_tcpPackageAnalyzers = vector<tcpPackageAnalyzer>(game::maxNumOfPlayers);
	for (auto& analyzer : m_tcpPackageAnalyzers)analyzer.clear();
	//m_buffers = vector<QByteArray>(game::maxNumOfPlayers);
	//for (auto& buffer : m_buffers)buffer.clear();
	//m_headLens = vector<int>(game::maxNumOfPlayers, 0);
	m_SocketMap = map<QTcpSocket*, int>();
	m_SocketMap.clear();

	m_tcpServer = new QTcpServer(this);
	
	/*QString hostAddress = QNetworkInterface().allAddresses().at(1).toString();*/
	//quint16 port = 6060;
	//m_tcpServer->listen(QHostAddress::Any, port);

	connect(m_setPort, SIGNAL(clicked()), this, SLOT(setPort()));
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}

//�������д�ļ���else if������Ӧ�ó�����������������е��ҡ�	//�ѳ��
//��֧Ӧ����==д
void emptyServerUI::analyzeCommand(QByteArray received, const int fromPlayerIndex)
{
	QByteArray commandArray = received.left(commandAndDataToServer::byteOfInt);
	QByteArray dataArray = received;
	dataArray.remove(0, commandAndDataToServer::byteOfInt);

	tcpCommandToServer receivedCommand = (tcpCommandToServer)bytes4ToInt(commandArray);

	if (receivedCommand == tcpCommandToServer::noCommandToServer) {
		return;
	}
	else if (receivedCommand == tcpCommandToServer::nowPlayerRaiseCommand) {
		const int raiseTo = bytes4ToInt(dataArray);
		this->m_game->nowPlayerRaise(raiseTo);
	}
	else if (receivedCommand == tcpCommandToServer::nowPlayerAllinCommand) {
		this->m_game->nowPlayerAllin();
	}
	else if (receivedCommand == tcpCommandToServer::nowPlayerCheckCommand) {
		this->m_game->nowPlayerCheck();
	}
	else if (receivedCommand == tcpCommandToServer::nowPlayerCallCommand) {
		this->m_game->nowPlayerCall();
	}
	else if (receivedCommand == tcpCommandToServer::nowPlayerFoldCommand) {
		this->m_game->nowPlayerFold();
	}
	else if (receivedCommand == tcpCommandToServer::playerReadyCommand) {	//����from�����ready��
		this->showPlayerActionMessage(fromPlayerIndex, "׼����");
		this->m_game->addNumOfReadyPlayer();								//ready������һ����begin���������

		const int numOfPlayer = this->m_game->getNumOfPlayers();			//�ڴ��ж���Ϸ�Ƿ�ʼ�������߼�
		const int numOfReadyPlayer = this->m_game->getNumOfReadyPlayer();
		if (numOfReadyPlayer == numOfPlayer) {
			this->m_game->begin();											//����о��߼���Ӧ���ǣ�ֱ���ж������Ƿ���ȣ��Ծ����Ƿ����begin�����������߼���������
		}
	}
	else if (receivedCommand == tcpCommandToServer::setPlayerNameCommand) {
		string newPlayerName = dataArray.toStdString();				//������������
		this->setPlayerUniqueName(fromPlayerIndex, newPlayerName);
	}
	else if (receivedCommand == tcpCommandToServer::setClientMacAddressCommand) {
		const string nowClientMacAddress = dataArray.toStdString();
		this->setPlayerUniqueMacAddress(fromPlayerIndex, nowClientMacAddress);
	}
	else if (receivedCommand == tcpCommandToServer::showScoreChartCommand) {
		QStandardItemModel const* scoreChart = this->m_game->getScoreChart();
		commandAndDataToClient toSend(tcpCommandToClient::sendScoreChartData, scoreChart);
		this->sendCommandAndDataToPlayer(fromPlayerIndex, toSend);
	}
	else if (receivedCommand == tcpCommandToServer::showChatMessage) {
		const string message = dataArray.toStdString();
		commandAndDataToClient toSend(tcpCommandToClient::showClientChatMessage, message);
		this->sendCommandAndDataToAllExcept(fromPlayerIndex, toSend);
	}
}

void emptyServerUI::sendCommandAndDataToPlayer(const int playerIndex, commandAndDataToClient toSend)const
{
	QTcpSocket *toSendPlayerSocket = this->m_arrayClient[playerIndex];
	QByteArray toSendQByteArray = toSend.getTcpSend();
	if (toSendPlayerSocket == nullptr) {
		//QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("���͵����socket��Ч"));
		return;
	}
	toSendPlayerSocket->write(toSendQByteArray);
}

void emptyServerUI::sendCommandAndDataToAll(commandAndDataToClient toSend)const
{
	for (auto const& toSendPlayerSocket : this->m_arrayClient) {
		if (toSendPlayerSocket != nullptr) {
			toSendPlayerSocket->write(toSend.getTcpSend());	
		}
	}
}

void emptyServerUI::sendCommandAndDataToAllExcept(const int exceptPlayerIndex, commandAndDataToClient toSend)
{
	for (auto const& toSendPlayerSocket : this->m_arrayClient) {
		if (toSendPlayerSocket != nullptr && toSendPlayerSocket != this->m_arrayClient[exceptPlayerIndex]) {
			toSendPlayerSocket->write(toSend.getTcpSend());
		}
	}
}

void emptyServerUI::showDealer()const {
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showDealer is called!\" չʾdealer");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showDealer, this->m_game->getDealer());
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showCommonCards(vector<card> const & commonCards) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showCommonCards is called!\" չʾ������");
#endif // DEBUG_MESSAGE

	tcpCommandToClient command = tcpCommandToClient::showCommonCardsCommand;
	commandAndDataToClient toSend(command, commonCards);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hideCommonCards() const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hideCommonCards is called!\" ���ع�����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideCommonCardsCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showRound(gameRound nowRound) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showRound is called!\" չʾround");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showRoundCommand, (int)nowRound);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hideRound() const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hideRound is called!\" ����round");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideRoundCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPot(const int potNum) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPot is called!\" չʾ�׳�");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPotCommand, potNum);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePot() const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePot is called!\" ���ص׳�");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePotCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showBegin(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showBegin is called!\" չʾ��ʼ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showBeginCommand);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::hideBegin(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hideBegin is called!\" ���ؿ�ʼ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideBeginCommand);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::showPlayerHandCards(const int playerIndex, vector<card> const & handCards) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerHandCards is called!\" չʾ��� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerHandCardsCommand, playerIndex, handCards);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::showPlayerName(const int playerIndex, string const & playerName)const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerName is called!\" չʾ��� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerNameCommand, playerIndex, playerName);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerChip(const int playerIndex, const int chip) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerChip is called!\" չʾ��� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerChipCommand, playerIndex, chip);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerActionMessage(const int playerIndex, string const & actionMessage)const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerActionMessage is called!\" չʾ��� = " + std::to_string(playerIndex) + " �ж���Ϣ");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerActionMessageCommand, playerIndex, actionMessage);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerSidePot(const int playerIndex, const int money) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerSidePot is called!\" չʾ��� = " + std::to_string(playerIndex) + " �߳�");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerSidePotCommand, playerIndex, money);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerRaiseAction is called!\" չʾ��� = " + std::to_string(nowPlayerIndex) + " ��ע����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerRaiseActionCommand, minRaiseMoney, maxRaiseMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerAllinAction is called!\" չʾ��� = " + std::to_string(nowPlayerIndex) + " allin����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerAllinActionCommand, allinMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerCheckAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerCheckAction is called!\" չʾ��� = " + std::to_string(nowPlayerIndex) + " check����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerCheckActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerCallAction(const int nowPlayerIndex, const int callMoney) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerCallAction is called!\" չʾ��� = " + std::to_string(nowPlayerIndex) + " ��ע����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerCallActionCommand, callMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerFoldAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::showPlayerFoldAction is called!\" չʾ��� = " + std::to_string(nowPlayerIndex) + " ���ƽ���");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerFoldActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerHandCards(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerHandCards is called!\" ������� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerHandCardsCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerName(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerName is called!\" ������� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerNameCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerChip(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerChip is called!\" ������� = " + std::to_string(playerIndex) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerChipCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerActionMessage(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerActionMessage is called!\" ������� = " + std::to_string(playerIndex) + " �ж���Ϣ");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerActionMessageCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerSidePot(const int playerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerSidePot is called!\" ������� = " + std::to_string(playerIndex) + " �߳�");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerSidePotCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerRaiseAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerRaiseAction is called!\" ������� = " + std::to_string(nowPlayerIndex) + " ��ע����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerRaiseActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerAllinAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerAllinAction is called!\" ������� = " + std::to_string(nowPlayerIndex) + " allin����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerAllinActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerCheckAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerCheckAction is called!\" ������� = " + std::to_string(nowPlayerIndex) + " check����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerCheckActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerCallAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerCallAction is called!\" ������� = " + std::to_string(nowPlayerIndex) + " ��ע����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerCallActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerFoldAction(const int nowPlayerIndex) const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"emptyServerUI::hidePlayerFoldAction is called!\" ������� = " + std::to_string(nowPlayerIndex) + " ���ƽ���");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerFoldActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const & handCards)const
{
#ifdef DEBUG_MESSAGE
	m_game->m_debugMessage->debug("\"game::showPlayer1CardsOnPlayer2 is called!\" ����� = " + std::to_string(player2Index) + " ����չʾ��� = " + std::to_string(player1Index) + " ����");
#endif // DEBUG_MESSAGE

	commandAndDataToClient toSend(tcpCommandToClient::showPlayerHandCardsCommand, player1Index, handCards);
	this->sendCommandAndDataToPlayer(player2Index, toSend);
}

void emptyServerUI::showPlayerActionMessageWithTime(const int playerIndex)const {
	commandAndDataToClient toSend(tcpCommandToClient::startTimer, playerIndex, m_timePerAction);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::stopAllClientTimer() const
{
	if (this->m_timePerAction != infiniteTime) {		//����ʱ�䲻�ò���
		commandAndDataToClient toSend(tcpCommandToClient::stopTimer);
		this->sendCommandAndDataToAll(toSend);
	}
}

//analyze���ֳ���ĺ���
void emptyServerUI::setPlayerUniqueName(const int playerIndex, const string& playerName) {
	string noRepeatName = playerName;						//���ظ�������

	std::unordered_set<std::string> playerNameSet;				//������������ϣ�����½���Ա������ά���Ƚ��鷳���Ժ���Ū
	for (const auto& player : this->m_game->getPlayers()) {
		if (player.getPlayerType() != playerType::Empty) {
			playerNameSet.insert(player.getName());
		}
	}

	int index = 1;												//��ԭ������+(����)
	while (playerNameSet.find(noRepeatName) != playerNameSet.end()) {
		noRepeatName = playerName + "(" + std::to_string(index) + ")";
		index++;
	}

	if (noRepeatName != "������"&& noRepeatName != "����") {					//����������֣����Կ��Ǹ�client������ʾ
		this->m_game->setPlayerName(playerIndex, noRepeatName);
		this->showPlayerName(playerIndex, noRepeatName);

		//�������ʾ���˽��룬����Ӧ�÷���newConnect��������û�����֣����ǵ�Ŀǰֻ�ڽ���ʱ�������ͷ���������
		commandAndDataToClient toSend(tcpCommandToClient::showClientChatMessage, "***ϵͳ��Ϣ�����\"" + noRepeatName + "\"��������Ϸ��");
		this->sendCommandAndDataToAll(toSend);

	}
}
void emptyServerUI::setPlayerUniqueMacAddress(const int playerIndex, const string& macAddress) {
	for (int i = 0; i < game::maxNumOfPlayers; i++) {
		player const& curPlayer = this->m_game->getPlayer(i);
		if (curPlayer.getPlayerType() != playerType::Empty && curPlayer.getMacAddress() == macAddress) {	//ͬһmac��������client��server
																											//�ڿͻ��˴����	//����һ�°ɣ���ֹ�������⣺��ͬmac��ַ��Ͽ����ӣ�������Ϣ���ͻ���
			commandAndDataToClient toSend(tcpCommandToClient::sameMacAddress);
			this->sendCommandAndDataToPlayer(playerIndex, toSend);

			//��Ҵ���Ϊ���ܣ�����ͬ���disconnect
			this->m_game->playerEscape(playerIndex);

			//����socket���
			QTcpSocket *fromPlayerClientSocket = this->m_arrayClient[playerIndex];
			this->m_SocketMap.erase(fromPlayerClientSocket);
			this->m_arrayClient[playerIndex] = nullptr;
			this->m_tcpPackageAnalyzers[playerIndex].clear();
		}
	}

	this->m_game->setPlayerMacAddress(playerIndex, macAddress);
	unordered_map<string, int>& macAddressToScoreChartIndex = this->m_game->getMacAddressToScoreChartIndex();
	QStandardItemModel* scoreChart = this->m_game->getScoreChart();

	if (macAddressToScoreChartIndex.find(macAddress) == macAddressToScoreChartIndex.end()) {	//mac-�÷ֱ�����ӳ�����δ�ҵ���ǰmac��ַ
		const int nowScoreChartRowCount = scoreChart->rowCount();
		scoreChart->insertRow(nowScoreChartRowCount);
		scoreChart->setItem(nowScoreChartRowCount, 1, new QStandardItem("0"));
		scoreChart->setItem(nowScoreChartRowCount, 2, new QStandardItem("0"));
		scoreChart->setItem(nowScoreChartRowCount, 3, new QStandardItem("0"));
		scoreChart->setItem(nowScoreChartRowCount, 4, new QStandardItem("0"));

		macAddressToScoreChartIndex[macAddress] = nowScoreChartRowCount;	//�������в����mac��ַ
	}
	const int clientScoreChartIndex = macAddressToScoreChartIndex[macAddress];
	const string clientPlayerName = this->m_game->getPlayer(playerIndex).getName();

	scoreChart->setItem(clientScoreChartIndex, 0, new QStandardItem(QString::fromLocal8Bit(clientPlayerName.data())));	//����
	int totalBuyIn = scoreChart->item(clientScoreChartIndex, 1)->text().toInt();
	int totalChip = scoreChart->item(clientScoreChartIndex, 2)->text().toInt();
	if (totalChip == 0) {
		totalBuyIn += game::initChip;				//��һ��buyIn
		totalChip = game::initChip;
	}
	else if (totalChip > 0 && totalChip < game::initChip) {
		totalBuyIn += game::initChip - totalChip;	//�����
		totalChip = game::initChip;
	}
	else if (totalChip >= game::initChip) {

	}
	int onTableChip = game::initChip;
	int totalWin = totalChip - totalBuyIn;

	scoreChart->setItem(clientScoreChartIndex, 1, new QStandardItem(QString::number(totalBuyIn)));						//�ܴ���
	scoreChart->setItem(clientScoreChartIndex, 2, new QStandardItem(QString::number(totalChip)));						//��ǰ�ܳ���
	scoreChart->setItem(clientScoreChartIndex, 3, new QStandardItem(QString::number(onTableChip)));						//�������ϵĳ���
	scoreChart->setItem(clientScoreChartIndex, 4, new QStandardItem(QString::number(totalWin)));						//��ǰ��ʤ
}
//�ۺ���
void emptyServerUI::newConnectionSlot() {
	//QString testMessage = "���ӳɹ�";
	QTcpSocket *currentClient = m_tcpServer->nextPendingConnection();
	connect(currentClient, SIGNAL(readyRead()), this, SLOT(readData()));	//���ӵ��ۺ���
	connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));

	const int addPlayerIndex = this->m_game->addNewPlayer("���");
	m_arrayClient[addPlayerIndex] = currentClient;		//socket����������
	m_SocketMap[currentClient] = addPlayerIndex;		//��¼socket��Ӧ�����

	this->m_game->getPlayer(addPlayerIndex).setName("���" + to_string(addPlayerIndex));	//��ʱ�����������

																							//�������ݸ�client
																							//�˴���֪����ڱ�����Ϸ�е����
	commandAndDataToClient setClientIndex(tcpCommandToClient::setClientPlayerIndex, addPlayerIndex);
	currentClient->write(setClientIndex.getTcpSend());

	//��֪�ͻ��� ��Ϸģʽ
	std::string modeText(this->m_gameModeDisplay->text().toLocal8Bit());
	modeText += "��˼��ʱ�䣺";
	if (this->m_timePerAction == infiniteTime) {
		modeText += "����";
	}
	else {
		modeText += std::to_string(this->m_timePerAction);
	}
	
	commandAndDataToClient toSend(tcpCommandToClient::setGameMode, modeText);
	this->sendCommandAndDataToPlayer(addPlayerIndex, toSend);
	
	vector<player> const& playersArray = this->m_game->getPlayers();
	//�����Ϸ�Ѿ���ʼ
	if (this->m_game->getGameHasStarted()) {
		for (int i = 0; i < (int)playersArray.size(); ++i) {
			if (playersArray[i].getPlayerType() != playerType::Empty) {
				this->m_game->showPlayerName(i);
				this->m_game->showPlayerChip(i);
			}
		}
		this->showCommonCards(this->m_game->getCommonCards());
		this->m_game->showOthersCardBackOnPlayerIndex(addPlayerIndex);	//��Ⱦ�����ƺ��Ʊ������Թ��ߵ����ϣ�����������Ϣ��ʾ����
		return;
	}

	//Ȼ����Ⱦ�ڳ����
	//����ҽ������׼����Ӧ��ȡ��׼��
	for (int i = 0; i < (int)playersArray.size(); ++i) {
		if (playersArray[i].getPlayerType() != playerType::Empty) {
			this->m_game->showPlayerName(i);
			this->m_game->showPlayerChip(i);
			this->hidePlayerActionMessage(i);	//��ʾ ȡ��׼��
		}
		if (playersArray[i].getPlayerType() == playerType::OnSitePlayer
			|| (playersArray[i].getPlayerType() == playerType::Looker && playersArray[i].getChip() > 0)) {
			this->showBegin(i);					//��ʾbegin����
		}
	}
	this->m_game->clearNumOfReadyPlayer();		//ready�������
	//this->showBegin(addPlayerIndex);

	
	
}

void emptyServerUI::readData()
{
	/*const int nowPlayerIndex = this->m_game->getNowPlayerIndex();
	QTcpSocket *nowClientSocket = this->m_arrayClient[nowPlayerIndex];*/

	QTcpSocket *nowClientSocket = qobject_cast<QTcpSocket*>(sender());
	if (m_SocketMap.find(nowClientSocket) == m_SocketMap.end()) {
		return;	//����
	}
	const int nowPlayerIndex = this->m_SocketMap[nowClientSocket];	//��ȡ��ǰ������Ϣ��������
	
	QByteArray newData = nowClientSocket->readAll();				//��readyRead�����Ѿ�����
	vector<QByteArray> tcpPackages = this->m_tcpPackageAnalyzers[nowPlayerIndex].getTcpPackages(newData);
	for (QByteArray const& commandAndDataArray : tcpPackages) {
		this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	}

	//QByteArray& nowBuffer = this->m_buffers[nowPlayerIndex];		//��ǰ��һ��������
	//int& nowHeadLen = this->m_headLens[nowPlayerIndex];				//��ǰ��Ұ���������

	//QByteArray newData = nowClientSocket->readAll();	//��readyRead�����Ѿ�����
	//

	//nowBuffer = nowBuffer.append(buffer);

	//while (1) {		//����һ�ζ��˶���������˸�ѭ������֪���Բ���
	//	if (nowHeadLen <= 0) {	//���峤�������壬��Ҫ��ȡ���峤��
	//		if (nowBuffer.length() < 4)		//ǰ4�ֽ��ǰ��峤��,�����͵��´�
	//			return;
	//		QByteArray head = nowBuffer.left(4);
	//		nowBuffer.remove(0, 4);
	//		nowHeadLen = bytes4ToInt(head);
	//	}
	//	const int len = nowBuffer.length();
	//	if (len < nowHeadLen) {	//���ݳ��Ȳ��������´�
	//		return;
	//	}
	//	QByteArray commandAndDataArray = nowBuffer.left(nowHeadLen);
	//	nowBuffer.remove(0, nowHeadLen);
	//	nowHeadLen = 0;						//�������ݣ����ð��峤��

	//												//����������в�����
	//	this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	//}
}

void emptyServerUI::disconnectionSlot() {	//�пͻ��˶Ͽ�����
	QTcpSocket *disconnectClientSocket = qobject_cast<QTcpSocket*>(sender());
	auto disconnectIter = this->m_SocketMap.find(disconnectClientSocket);
	if (disconnectIter == this->m_SocketMap.end()) {	//û�ҵ�
		return;
	}
	const int disconnectPlayerIndex = disconnectIter->second;

	//�����Ϸû��ʼ��ȡ�������׼��״̬
	if (!this->m_game->getGameHasStarted()) {
		vector<player> const& playersArray = this->m_game->getPlayers();
		for (int i = 0; i < (int)playersArray.size(); ++i) {
			if (i == disconnectPlayerIndex) {		//�Ͽ���Ҳ�����ʾ
				continue;
			}
			if (playersArray[i].getPlayerType() != playerType::Empty) {
				//this->m_game->showPlayerChip(i);	//��Ϸû��ʼ�����ø��³���
				this->hidePlayerActionMessage(i);	//��ʾ ȡ��׼��
			}
			if (playersArray[i].getPlayerType() == playerType::OnSitePlayer
				|| (playersArray[i].getPlayerType() == playerType::Looker && playersArray[i].getChip() > 0)) {
				this->showBegin(i);					//��ʾbegin����
			}
		}
		this->m_game->clearNumOfReadyPlayer();		//ready�������
	}	
	
	//�������ʾ���˽���
	commandAndDataToClient toSend(tcpCommandToClient::showClientChatMessage, "***ϵͳ��Ϣ�����\"" + this->m_game->getPlayer(disconnectPlayerIndex).getName() + "\"�뿪����Ϸ��");
	this->sendCommandAndDataToAllExcept(disconnectPlayerIndex, toSend);


	this->m_game->playerEscape(disconnectPlayerIndex);

	//����socket���,Tcpserver��Ҫ���д�����
	this->m_SocketMap.erase(disconnectClientSocket);
	this->m_arrayClient[disconnectPlayerIndex] = nullptr;
	
	this->m_tcpPackageAnalyzers[disconnectPlayerIndex].clear();
	/*this->m_buffers[disconnectPlayerIndex].clear();
	this->m_headLens[disconnectPlayerIndex] = 0;*/

	if (this->m_SocketMap.size() < 1) {		//����1����������˳�
		QApplication::exit();
	}
}

void emptyServerUI::setPort()
{
	if (m_timePerActionInput->text().size() > 0) {
		int timeInput = m_timePerActionInput->text().toInt();
		if (timeInput < 5 || timeInput > 99) {
			QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("����Ӧ��5~99֮��"));
			return;
		}
		m_timePerAction = timeInput;
		m_timePerActionShow->setText(QStringLiteral("˼��ʱ�䣺") + QString::number(m_timePerAction));
	}
	else {
		m_timePerAction = infiniteTime;
		m_timePerActionShow->setText(QStringLiteral("˼��ʱ�䣺����"));
	}
	m_timePerActionInput->hide();


	int aaa = m_port->text().toInt();
	qint32 port = m_port->text().toInt();
	m_tcpServer->listen(QHostAddress::Any, port);	//���ü����˿�
	//��ʾ����
	m_port->hide();
	m_setPort->hide();
	m_portDisplay->setText(QStringLiteral("�˿ں�Ϊ��") + QString::number(port));

	//������Ϸģʽ
	for (auto& button : m_gameMode->buttons()) {
		button->hide();
	}

	//m_gameMode->checkedButton();		//�����button
	if (this->nomalMode->isChecked()) {					//����ģʽ 
		m_gameModeDisplay->setText(QStringLiteral("��ϷģʽΪ������ģʽ"));		
	} else if (this->shortDeckMode->isChecked()) {
		this->m_game->removeCardsInHeap(2, 5);			//���Ʋ�Ҫ2~5
		m_gameModeDisplay->setText(QStringLiteral("��ϷģʽΪ������ģʽ����2��5��"));
	}
	m_gameModeDisplay->show();
}

void emptyServerUI::closeEvent(QCloseEvent *e) {
	//for (auto clientSocket : this->m_arrayClient) {
	//	if (clientSocket != nullptr) {
	//		//clientSocket->close();
	//		clientSocket->disconnect();
	//	}
	//}

	//if (QMessageBox::question(this,
	//	tr("Quit"),
	//	tr("Are you sure to quit this application?"),
	//	QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
	//	e->accept();//���Ὣ�¼����ݸ�����ĸ����

	//	qDebug() << "ok";
	//}
	//else
	//	e->ignore();
}
