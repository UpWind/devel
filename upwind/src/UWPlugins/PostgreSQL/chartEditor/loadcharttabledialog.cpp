/**
 * @file loadcharttabledialog.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "loadcharttabledialog.h"
#include "ui_loadcharttabledialog.h"

LoadChartTableDialog::LoadChartTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadChartTableDialog)
{
        ui->setupUi(this);
        connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
        connect(ui->browseFileButton, SIGNAL(clicked()), this, SLOT(onBrowseXMLFileClicked()));
}

LoadChartTableDialog::~LoadChartTableDialog()
{
    delete ui;
}

void LoadChartTableDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoadChartTableDialog::setDBTableName(QString name) {
        ui->DBTableNameLineEdit->setText(name);
}

void LoadChartTableDialog::setXMLFileName(QString name) {
        ui->fileNameLineEdit->setText(name);
}

QString LoadChartTableDialog::getDBTableName() {
        return ui->DBTableNameLineEdit->text();
}

QString LoadChartTableDialog::getXMLFileName() {
        return ui->fileNameLineEdit->text();
}

void LoadChartTableDialog::onBrowseXMLFileClicked() {
        QString fileName = QFileDialog::getOpenFileName(this, "", "", tr("XML file (*.xml);;Any file (*)"));
        if (!fileName.isEmpty()) {
                ui->fileNameLineEdit->setText(fileName);
        }
}

