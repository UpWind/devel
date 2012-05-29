#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent){
    ui.setupUi(this);
    this->setParent(parent);

    connect(ui.backSettingsButton, SIGNAL(clicked()), this, SLOT(back()));
}

SettingsWindow::~SettingsWindow(){
    //delete ui;
}

void SettingsWindow::showSettings(){
    this->show();
}

void SettingsWindow::back(){
    this->hide();
    emit goBack();
}
