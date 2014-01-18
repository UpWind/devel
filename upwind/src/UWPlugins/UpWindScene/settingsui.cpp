#include "settingsui.h"
#include "ui_settingsui.h"

#include <QCheckBox>

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::setupSettings(Settings *s){
    settings = s;
    //QCheckBox* box1 = qFindChild<QCheckBox*>(this, "checkBox");
    QCheckBox* box1 = this->findChild<QCheckBox*>("checkBox");
    //QCheckBox* box2 = qFindChild<QCheckBox*>(this, "checkBox_2");
    QCheckBox* box2 = this->findChild<QCheckBox*>("checkBox_2");

    if(s->getSetting("Something") == "true")
        box1->setChecked(true);
    else
        box1->setChecked(false);

    if(s->getSetting("SomethingElse") == "true")
        box2->setChecked(true);
    else
        box2->setChecked(false);
}

void SettingsUI::updateSettings(){
    //QCheckBox* box1 = qFindChild<QCheckBox*>(this, "checkBox");
    QCheckBox* box1 = this->findChild<QCheckBox*>("checkBox");
    //QCheckBox* box2 = qFindChild<QCheckBox*>(this, "checkBox_2");
    QCheckBox* box2 = this->findChild<QCheckBox*>("checkBox_2");

    if(box1->isChecked())
        settings->setSetting("Something", "true");
    else
        settings->setSetting("Something", "false");


    if(box2->isChecked())
        settings->setSetting("SomethingElse", "true");
    else
        settings->setSetting("SomethingElse", "false");

    settings->saveSettings();
}

void SettingsUI::on_checkBox_clicked(){
    updateSettings();
}

void SettingsUI::on_checkBox_2_clicked(){
    updateSettings();
}
