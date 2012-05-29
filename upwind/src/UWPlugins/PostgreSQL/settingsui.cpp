#include "settingsui.h"
#include "ui_settingsui.h"
#include "pgdatabase.h"
#include "ogrsf_frmts.h"

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
    connect(ui->pushButton_Save,SIGNAL( clicked() ),this,SLOT( save() ) );
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getChartNames()));
    connect(ui->pushButton_chartTableEditor, SIGNAL(clicked()), this, SLOT(openChartEditor()));
    connect(ui->pushButton_browseChartTableXMLFile, SIGNAL(clicked()), this, SLOT(browseChartTableXMLFile()));
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::save(){
    updateSettings();
}
void SettingsUI::setChart(PGDataBase* c){
    chart = c;
}

void SettingsUI::setupSettings(Settings *s){
    settings = s;

    ui->userpath->setText(s->getSetting("User"));
    ui->passwordpath->setText(s->getSetting("Password"));
    ui->portpath->setText(s->getSetting("Port"));
    ui->hostpath->setText(s->getSetting("Host"));

    if(s->getSetting("Driver") == "MySQL")
        ui->driverCB->setCurrentIndex(0);
    else
        ui->driverCB->setCurrentIndex(1);

    ui->comboBox->addItem(s->getSetting("DBName"));
    ui->lineEdit_chartTableXMLFile->setText(s->getSetting("XML"));


    chart->setDBDriver(s->getSetting(("Driver")));
    chart->setDBHost(s->getSetting(("Host")));
    chart->setDBPass(s->getSetting(("Password")));
    chart->setDBPort(s->getSetting(("Port")));
    chart->setDBUser(s->getSetting(("User")));
}

void SettingsUI::updateSettings(){
    settings->setSetting("User", ui->userpath->text());
    settings->setSetting("Password", ui->passwordpath->text());
    settings->setSetting("Port", ui->portpath->text());
    settings->setSetting("Host", ui->hostpath->text());
    settings->setSetting("Driver", QString::fromStdString(ui->driverCB->currentText().toStdString()));
    settings->setSetting("DBName", ui->comboBox->currentText());
    settings->setSetting("XML", ui->lineEdit_chartTableXMLFile->text());

    settings->saveSettings();

    chart->setDBDriver(QString::fromStdString(ui->driverCB->currentText().toStdString()));
    chart->setDBHost(ui->hostpath->text());
    chart->setDBPass(ui->passwordpath->text());
    chart->setDBPort(ui->portpath->text());
    chart->setDBUser(ui->userpath->text());
}

//TODO : From here should be done in PGDataBase, not here.
void SettingsUI::getChartNames(){
    if(ui->comboBox->count() != 0)
        ui->comboBox->clear();

    if(chart->getDBDriver() == "PostgreSQL"){
        QString driver = "PG:";
        driver.append("dbname=chart54");
        driver.append(" user=");
        driver.append(chart->getDBUser());
        driver.append(" password=");
        driver.append(chart->getDBPass());
        driver.append(" port=");
        driver.append(chart->getDBPort());
        driver.append(" host=");
        driver.append(chart->getDBHost());

        OGRDataSource *dataSource;
        OGRRegisterAll();
        dataSource = OGRSFDriverRegistrar::Open(driver.toAscii(), TRUE);
        if(dataSource == NULL){
            qDebug() << "Open failed.";
            OGRDataSource::DestroyDataSource(dataSource);
        } else{
            QString sqlQuery;

            const char *dialect = "";
            OGRGeometry *spatialFilter = NULL;

            sqlQuery = "SELECT datname FROM pg_database";

            OGRLayer * databaseNames = dataSource->ExecuteSQL(sqlQuery.toAscii(), spatialFilter,dialect);
            qDebug() << "Layer count: " << databaseNames->GetFeatureCount();

            QList<QString> names;

            for(int i = 0; i < databaseNames->GetFeatureCount(); i++){
                OGRFeature* feat = databaseNames->GetFeature(i);
                if(feat){
                    QString dbName = (QString)feat->GetFieldAsString(0);
                    qDebug() << "Database name: " << dbName;

                    if(testDBType(dbName))
                        names.append(dbName);
                }
            }
            ui->comboBox->addItems(names);

            OGRDataSource::DestroyDataSource(dataSource);
        }
    }
    updateSettings();
}

void SettingsUI::browseChartTableXMLFile(){
    QString fileName = QFileDialog::getOpenFileName(this, "", "", tr("XML file (*.xml);;All files (*)"));
    if(!fileName.isEmpty())
        ui->lineEdit_chartTableXMLFile->setText(fileName);
}

void SettingsUI::openChartEditor(){
    SettingsManager* settingsManager;
    settingsManager=SettingsManager::getInstance();

    chart->setDBName(ui->comboBox->currentText());

    settingsManager->setDBDriver(chart->getDBDriver());
    settingsManager->setDBHost(chart->getDBHost());
    settingsManager->setDBName(chart->getDBName());
    settingsManager->setDBPass(chart->getDBPass());
    settingsManager->setDBPort(chart->getDBPort());
    settingsManager->setDBUser(chart->getDBUser());

    settingsManager->setChartTableXMLFile(ui->lineEdit_chartTableXMLFile->text());

    chartEditorWindow = new ChartEditorWindow();
    // editor is QMainWindow -- make it a modal dialog so it doesn't start to haunt around
    chartEditorWindow->setWindowFlags(Qt::Dialog);
    chartEditorWindow->setWindowModality(Qt::WindowModal);
    chartEditorWindow->showFullScreen();

    if(!ui->lineEdit_chartTableXMLFile->text().isEmpty())
        chartEditorWindow->showLoadChartTableDialog();

    updateSettings();
}

bool SettingsUI::testDBType(QString databaseName){
    QString driver = "PG:";
    driver.append("dbname=");
    driver.append(databaseName);
    driver.append(" user=");
    driver.append(chart->getDBUser());
    driver.append(" password=");
    driver.append(chart->getDBPass());
    driver.append(" port=");
    driver.append(chart->getDBPort());
    driver.append(" host=");
    driver.append(chart->getDBHost());

    OGRDataSource *dSource;
    OGRRegisterAll();
    dSource = OGRSFDriverRegistrar::Open(driver.toAscii(), TRUE);
    if(dSource == NULL){
        qDebug() << "Open failed.";
        OGRDataSource::DestroyDataSource(dSource);
        return false;
    }

    QString sqlQuery;

    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;

    sqlQuery = "select table_name from information_schema.tables where table_schema='public' and table_type='BASE TABLE'";

    OGRLayer * testlayer = dSource->ExecuteSQL(sqlQuery.toAscii(), spatialFilter, dialect);

    if(testlayer){
        for(int i = 0; i < testlayer->GetFeatureCount(); i++){
            OGRFeature* feat = testlayer->GetFeature(i);
            if(feat){
                QString layername = (QString)feat->GetFieldAsString(0);
                QString query = "SELECT wkb_geometry FROM ";
                query.append(layername);
                OGRLayer * layer = dSource->ExecuteSQL(query.toAscii(), spatialFilter, dialect);
                if(layer){
                    int count = layer->GetFeatureCount(true);
                    qDebug() << count;
                    OGRDataSource::DestroyDataSource(dSource);
                    return true;
                }
            }
        }
    }
    OGRDataSource::DestroyDataSource(dSource);

    return false;
}
