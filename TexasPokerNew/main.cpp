//#include "TexasPokerNew.h"
#include <QtWidgets/QApplication>
#include "singleGameUI.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	vector<player> playerArray;
	for (int i = 0; i < 8; ++i) {
		string playerName = "Íæ¼Ò";
		playerName += i + '0';
		player newPlayer(OnSitePlayer, playerName, {}, 200, 0);
		playerArray.push_back(newPlayer);
	}

	game myGame = game("game1", playerArray, {}, 0, {}, 0, cardHeap(), ErrorRound, -1, -1, rand() % game::maxNumOfPlayers, {}, nullptr);
	singleGameWindow w;
	//Áª½á
	myGame.setVirUIPoint(&w);
	w.setGamePoint(&myGame);
    
	for (int i = 0; i < game::maxNumOfPlayers; ++i) {
		if (myGame.getPlayer(i).getPlayerType() == playerType::OnSitePlayer) {
			myGame.showPlayerName(i);
			myGame.showPlayerChip(i);
		}
	}

	w.show();
	//myGame.begin();
	
	return a.exec();
}
