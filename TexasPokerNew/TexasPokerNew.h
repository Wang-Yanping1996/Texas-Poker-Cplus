#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TexasPokerNew.h"

class TexasPokerNew : public QMainWindow
{
    Q_OBJECT

public:
    TexasPokerNew(QWidget *parent = Q_NULLPTR);

private:
    Ui::TexasPokerNewClass ui;
};
