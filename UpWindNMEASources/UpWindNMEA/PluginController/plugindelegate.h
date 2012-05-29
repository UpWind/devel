#ifndef PLUGINDELEGATE_H
#define PLUGINDELEGATE_H

#include <QStyledItemDelegate>

class PluginController;

/**
  * The PluginDelegate is used to customize the item rendering of the
  * QTableView that displays the plugin names and text output filters.
  */
class PluginDelegate : public QStyledItemDelegate {

    Q_OBJECT

public:
    explicit PluginDelegate(const PluginController &controller, QObject *parent = 0):
            QStyledItemDelegate(parent), pluginController(&controller) {}
    /**
      * Reimplemented from QStyledItemDelegate::paint()
      * Renders the delegate using a custom paint style.
      * @param painter The QPainter to paint with
      * @param option The view parameters of the item
      * @param index The index to the model
      */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /**
      * Returns the pixel size of the item to be rendered by the delegate.
      * @param option The view parameters of the item
      * @param index The index to the model
      */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    const PluginController *pluginController;

signals:

public slots:

};

#endif // PLUGINDELEGATE_H
