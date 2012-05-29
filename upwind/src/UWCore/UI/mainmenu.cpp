#include "mainmenu.h"
#include "uwcore.h"

MainMenu::MainMenu(QWidget *parent){
    ui.setupUi(this);
    this->setParent(parent);
    logo = new QLabel(this);
    logo->setObjectName(QString::fromUtf8("logo"));
    logo->setPixmap(QPixmap(QString::fromUtf8(":poster")));
    logo->setScaledContents(true);
    ui.gridLayout->addWidget(logo, 0, 0, 3, 3);
    logo->lower();

    connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exit()));
    connect(ui.dbButton, SIGNAL(clicked()), this, SLOT(db()));
    connect(ui.settingsButton, SIGNAL(clicked()), this, SLOT(settings()));
}

MainMenu::~MainMenu(){
    delete logo;
}

void MainMenu::about(){
    QString pluginNames = "";
    QSet<UWPluginInterface *> plugList = UWCore::getInstance()->getPluginManager()->getLoadedPlugins();
    foreach(UWPluginInterface* upi, plugList){
        pluginNames += upi->getName() + "\n";
    }
    QMessageBox::about(this,"About UpWind",
                "UpWind Project:  Plugin based architecture.\n\nLoaded plugins:\n" + pluginNames);
}

void MainMenu::exit(){
    emit close();
}

void MainMenu::db(){
    this->hide();
    emit showChart();
}

void MainMenu::settings(){
    this->hide();
    emit showSettings();
}

Ui::mainMenuDLG* MainMenu::getUI(){
    return &ui;
}
