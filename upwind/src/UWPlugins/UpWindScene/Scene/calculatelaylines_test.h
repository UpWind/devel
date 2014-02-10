#ifndef BM_CALCULATELAYLINES_H
#define BM_CALCULATELAYLINES_H

#include <QObject>

class bm_calculateLaylines : public QObject
{
    Q_OBJECT

static QVector<QPointF> longTermRoute;

private slots:
    void test_data();
    void test();


};

#endif // BM_CALCULATELAYLINES_H
