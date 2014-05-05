#include "settingsui.h"
#include "ui_settingsui.h"
#include "ogrsf_frmts.h"

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
  , initializing(false)
{
    ui->setupUi(this);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    connect(ui->pushButton_apply, SIGNAL(clicked()),this, SLOT(save()));
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::save(){
    qDebug() << Q_FUNC_INFO;
//    updateSettings();
    getChartNames();
//    updateSettings();
}

void SettingsUI::getChartNames(){
    if(ui->comboBox_databases->count() != 0)
        ui->comboBox_databases->clear();

    QString driver = "PG:";
    driver.append("dbname=");
    driver.append("chart26");
    driver.append(" user=");
    driver.append(pgchart->getConUser());
    driver.append(" password=");
    driver.append(pgchart->getConPass());
    driver.append(" port=");
    driver.append(pgchart->getConPort());
    driver.append(" host=");
    driver.append(pgchart->getConHost());

    OGRDataSource *dataSource;
    OGRRegisterAll();
    dataSource = OGRSFDriverRegistrar::Open(driver.toLatin1(), TRUE);
    if(!dataSource){
        qDebug() << "Open failed.";
        OGRDataSource::DestroyDataSource(dataSource);

    }
    else{
        //This piece of code is supposed to retrieve all the Postgre
        //databases in the system

        QString sqlQuery;
        QList<QString> names;
        const char *dialect = "";
        OGRGeometry *spatialFilter = NULL;
        OGRLayer *databaseNames;
        OGRFeature *feat;

        sqlQuery = "SELECT datname FROM pg_database";

        databaseNames = dataSource->ExecuteSQL(sqlQuery.toLatin1(), spatialFilter, dialect);

        for(int i = 0; i < databaseNames->GetFeatureCount(); i++){
            feat = databaseNames->GetFeature(i);
            if(feat){
                QString dbName = (QString)feat->GetFieldAsString(0);
                if(testDBType(dbName))
                    names.append(dbName);
            }
        }
        ui->comboBox_databases->addItems(names);

        OGRDataSource::DestroyDataSource(dataSource);
    }
    updateSettings();
}

void SettingsUI::setupSettings(Settings *s){
    settings = s;

    initializing = true;

    ui->lineEdit_user->setText(s->getSetting("User"));
    ui->lineEdit_password->setText(s->getSetting("Password"));
    ui->lineEdit_port->setText(s->getSetting("Port"));
    ui->lineEdit_host->setText(s->getSetting("Host"));
    ui->level_edit->setText(s->getSetting(("DetailLevel")));

    ui->comboBox_databases->addItem(s->getSetting("DBName"));

    pgchart->initialize(s->getSetting(("Host")),
                        s->getSetting(("Port")).toInt(),
                        s->getSetting(("DBName")),
                        s->getSetting(("User")),
                        s->getSetting(("Password")),
                        s->getSetting(("DetailLevel")).toInt());

    initializing = false;
}

void SettingsUI::updateSettings(){
    if (initializing)
        return;

    settings->setSetting("User", ui->lineEdit_user->text());
    settings->setSetting("Password", ui->lineEdit_password->text());
    settings->setSetting("Port", ui->lineEdit_port->text());
    settings->setSetting("Host", ui->lineEdit_host->text());
    settings->setSetting("DBName", ui->comboBox_databases->currentText());
    settings->setSetting("DetailLevel", ui->level_edit->text());

    settings->saveSettings();

    pgchart->setConName(ui->comboBox_databases->currentText());
    pgchart->setConHost(ui->lineEdit_host->text());
    pgchart->setConPass(ui->lineEdit_password->text());
    pgchart->setConPort((ui->lineEdit_port->text()).toInt());
    pgchart->setConUser(ui->lineEdit_user->text());
    pgchart->setDetailLevel(ui->level_edit->text().toInt());
}


void SettingsUI::setPGChart(PostgreChartProvider * pgcp){
    pgchart = pgcp;
}

/** Checks if the DB is correct so it can be really used by the
  * Postgre phart provider, it's used to make the list of DBs
  * in the system.
  */
bool SettingsUI::testDBType(QString databaseName){
    QString driver = "PG:";
    driver.append("dbname=");
    driver.append(databaseName);
    driver.append(" user=");
    driver.append(pgchart->getConUser());
    driver.append(" password=");
    driver.append(pgchart->getConPass());
    driver.append(" port=");
    driver.append(pgchart->getConPort());
    driver.append(" host=");
    driver.append(pgchart->getConHost());

    QString query, layername;
    const char *dialect = "";
    OGRDataSource *dSource;
    OGRLayer *testlayer, *layer;
    OGRFeature *feat;
    OGRGeometry *spatialFilter = NULL;
    int count;
    bool valid = false;

    OGRRegisterAll();

    dSource = OGRSFDriverRegistrar::Open(driver.toLatin1(), TRUE);
    if(dSource == NULL){
        qDebug() << "Open failed.";
        OGRDataSource::DestroyDataSource(dSource);
        return false;
    }
    else{
        query = "select table_name from information_schema.tables where table_schema='public' and table_type='BASE TABLE'";
        testlayer = dSource->ExecuteSQL(query.toLatin1(), spatialFilter, dialect);

        if(testlayer){
            for(int i = 0; i < testlayer->GetFeatureCount() && !valid; i++){
                feat = testlayer->GetFeature(i);
                if(feat){
                    layername = (QString)feat->GetFieldAsString(0);
                    query = "SELECT wkb_geometry FROM ";
                    query.append(layername);
                    layer = dSource->ExecuteSQL(query.toLatin1(), spatialFilter, dialect);
                    if(layer){
                        count = layer->GetFeatureCount(true);
                        valid = true;
                    }
                }
            }
        }
    }
    OGRDataSource::DestroyDataSource(dSource);

    return valid;
}

void SettingsUI::on_level_edit_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok);
    if (ok) {
        updateSettings();
    }
}

void SettingsUI::on_comboBox_databases_currentIndexChanged(int index)
{
    updateSettings();
}
