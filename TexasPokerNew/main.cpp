#include "TexasPokerNew.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	vector<player> playerArray;
	for (int i = 0; i < 8; ++i) {
		string playerName = "player";
		playerName += i + '0';
		player newPlayer(OnSitePlayer, playerName, {}, 200, 0);
		playerArray.push_back(newPlayer);
	}

	game myGame = game("game1", playerArray, {}, 0, {}, 0, cardHeap(), ErrorRound, -1, -1, rand() % game::maxNumOfPlayers, {}, nullptr);
	//game myG("game1", playerArray);
	TexasPokerNew w;
	w.setGame(&myGame);
	Ui_singleGameWindow *singleGameWindow = new Ui_singleGameWindow(w.getUi().centralWidget);
	myGame.setVirUIPoint(singleGameWindow);

	myGame.begin();

    w.show();
    return a.exec();
}
