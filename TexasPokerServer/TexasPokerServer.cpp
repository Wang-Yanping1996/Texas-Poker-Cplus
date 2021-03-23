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

	QIcon icon;
	icon.addFile(QStringLiteral("image/tubiao/poker_window_tubiao.ico"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);

	//设定端口号
	m_setPort = new QPushButton(centralWidget);
	m_setPort->setObjectName(QStringLiteral("setPort"));
	m_setPort->setGeometry(QRect(500, 370, 100, 30));
	m_setPort->raise();
	m_setPort->setText(QStringLiteral("设置端口"));
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
	m_portDisplay->setText(QStringLiteral("端口号为："));
	m_portDisplay->raise();
	m_portDisplay->show();

	//游戏模式，正常or短牌
	m_gameMode = new QButtonGroup(centralWidget);
	m_gameMode->setExclusive(true);					//组内按键互斥

	nomalMode = new QRadioButton(centralWidget);
	nomalMode->setText(QStringLiteral("正常模式"));
	nomalMode->setGeometry(QRect(650, 300, 100, 30));
	nomalMode->show();
	m_gameMode->addButton(nomalMode);
	shortDeckMode = new QRadioButton(centralWidget);
	shortDeckMode->setText(QStringLiteral("短牌模式"));
	shortDeckMode->setGeometry(QRect(780, 300, 100, 30));
	shortDeckMode->show();
	m_gameMode->addButton(shortDeckMode);
	
	nomalMode->setChecked(true);

	m_gameModeDisplay = new QLabel(this->centralWidget);		//模式显示
	m_gameModeDisplay->setGeometry(QRect(500, 270, 200, 30));
	m_gameModeDisplay->hide();
	//tcp连接相关初始化
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


//分支应该用==写
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
	else if (receivedCommand == tcpCommandToServer::playerReadyCommand) {	//命令from的玩家ready了
		this->showPlayerActionMessage(fromPlayerIndex, "准备！");
		this->m_game->addNumOfReadyPlayer();								//ready人数加一，从begin中提出来了

		const int numOfPlayer = this->m_game->getNumOfPlayers();			//在此判断游戏是否开始，更合逻辑
		const int numOfReadyPlayer = this->m_game->getNumOfReadyPlayer();
		if (numOfReadyPlayer == numOfPlayer) {
			this->m_game->begin();											//这里，感觉逻辑上应该是，直接判断人数是否相等，以决定是否进入begin。现在这样逻辑不够清晰
		}
	}
	else if (receivedCommand == tcpCommandToServer::setPlayerNameCommand) {
		string newPlayerName = dataArray.toStdString();				//玩家想起的名字
		string noRepeatName = newPlayerName;						//不重复的名字

		std::unordered_set<std::string> playerNameSet;				//生成玩家名集合，如果新建成员变量，维护比较麻烦，以后考虑弄
		for (const auto& player : this->m_game->getPlayers()) {
			if (player.getPlayerType() != playerType::Empty) {
				playerNameSet.insert(player.getName());
			}
		}

		int index = 1;												//在原名后面+(数字)
		while (playerNameSet.find(noRepeatName) != playerNameSet.end()) {
			noRepeatName = newPlayerName + "(" + std::to_string(index) + ")";
			index++;
		}

		if (newPlayerName != "姓名："&&newPlayerName != "姓名") {					//不让起的名字，可以考虑给client返回提示
			this->m_game->setPlayerName(fromPlayerIndex, noRepeatName);
			this->showPlayerName(fromPlayerIndex, noRepeatName);
		}
	}
	else if (receivedCommand == tcpCommandToServer::setClientMacAddressCommand) {
		const string nowClientMacAddress = dataArray.toStdString();
		for (int i = 0; i < game::maxNumOfPlayers; i++) {
			player const& curPlayer = this->m_game->getPlayer(i);
			if (curPlayer.getPlayerType() != playerType::Empty && curPlayer.getMacAddress() == nowClientMacAddress) {	//同一mac连了两个client到server
				//在客户端处理吧
			}
		}

		this->m_game->setPlayerMacAddress(fromPlayerIndex, nowClientMacAddress);
		unordered_map<string, int>& macAddressToScoreChartIndex = this->m_game->getMacAddressToScoreChartIndex();
		QStandardItemModel* scoreChart = this->m_game->getScoreChart();

		if (macAddressToScoreChartIndex.find(nowClientMacAddress) == macAddressToScoreChartIndex.end()) {	//mac-得分表索引映射表中未找到当前mac地址
			const int nowScoreChartRowCount = scoreChart->rowCount();
			scoreChart->insertRow(nowScoreChartRowCount);
			scoreChart->setItem(nowScoreChartRowCount, 1, new QStandardItem("0"));
			scoreChart->setItem(nowScoreChartRowCount, 2, new QStandardItem("0"));
			scoreChart->setItem(nowScoreChartRowCount, 3, new QStandardItem("0"));
			scoreChart->setItem(nowScoreChartRowCount, 4, new QStandardItem("0"));

			macAddressToScoreChartIndex[nowClientMacAddress] = nowScoreChartRowCount;	//索引表中插入该mac地址
		}
		const int clientScoreChartIndex = macAddressToScoreChartIndex[nowClientMacAddress];
		const string clientPlayerName = this->m_game->getPlayer(fromPlayerIndex).getName();
		
		scoreChart->setItem(clientScoreChartIndex, 0, new QStandardItem(QString::fromLocal8Bit(clientPlayerName.data())));	//名字
		int totalBuyIn = scoreChart->item(clientScoreChartIndex, 1)->text().toInt();
		int totalChip = scoreChart->item(clientScoreChartIndex, 2)->text().toInt();
		if (totalChip == 0) {
			totalBuyIn += game::initChip;				//再一次buyIn
			totalChip = game::initChip;
		}
		else if (totalChip > 0 && totalChip < game::initChip) {
			totalBuyIn += game::initChip - totalChip;	//补差价
			totalChip = game::initChip;
		}
		else if (totalChip >= game::initChip) {

		}
		int onTableChip = game::initChip;
		int totalWin = totalChip - totalBuyIn;

		scoreChart->setItem(clientScoreChartIndex, 1, new QStandardItem(QString::number(totalBuyIn)));						//总带入
		scoreChart->setItem(clientScoreChartIndex, 2, new QStandardItem(QString::number(totalChip)));						//当前总筹码
		scoreChart->setItem(clientScoreChartIndex, 3, new QStandardItem(QString::number(onTableChip)));						//带到桌上的筹码
		scoreChart->setItem(clientScoreChartIndex, 4, new QStandardItem(QString::number(totalWin)));						//当前净胜
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

void emptyServerUI::sendCommandAndDataToAllExcept(const int exceptPlayerIndex, commandAndDataToClient toSend)
{
	for (auto const& toSendPlayerSocket : this->m_arrayClient) {
		if (toSendPlayerSocket != nullptr && toSendPlayerSocket != this->m_arrayClient[exceptPlayerIndex]) {
			toSendPlayerSocket->write(toSend.getTcpSend());
		}
	}
}

void emptyServerUI::showDealer()const {
	commandAndDataToClient toSend(tcpCommandToClient::showDealer, this->m_game->getDealer());
	this->sendCommandAndDataToAll(toSend);
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

	//告知客户端 游戏模式
	std::string modeText(this->m_gameModeDisplay->text().toLocal8Bit());
	commandAndDataToClient toSend(tcpCommandToClient::setGameMode, modeText);
	this->sendCommandAndDataToPlayer(addPlayerIndex, toSend);

	vector<player> const& playersArray = this->m_game->getPlayers();
	//如果游戏已经开始
	if (this->m_game->getGameHasStarted()) {
		for (int i = 0; i < (int)playersArray.size(); ++i) {
			if (playersArray[i].getPlayerType() != playerType::Empty) {
				this->m_game->showPlayerName(i);
				this->m_game->showPlayerChip(i);
			}
		}
		this->showCommonCards(this->m_game->getCommonCards());
		this->m_game->showOthersCardBackOnPlayerIndex(addPlayerIndex);	//渲染公共牌和牌背面在旁观者的桌上，问题在于信息显示不了
		return;
	}

	//然后渲染在场玩家
	//新玩家进入后，已准备的应当取消准备
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
	
	QByteArray newData = nowClientSocket->readAll();				//用readyRead触发已经读空
	vector<QByteArray> tcpPackages = this->m_tcpPackageAnalyzers[nowPlayerIndex].getTcpPackages(newData);
	for (QByteArray const& commandAndDataArray : tcpPackages) {
		this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	}

	//QByteArray& nowBuffer = this->m_buffers[nowPlayerIndex];		//当前玩家缓存的引用
	//int& nowHeadLen = this->m_headLens[nowPlayerIndex];				//当前玩家包长的引用

	//QByteArray newData = nowClientSocket->readAll();	//用readyRead触发已经读空
	//

	//nowBuffer = nowBuffer.append(buffer);

	//while (1) {		//可能一次读了多个包，加了个循环，不知道对不对
	//	if (nowHeadLen <= 0) {	//包体长度无意义，需要读取包体长度
	//		if (nowBuffer.length() < 4)		//前4字节是包体长度,不够就等下次
	//			return;
	//		QByteArray head = nowBuffer.left(4);
	//		nowBuffer.remove(0, 4);
	//		nowHeadLen = bytes4ToInt(head);
	//	}
	//	const int len = nowBuffer.length();
	//	if (len < nowHeadLen) {	//数据长度不够，等下次
	//		return;
	//	}
	//	QByteArray commandAndDataArray = nowBuffer.left(nowHeadLen);
	//	nowBuffer.remove(0, nowHeadLen);
	//	nowHeadLen = 0;						//读完数据，重置包体长度

	//												//根据命令进行操作了
	//	this->analyzeCommand(commandAndDataArray, nowPlayerIndex);
	//}
}

void emptyServerUI::disconnectionSlot() {	//有客户端断开连接
	QTcpSocket *disconnectClientSocket = qobject_cast<QTcpSocket*>(sender());
	auto disconnectIter = this->m_SocketMap.find(disconnectClientSocket);
	if (disconnectIter == this->m_SocketMap.end()) {	//没找到
		return;
	}
	const int disconnectPlayerIndex = disconnectIter->second;

	//如果游戏没开始，取消各玩家准备状态
	if (!this->m_game->getGameHasStarted()) {
		vector<player> const& playersArray = this->m_game->getPlayers();
		for (int i = 0; i < (int)playersArray.size(); ++i) {
			if (i == disconnectPlayerIndex) {		//断开玩家不做显示
				continue;
			}
			if (playersArray[i].getPlayerType() != playerType::Empty) {
				//this->m_game->showPlayerChip(i);	//游戏没开始，不用更新筹码
				this->hidePlayerActionMessage(i);	//显示 取消准备
			}
			if (playersArray[i].getPlayerType() == playerType::OnSitePlayer
				|| (playersArray[i].getPlayerType() == playerType::Looker && playersArray[i].getChip() > 0)) {
				this->showBegin(i);					//显示begin按键
			}
		}
		this->m_game->clearNumOfReadyPlayer();		//ready人数清空
	}	
	
	this->m_game->playerEscape(disconnectPlayerIndex);

	//清理socket相关,Tcpserver需要进行处理吗？
	this->m_SocketMap.erase(disconnectClientSocket);
	this->m_arrayClient[disconnectPlayerIndex] = nullptr;
	
	this->m_tcpPackageAnalyzers[disconnectPlayerIndex].clear();
	/*this->m_buffers[disconnectPlayerIndex].clear();
	this->m_headLens[disconnectPlayerIndex] = 0;*/

	if (this->m_SocketMap.size() < 1) {		//少于1人则服务器退出
		QApplication::exit();
	}

	
	
}

void emptyServerUI::setPort()
{
	int aaa = m_port->text().toInt();
	qint32 port = m_port->text().toInt();
	m_tcpServer->listen(QHostAddress::Any, port);	//设置监听端口
	//显示更新
	m_port->hide();
	m_setPort->hide();
	m_portDisplay->setText(QStringLiteral("端口号为：") + QString::number(port));

	//设置游戏模式
	for (auto& button : m_gameMode->buttons()) {
		button->hide();
	}

	//m_gameMode->checkedButton();		//激活的button
	if (this->nomalMode->isChecked()) {					//正常模式 
		m_gameModeDisplay->setText(QStringLiteral("游戏模式为：正常模式"));		
	} else if (this->shortDeckMode->isChecked()) {
		this->m_game->removeCardsInHeap(2, 5);			//短牌不要2~5
		m_gameModeDisplay->setText(QStringLiteral("游戏模式为：短牌模式（无2到5）"));
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
	//	e->accept();//不会将事件传递给组件的父组件

	//	qDebug() << "ok";
	//}
	//else
	//	e->ignore();
}
