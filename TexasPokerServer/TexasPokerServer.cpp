#include "TexasPokerServer.h"

emptyServerUI::emptyServerUI(QWidget *parent, game* g)
	:QMainWindow(parent), m_game(g)
{
	setWindowTitle(QStringLiteral("德州扑克服务器"));
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

	m_arrayClient.clear();
	m_arrayClient = vector<QTcpSocket*>(game::maxNumOfPlayers, nullptr);

	m_buffers = vector<QByteArray>(game::maxNumOfPlayers);
	for (auto buffer : m_buffers)buffer.clear();
	m_headLens = vector<int>(game::maxNumOfPlayers, 0);
	m_SocketMap.clear();

	m_tcpServer = new QTcpServer(this);
	QString hostAddress = QNetworkInterface().allAddresses().at(1).toString();
	quint16 port = 6060;
	m_tcpServer->listen(QHostAddress::Any, port);
	
	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}



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
	else if (receivedCommand&tcpCommandToServer::playerReadyCommand) {	//命令from的玩家ready了
		this->showPlayerActionMessage(fromPlayerIndex, "准备！");
		this->m_game->begin();
	}
}

void emptyServerUI::sendCommandAndDataToPlayer(const int playerIndex, commandAndDataToClient toSend)const
{
	QTcpSocket *toSendPlayerSocket = this->m_arrayClient[playerIndex];
	QByteArray toSendQByteArray = toSend.getTcpSend();
	if (toSendPlayerSocket == nullptr) {
		//QMessageBox::about(this, QStringLiteral("错误"), QStringLiteral("发送的玩家socket无效"));
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

//槽函数
void emptyServerUI::newConnectionSlot() {
	//QString testMessage = "连接成功";
	QTcpSocket *currentClient = m_tcpServer->nextPendingConnection();
	connect(currentClient, SIGNAL(readyRead()), this, SLOT(readData()));	//连接到槽函数
	connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));

	const int addPlayerIndex = this->m_game->addNewPlayer("玩家");
	m_arrayClient[addPlayerIndex] = currentClient;		//socket加入数组中
	m_SocketMap[currentClient] = addPlayerIndex;		//记录socket对应的玩家

	this->m_game->getPlayer(addPlayerIndex).setName("玩家" + to_string(addPlayerIndex));	//暂时用这个起名字

																						//返回数据给client
																						//此处告知玩家在本局游戏中的序号
	commandAndDataToClient setClientIndex(tcpCommandToClient::setClientPlayerIndex, addPlayerIndex);
	currentClient->write(setClientIndex.getTcpSend());

	//然后渲染在场玩家
	//新玩家进入后，已准备的应当取消准备
	vector<player> const& playersArray = this->m_game->getPlayers();
	for (int i = 0; i < (int)playersArray.size(); ++i) {
		if (playersArray[i].getPlayerType() != playerType::Empty) {
			this->m_game->showPlayerName(i);
			this->m_game->showPlayerChip(i);
			this->hidePlayerActionMessage(i);	//显示 取消准备
		}
		if (playersArray[i].getPlayerType() == playerType::OnSitePlayer
			|| (playersArray[i].getPlayerType() == playerType::Looker && playersArray[i].getChip() > 0)) {
			this->showBegin(i);					//显示begin按键
		}
	}
	this->m_game->clearNumOfReadyPlayer();		//ready人数清空
	//this->showBegin(addPlayerIndex);

	
	
}

void emptyServerUI::readData()
{
	/*const int nowPlayerIndex = this->m_game->getNowPlayerIndex();
	QTcpSocket *nowClientSocket = this->m_arrayClient[nowPlayerIndex];*/

	QTcpSocket *nowClientSocket = qobject_cast<QTcpSocket*>(sender());
	if (m_SocketMap.find(nowClientSocket) == m_SocketMap.end()) {
		return;	//错误
	}
	const int nowPlayerIndex = this->m_SocketMap[nowClientSocket];	//获取当前发送信息的玩家序号

	QByteArray& nowBuffer = this->m_buffers[nowPlayerIndex];		//当前玩家缓存的引用
	int& nowHeadLen = this->m_headLens[nowPlayerIndex];				//当前玩家包长的引用

	QByteArray buffer = nowClientSocket->readAll();	//用readyRead触发已经读空
	nowBuffer = nowBuffer.append(buffer);

	while (1) {		//可能一次读了多个包，加了个循环，不知道对不对
		if (nowHeadLen <= 0) {	//包体长度无意义，需要读取包体长度
			if (nowBuffer.length() < 4)		//前4字节是包体长度,不够就等下次
				return;
			QByteArray head = nowBuffer.left(4);
			nowBuffer.remove(0, 4);
			nowHeadLen = bytes4ToInt(head);
		}
		const int len = nowBuffer.length();
		if (len < nowHeadLen) {	//数据长度不够，等下次
			return;
		}
		QByteArray commandAndDataArray = nowBuffer.left(nowHeadLen);
		nowBuffer.remove(0, nowHeadLen);
		nowHeadLen = 0;						//读完数据，重置包体长度

													//根据命令进行操作了
		this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	}
}

void emptyServerUI::disconnectionSlot() {	//有客户端断开连接
	QTcpSocket *disconnectClientSocket = qobject_cast<QTcpSocket*>(sender());
	if (this->m_SocketMap.find(disconnectClientSocket) == this->m_SocketMap.end()) {	//没找到
		return;
	}
	const int disconnectPlayerIndex = this->m_SocketMap[disconnectClientSocket];
	this->m_game->playerEscape(disconnectPlayerIndex);

	//清理socket相关,Tcpserver需要进行处理吗？
	this->m_SocketMap.erase(disconnectClientSocket);
	this->m_arrayClient[disconnectPlayerIndex] = nullptr;
	this->m_buffers[disconnectPlayerIndex].clear();
	this->m_headLens[disconnectPlayerIndex] = 0;
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
	//	e->accept();//不会将事件传递给组件的父组件

	//	qDebug() << "ok";
	//}
	//else
	//	e->ignore();
}
