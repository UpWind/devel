#include "nmeacompass.h"
#include "ui_compasswidget.h"

//Sets plugin name, messageID, timerDelay
NMEACompass::NMEACompass(): ui(new Ui::CompassWidget), heading(0)
{
    this->pluginName = "NMEA Compass";
    this->produceIDs = "HDG";
    this->parseIDs = "HDG";

    CreateGUI();
    setTimerDelay(updateDelay);
    plugin_widget->setWindowTitle(pluginName);
    //start();
}

NMEACompass::~NMEACompass(){
    delete ui;
}

int NMEACompass::getHeading(){
    return heading;
}

void NMEACompass::setHeading(int heading){

    this->heading = heading;
    ui->lineEdit->setText(QString::number(heading));
    (heading>=180 ? heading -=180 : heading += 180);
    ui->dial->setValue(heading);
}

void NMEACompass::CreateGUI(){
    ui->setupUi(plugin_widget);
    //ui->dial->setStyle(&winStyle);

    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(updateLineEdit(int)));
    connect(ui->orientationButton,SIGNAL(clicked()),this,SLOT(updateDial()));
    connect(ui->lineEdit,SIGNAL(returnPressed()), this, SLOT(updateDial()));

}

void NMEACompass::updateDial(){
    bool isNum;
    int dialValue = ui->lineEdit->text().toInt(&isNum);
    if (!isNum){
        dialValue = 0;
        ui->lineEdit->setText("0");
    }
    if(dialValue>359 || dialValue < 0){
        dialValue = 0;
        ui->lineEdit->setText("0");
    }

    // TODO: wtf
    if(dialValue>=180 ? dialValue -=180 : dialValue += 180);
    heading = dialValue;
    ui->dial->setValue(dialValue);

}


void NMEACompass::updateLineEdit(int value){
    int lineValue = value;

    // TODO: wtf
    if(lineValue>179 ? lineValue -=180 : lineValue += 180);
    heading = lineValue;
    ui->lineEdit->setText(QString::number(lineValue));
}

void NMEACompass::updateNMEAString(){
/** $HCHDG,101.1,,,7.1,W*3C

    where:
         HCHDG    Magnetic heading, deviation, variation
         101.1    heading
         ,,       deviation (no data)
         7.1,W    variation
**/

    QString newString = "$IIHDG,"+QString::number(heading)+",,,7.1,W";
    newString = newString + "*" + generateChecksum(newString);
    //if(newString != NMEAString){
        NMEAString = newString;
        emit newNMEAString(NMEAString);
    //}
}

void NMEACompass::parseNMEAString( const QString & text){  
    //$HCHDG,101.1,,,7.1,W*3C
    QStringList strList = text.split(",");
    ui->lineEdit->setText(strList.at(1));
    int dialValue = ((QString)strList.at(1)).toInt();
    heading = dialValue;

    // TODO: wtf
    if(dialValue>=180 ? dialValue -=180 : dialValue += 180);
    ui->dial->setValue(dialValue);
}

bool NMEACompass::supports(const QString &m_ID, bool excludeProduced){
    return CorePlugin::supports(m_ID, false);
}

void NMEACompass::setTimerDelay(int tDelay){
    CorePlugin::setTimerDelay(updateDelay);
}
