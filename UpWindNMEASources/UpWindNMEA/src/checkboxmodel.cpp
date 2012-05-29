#include "checkboxmodel.h"

#include "environment.h"

#include <QPixmap>
#include <QAbstractTableModel>

CheckBoxModel::CheckBoxModel(QObject *parent) :
    QAbstractTableModel(parent),
    greenButton(QPixmap(":/images/greenbutton.png")),
    redButton(QPixmap(":/images/redbutton.png")) {

}

void CheckBoxModel::addString(const QString &string){
    QPair<QString,bool> dat(string,true);
    modelData.append(dat);
}

int CheckBoxModel::columnCount ( const QModelIndex & parent ) const{
    return 2;
}

QVariant CheckBoxModel::data ( const QModelIndex & index, int role) const{
    if(role == Qt::DisplayRole){
        if(index.column() == 1)
            return QVariant(modelData.at(index.row()).first);
        else
            return QVariant();
    }
    else if(index.column() == 0 && role == Qt::DecorationRole) {
        return modelData.at(index.row()).second ? greenButton : redButton;
    }
    else
        return QVariant();
}

QString CheckBoxModel::getString(const QModelIndex & index){
    return modelData.at(index.row()).first;
}

QVariant CheckBoxModel::headerData ( int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        if(section == 0){
            return QVariant(QString("Show"));
        } else {
            return QVariant(QString("Plugin"));
        }
    }
    else
        return QVariant();
}

bool CheckBoxModel::isChecked(const QModelIndex & index){
    return modelData.at(index.row()).second;
}

bool CheckBoxModel::isChecked(const int row){
    return modelData.at(row).second;
}

int CheckBoxModel::rowCount ( const QModelIndex & parent) const{
    return modelData.count();
}

void CheckBoxModel::toggleCheck(const QModelIndex & index){
    QPair<QString,bool> dat = modelData.at(index.row());
    dat.second = !dat.second;
    modelData.replace(index.row(), dat);
    emit dataChanged(index,index);
}
