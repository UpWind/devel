/**
 * @file loadcharttabledialog.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef LOADCHARTTABLEDIALOG_H
#define LOADCHARTTABLEDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QFileDialog>

namespace Ui {
    class LoadChartTableDialog;
}

class LoadChartTableDialog : public QDialog {
    Q_OBJECT
public:
    LoadChartTableDialog(QWidget *parent = 0);
    ~LoadChartTableDialog();
        void setDBTableName(QString name);
        void setXMLFileName(QString name);
        QString getDBTableName();
        QString getXMLFileName();

public slots:
        void onBrowseXMLFileClicked();

protected:
    void changeEvent(QEvent *e);

private:
        Ui::LoadChartTableDialog *ui;
};

#endif // LOADCHARTTABLEDIALOG_H
