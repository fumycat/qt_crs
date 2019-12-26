#include "configscore.h"
#include "ui_configscore.h"

ConfigScore::ConfigScore(QStringList p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigScore)
{
    ui->setupUi(this);
}

ConfigScore::~ConfigScore()
{
    delete ui;
}
