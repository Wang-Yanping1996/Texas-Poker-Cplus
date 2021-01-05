#include "TexasPokerNew.h"

TexasPokerNew::TexasPokerNew(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	for (int i = 0; i < 8; ++i) {
		bool flag = connect(this->ui.table->players[i]->raise, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
		connect(this->ui.table->players[i]->fold, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
		connect(this->ui.table->players[i]->check, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
		connect(this->ui.table->players[i]->call, SIGNAL(clicked()), this, SLOT(nowPlayerRaise()));
	}

	bool flag = connect(this->ui.table->begin, SIGNAL(QPushButton::clicked()), this, SLOT(nowPlayerRaise()));
}


void TexasPokerNew::nowPlayerRaise() {
	const int nowPlayerIndex = ui.table->m_game->getNowPlayerIndex();
	const int raiseMoney = ui.table->players[nowPlayerIndex]->raiseMoney->text().toInt();
	ui.table->m_game->nowPlayerRaise(raiseMoney);
}
