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

	m_arrayClient = vector<QTcpSocket*>(game::maxNumOfPlayers, nullptr);

	m_buffers = vector<QByteArray>(game::maxNumOfPlayers);
	for (auto& buffer : m_buffers)buffer.clear();
	m_headLens = vector<int>(game::maxNumOfPlayers, 0);
	m_SocketMap = map<QTcpSocket*, int>();
	m_SocketMap.clear();

	m_tcpServer = new QTcpServer(this);
	
	/*QString hostAddress = QNetworkInterface().allAddresses().at(1).toString();*/
	//quint16 port = 6060;
	//m_tcpServer->listen(QHostAddress::Any, port);
	
	connect(m_setPort, SIGNAL(clicked()), this, SLOT(setPort()));
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}


//��֧Ӧ����==д
void emptyServerUI::analyzeCommand(QByteArray received, const int fromPlayerIndex)
{
	QByteArray commandArray = received.left(commandAndDataToServer::byteOfInt);
	QByteArray dataArray = received;
	dataArray.remove(0, commandAndDataToServer::byteOfInt);

	tcpCommandToServer receivedCommand = (tcpCommandToServer)bytes4ToInt(commandArray);

	if (receivedCommand&tcpCommandToServer::noCommandToServer) {
		return;
	}
	else if (receivedCommand&tcpCommandToServer::nowPlayerRaiseCommand) {
		const int raiseTo = bytes4ToInt(dataArray);
		this->m_game->nowPlayerRaise(raiseTo);
	}
	else if (receivedCommand&tcpCommandToServer::nowPlayerAllinCommand) {
		this->m_game->nowPlayerAllin();
	}
	else if (receivedCommand&tcpCommandToServer::nowPlayerCheckCommand) {
		this->m_game->nowPlayerCheck();
	}
	else if (receivedCommand&tcpCommandToServer::nowPlayerCallCommand) {
		this->m_game->nowPlayerCall();
	}
	else if (receivedCommand&tcpCommandToServer::nowPlayerFoldCommand) {
		this->m_game->nowPlayerFold();
	}
	else if (receivedCommand&tcpCommandToServer::playerReadyCommand) {	//����from�����ready��
		this->showPlayerActionMessage(fromPlayerIndex, "׼����");
		this->m_game->addNumOfReadyPlayer();							//ready������һ����begin���������
		this->m_game->begin();
	}
	else if (receivedCommand&tcpCommandToServer::setPlayerNameCommand) {
		const string playerName = dataArray.toStdString();
		if (playerName != "������"&&playerName != "����") {			//����������֣����Կ��Ǹ�client������ʾ
			this->m_game->setPlayerName(fromPlayerIndex, playerName);
			this->showPlayerName(fromPlayerIndex, playerName);
		}
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

void emptyServerUI::showCommonCards(vector<card> const & commonCards) const
{
	tcpCommandToClient command = tcpCommandToClient::showCommonCardsCommand;
	commandAndDataToClient toSend(command, commonCards);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hideCommonCards() const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideCommonCardsCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showRound(gameRound nowRound) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showRoundCommand, (int)nowRound);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hideRound() const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideRoundCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPot(const int potNum) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPotCommand, potNum);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePot() const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePotCommand);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showBegin(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showBeginCommand);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::hideBegin(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideBeginCommand);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::showPlayerHandCards(const int playerIndex, vector<card> const & handCards) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerHandCardsCommand, playerIndex, handCards);
	this->sendCommandAndDataToPlayer(playerIndex, toSend);
}

void emptyServerUI::showPlayerName(const int playerIndex, string const & playerName)const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerNameCommand, playerIndex, playerName);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerChip(const int playerIndex, const int chip) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerChipCommand, playerIndex, chip);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerActionMessage(const int playerIndex, string const & actionMessage)const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerActionMessageCommand, playerIndex, actionMessage);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerSidePot(const int playerIndex, const int money) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerSidePotCommand, playerIndex, money);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::showPlayerRaiseAction(const int nowPlayerIndex, const int minRaiseMoney, const int maxRaiseMoney) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerRaiseActionCommand, minRaiseMoney, maxRaiseMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerAllinAction(const int nowPlayerIndex, const int allinMoney) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerAllinActionCommand, allinMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerCheckAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerCheckActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerCallAction(const int nowPlayerIndex, const int callMoney) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerCallActionCommand, callMoney);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayerFoldAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::showClientPlayerFoldActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerHandCards(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerHandCardsCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerName(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerNameCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerChip(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerChipCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerActionMessage(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerActionMessageCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerSidePot(const int playerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hidePlayerSidePotCommand, playerIndex);
	this->sendCommandAndDataToAll(toSend);
}

void emptyServerUI::hidePlayerRaiseAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerRaiseActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerAllinAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerAllinActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerCheckAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerCheckActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerCallAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerCallActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::hidePlayerFoldAction(const int nowPlayerIndex) const
{
	commandAndDataToClient toSend(tcpCommandToClient::hideClientPlayerFoldActionCommand);
	this->sendCommandAndDataToPlayer(nowPlayerIndex, toSend);
}

void emptyServerUI::showPlayer1HandCardOnPlayer2(const int player1Index, const int player2Index, vector<card> const & handCards)const
{
	commandAndDataToClient toSend(tcpCommandToClient::showPlayerHandCardsCommand, player1Index, handCards);
	this->sendCommandAndDataToPlayer(player2Index, toSend);
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

	QByteArray& nowBuffer = this->m_buffers[nowPlayerIndex];		//��ǰ��һ��������
	int& nowHeadLen = this->m_headLens[nowPlayerIndex];				//��ǰ��Ұ���������

	QByteArray buffer = nowClientSocket->readAll();	//��readyRead�����Ѿ�����
	nowBuffer = nowBuffer.append(buffer);

	while (1) {		//����һ�ζ��˶���������˸�ѭ������֪���Բ���
		if (nowHeadLen <= 0) {	//���峤�������壬��Ҫ��ȡ���峤��
			if (nowBuffer.length() < 4)		//ǰ4�ֽ��ǰ��峤��,�����͵��´�
				return;
			QByteArray head = nowBuffer.left(4);
			nowBuffer.remove(0, 4);
			nowHeadLen = bytes4ToInt(head);
		}
		const int len = nowBuffer.length();
		if (len < nowHeadLen) {	//���ݳ��Ȳ��������´�
			return;
		}
		QByteArray commandAndDataArray = nowBuffer.left(nowHeadLen);
		nowBuffer.remove(0, nowHeadLen);
		nowHeadLen = 0;						//�������ݣ����ð��峤��

													//����������в�����
		this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	}
}

void emptyServerUI::disconnectionSlot() {	//�пͻ��˶Ͽ�����
	QTcpSocket *disconnectClientSocket = qobject_cast<QTcpSocket*>(sender());
	if (this->m_SocketMap.find(disconnectClientSocket) == this->m_SocketMap.end()) {	//û�ҵ�
		return;
	}
	const int disconnectPlayerIndex = this->m_SocketMap[disconnectClientSocket];
	this->m_game->playerEscape(disconnectPlayerIndex);

	//����socket���,Tcpserver��Ҫ���д�����
	this->m_SocketMap.erase(disconnectClientSocket);
	this->m_arrayClient[disconnectPlayerIndex] = nullptr;
	this->m_buffers[disconnectPlayerIndex].clear();
	this->m_headLens[disconnectPlayerIndex] = 0;

	if (this->m_SocketMap.size() < 1) {		//����1����������˳�
		QApplication::exit();
	}
	//�����Ϸû��ʼ
	if (!this->m_game->getGameHasStarted()) {
		this->m_game->begin();
	}
	
}

void emptyServerUI::setPort()
{
	int aaa = m_port->text().toInt();
	qint32 port = m_port->text().toInt();
	m_tcpServer->listen(QHostAddress::Any, port);	//���ü����˿�
	//��ʾ����
	m_port->hide();
	m_setPort->hide();
	m_portDisplay->setText(QStringLiteral("�˿ں�Ϊ��") + QString::number(port));
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
