#include "plugindelegate.h"
#include <QPainter>
#include <QDebug>
#include "plugincontroller.h"

void PluginDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect smallerRect = option.rect.adjusted(2, 2, -2, -2);
    QLinearGradient gradient(0.0, 0.0, 0.0, 1.0);
    gradient.setColorAt(0.0, QColor(120, 120, 120));
    gradient.setColorAt(0.05, QColor(150, 150, 150));
    gradient.setColorAt(0.45, QColor(120, 120, 120));
    gradient.setColorAt(0.50, QColor(80, 80, 80));
    gradient.setColorAt(0.51, QColor(50, 50, 50));
    gradient.setColorAt(1.0, QColor(50, 50, 50));

    //if(qVariantCanConvert<QString>(index.data(Qt::DisplayRole))) {
      //  QString pluginName = qVariantValue<QString>(index.data(Qt::DisplayRole));

    if(index.data(Qt::DisplayRole).canConvert(QMetaType::QString)) {
          QString pluginName = index.data(Qt::DisplayRole).value<QString>();
        QPen pluginTextColor(QColor(255, 255, 255));

        //Quick and ugly hack to get plugin widget visibility
        if(!const_cast<PluginController *>(pluginController)->getPlugin(pluginName)->getWidget()->isHidden()) {
            pluginTextColor = QColor(125, 255, 125);
        }

        if(option.state & QStyle::State_MouseOver) {

            painter->save();

            painter->setPen(QPen(QColor(255, 255, 255), 0.5));
            painter->setBrush(QBrush(QColor(165, 165, 165)));
            painter->drawRoundedRect(smallerRect, 4, 4, Qt::AbsoluteSize);

            painter->setPen(pluginTextColor);
            painter->translate(7, 7);
            painter->drawText(option.rect, pluginName);

            painter->restore();
        }

        else if(option.state & QStyle::State_Enabled) {

            painter->save();

            painter->setPen(QPen(QColor(75, 75, 75), 0.5));
            painter->setBrush(QBrush(QColor(95, 95, 95)));
            painter->drawRoundedRect(smallerRect, 4, 4, Qt::AbsoluteSize);

            painter->setPen(pluginTextColor);
            painter->translate(7, 7);
            painter->drawText(option.rect, pluginName);

            painter->restore();
        }
        else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }


    //else if(qVariantCanConvert<QPixmap>(index.data(Qt::DecorationRole))) {
    //    QPixmap button = qVariantValue<QPixmap>(index.data(Qt::DecorationRole));
    else if(index.data(Qt::DecorationRole).canConvert(QMetaType::QPixmap)) {
        QPixmap button = index.data(Qt::DecorationRole).value<QPixmap>();
        if(option.state & QStyle::State_MouseOver) {

            painter->save();

            painter->setPen(QPen(QColor(255, 255, 255), 0.5));
            painter->setBrush(QBrush(QColor(165, 165, 165)));
            painter->drawRoundedRect(smallerRect, 4, 4, Qt::AbsoluteSize);

            painter->restore();
        }

        else if(option.state & QStyle::State_Enabled) {

            painter->save();

            painter->setPen(QPen(QColor(75, 75, 75), 0.5));
            painter->setBrush(QBrush(QColor(95, 95, 95)));
            painter->drawRoundedRect(smallerRect, 4, 4, Qt::AbsoluteSize);

            painter->restore();
        }

        painter->save();
        painter->setPen(QColor(225, 225, 225));
        painter->translate(34, 7);
        painter->drawText(option.rect, "Show output");
        painter->restore();

        QStyledItemDelegate::paint(painter, option, index);

    }
}

QSize PluginDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    //if(qVariantCanConvert<QString>(index.data(Qt::DisplayRole))) {
    if (index.data(Qt::DisplayRole).canConvert(QMetaType::QString)) {
        return QSize(50, 250);
    }
    else {
        return QSize(50, 10);
    }
}
