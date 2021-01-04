#include "TexasPokerNew.h"
#include "singleGame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    TexasPokerNew w;	
	
	vector<player> playerArray;
	for (int i = 0; i < 8; ++i) {
		string playerName = "player";
		playerName += i + '0';
		player newPlayer(OnSitePlayer, playerName, {}, 200, 0);
		playerArray.push_back(newPlayer);
	}

	singleGame myGame = singleGame("game1", playerArray, {}, 0, {}, 0, cardHeap(), ErrorRound, -1, -1, rand() % game::maxNumOfPlayers, {}, nullptr);
	//game myG("game1", playerArray);
	//Ui_singleGameWindow *singleGameWindow = new Ui_singleGameWindow(w.getUi().centralWidget);
	//myGame.setGameWindow(singleGameWindow);

    
    w.show();
    return a.exec();
}
