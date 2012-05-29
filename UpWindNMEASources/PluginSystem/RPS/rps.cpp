#include "rps.h"
#include "ui_widget.h"
#include "settingsmanager.h"

RPS::RPS():
        ui(new Ui::Widget), forceToBeReconWith(1), wins(0), draws(0), loses(0)
{
    this->pluginName = "NMEA RPS";
    this->produceIDs = "???";
    this->parseIDs = "MWD";

    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName);
}

RPS::~RPS(){
    delete ui;
    //delete allocated variables
}

void RPS::CreateGUI(){
    ui->setupUi(plugin_widget);
    ui->radioButton_r->setChecked(true);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(calculateVictory()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearStatistics()));
}

void RPS::parseNMEAString( const QString & text){
    //Parse the text
    if(this->parseMessageID(text) == "MWD"){
        QStringList pList = text.split(",");
        QString angle = pList.at(3).split(".").at(0);
        forceToBeReconWith = angle.toInt();

    }
}

void RPS::updateNMEAString(){
    //Create and emit a string
}

void RPS::calculateVictory(){
    int ai = qrand()*forceToBeReconWith;
    int result = ai%3;

    int player = 0;
    if(ui->radioButton_r->isChecked())
        player = 0;
    if(ui->radioButton_p->isChecked())
        player = 1;
    if(ui->radioButton_s->isChecked())
       player = 2;

    switch(player){
    case 0:
        if(result == 0){
             ui->statusLabel->setText("Match tied");
             ui->label_3->setText("Rock");
             draws++;
             ui->drawLabel->setNum(draws);
        }
        if(result == 1){
             ui->statusLabel->setText("EPIC FAIL");
             ui->label_3->setText("Paper");
             loses++;
             ui->loseLabel->setNum(loses);
        }
        if(result == 2){
             ui->statusLabel->setText("YOU WIN!!!!");
             ui->label_3->setText("Scissors");
             wins++;
             ui->winLabel->setNum(wins);
        }
        break;
    case 1:
        if(result == 0){
             ui->statusLabel->setText("YOU WIN!!!!");
             ui->label_3->setText("Rock");
             wins++;
             ui->winLabel->setNum(wins);
        }
        if(result == 1){
             ui->statusLabel->setText("Match tied");
             ui->label_3->setText("Paper");
             draws++;
             ui->drawLabel->setNum(draws);
        }
        if(result == 2){
             ui->statusLabel->setText("EPIC FAIL");
             ui->label_3->setText("Scissors");
             loses++;
             ui->loseLabel->setNum(loses);
        }
        break;
     case 2:
        if(result == 0){
             ui->statusLabel->setText("EPIC FAIL");
             ui->label_3->setText("Rock");
             loses++;
             ui->loseLabel->setNum(loses);
        }
        if(result == 1){
             ui->statusLabel->setText("YOU WIN!!!!");
             ui->label_3->setText("Paper");
             wins++;
             ui->winLabel->setNum(wins);
        }
        if(result == 2){
             ui->statusLabel->setText("Match tied");
             ui->label_3->setText("Scissors");
             draws++;
             ui->drawLabel->setNum(draws);
        }
        break;
     default:
        ui->statusLabel->setText("Nothing happened...");

    }

}

void RPS::saveTo(SettingsManager &sm){
    CorePlugin::saveTo(sm);
    sm.saveProperty("statusline", ui->statusLabel->text());
    if(ui->radioButton_r->isChecked())
        sm.saveProperty("checked", "R");
    if(ui->radioButton_p->isChecked())
        sm.saveProperty("checked", "P");
    if(ui->radioButton_s->isChecked())
        sm.saveProperty("checked", "S");

    sm.saveProperty("ailine", ui->label_3->text());
    sm.saveProperty("wins", QString::number(wins));
    sm.saveProperty("draws", QString::number(draws));
    sm.saveProperty("loses", QString::number(loses));

}

QString RPS::loadFrom(SettingsManager &sm){
    QString fileVer = CorePlugin::loadFrom(sm);
    ui->statusLabel->setText(sm.loadProperty("statusline", "EPIC FAIL!"));
    QString check = sm.loadProperty("checked", "R");
    if(check =="R")
        ui->radioButton_r->setChecked(true);
    if(check =="P")
        ui->radioButton_p->setChecked(true);
    if(check=="S")
        ui->radioButton_s->setChecked(true);
    ui->label_3->setText(sm.loadProperty("ailine", "I choose nothing!"));
    wins = sm.loadProperty("wins", "0").toInt();
    draws = sm.loadProperty("draws", "0").toInt();
    loses = sm.loadProperty("loses", "0").toInt();
    ui->winLabel->setNum(wins);
    ui->drawLabel->setNum(draws);
    ui->loseLabel->setNum(loses);
    return fileVer;
}

void RPS::clearStatistics(){
    wins = 0;
    draws = 0;
    loses = 0;
    ui->winLabel->setNum(wins);
    ui->drawLabel->setNum(draws);
    ui->loseLabel->setNum(loses);

}

