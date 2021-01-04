#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_TexasPokerNew.h"
#include "TexasPokerSingleUi.h"

class TexasPokerNew : public QMainWindow
{
    Q_OBJECT

public:
    TexasPokerNew(QWidget *parent = Q_NULLPTR);
	void setGame(game* g){ this->ui.table->m_game = g; };
	Ui::TexasPokerNewClass getUi()const { return this->ui; };
public slots:
	void nowPlayerRaise();
private:
    Ui::TexasPokerNewClass ui;
};
