#include "%ProjectName:l%.h"
#include "ui_%ProjectName:l%.h"
#include "settingsmanager.h"

%ProjectName%::%ProjectName%():
        ui(new Ui::%ProjectName%)
{
    this->pluginName = "NMEA %ProjectName%";
    this->produceIDs = "???";
    this->parseIDs = "???";

    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName);
}

%ProjectName%::~%ProjectName%(){
    delete ui;
    //delete allocated variables
}

void %ProjectName%::CreateGUI(){
    ui->setupUi(plugin_widget);
}

void %ProjectName%::parseNMEAString( const QString & text){
    //Parse the text
}

void %ProjectName%::updateNMEAString(){
    //Create and emit a string
}

/*
If you need more info, check out the clock plugin. It's a simple NMEA plugin that contains a few extra comments
for new developers.
 */
