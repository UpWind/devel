#ifndef CHECKBOXMODEL_H
#define CHECKBOXMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QPair>
#include <QPixmap>

class CheckBoxModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
    /**
      * Contains the data for the model. Each string has a boolean value that marks if the checkbox is checked or not (on/off)
      */
    QVector<QPair<QString, bool> > modelData;
public:
    /**
      * Constructor of the CheckBoxModel.
      * @param parent Parent of the model.
      */
    explicit CheckBoxModel(QObject *parent = 0);
    /**
      * Adds a new string to the model and sets the checkbox of it to true.
      * @param string String to be added
      */
    void addString(const QString &string);
    /**
      * Implemented from the abstract model. Always returns 2.
      * @param parent Not used.
      * @return Returns 2.
      */
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    /**
      * Implemented from the abstract model. Returns the data object for given index and role.
      * @param index QModelIndex containing the position needed.
      * @param role Displayrole that is used to identify what to return.
      */
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /**
      * Returns a strong with the given index.
      * @param index QModelIndex used to identify the data. Set the row parameter of the index properly.
      * @return Returns the string from the row that is set in the index.
      */
    QString getString(const QModelIndex & index);
    /**
      * Implemented from the abstract model. Used to get the header data objects from the model.
      * @param section Section whose header is being read. Placement of section depends of orientation.
      * @param orientation Orientation of the header data. Vertical or horizontal.
      * @param role Displayrole of the header cell.
      * @return Returns the header object.
      */
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /**
      * @param index Index containing the row which is tested.
      * @return Returns true if the given row is checked, otherwise false.
      */
    bool isChecked(const QModelIndex & index);
    /**
      * @param row Row which is to be tested.
      * @return Returns true if the given row is checked, otherwise false.
      */
    bool isChecked(const int row);
    /**
      * @param parent Parent is not used.
      * @return Returns the number of items (rows) in the model.
      */
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    /**
      * Toggles the checked value of row given by index to on and off.
      * @param index Index containing the row that is going to be toggled.
      */
    void toggleCheck(const QModelIndex & index);
signals:

public slots:

private:
    QPixmap greenButton;
    QPixmap redButton;
};

#endif // CHECKBOXMODEL_H
