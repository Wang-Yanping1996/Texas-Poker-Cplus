#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_TexasPokerNew.h"
#include "TexasPokerSingleUi.h"

class TexasPokerNew : public QMainWindow
{
    Q_OBJECT

public:
    TexasPokerNew(QWidget *parent = Q_NULLPTR);
	Ui::TexasPokerNewClass getUi()const { return this->ui; };
private:
    Ui::TexasPokerNewClass ui;
};
