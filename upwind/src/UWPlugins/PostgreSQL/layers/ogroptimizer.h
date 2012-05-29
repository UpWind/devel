#ifndef OGROPTIMIZER_H
#define OGROPTIMIZER_H

#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "ogr_srs_api.h"
#include "ogr_geometry.h"
#include <QPolygonF>
#include <QObject>
#include <QVector>
#include <QThread>
#include "libpq-fe.h"


class LayersSaver;
/** implements the Peucker-Douglas optimization to a OGRLayer **/
class OGROptimizer : public QThread
{
    Q_OBJECT

    public:
    OGROptimizer();
    /** method where the thread execution starts **/
    void run();

    /** sets the state of the object, must be called before calling run()/start()
        @param points points/polygons to optimize
        @param results contains the optimized polygons
        @param level the "epsilon" value **/
    void optimize(OGRLayer* layer, OGRLayer* original_layer, qreal level, LayersSaver* callback);
    void altOptimize( QString &layerId, QString &query, qreal &level, LayersSaver* callback);

    private:
    /** optimizes the polygons, calls itself recursively
        @param points points/polygon to optimize
        @param level the "epsilon" value
        @returns optimized polygon **/
    QPolygonF* RDPOptimize(QPolygonF const points, qreal level);

    void optimizeOGRLayer();
    void alternativeOptimizeLayer();
     /** calculates a points distance from a linesegment
        @param point point
        @param lineStart starting point of a segment
        @param lineEnd ending point of a segment **/
    qreal pointsDistanceFromLine(QPointF const point, QPointF const lineStart, QPointF const lineEnd);


    /** original polygons before optimization**/
    OGRLayer *layer, *original_layer;

    /** the "epsilon" value **/
    qreal level;

    /** callback to pass the results */
    LayersSaver* callback;

    QString layerId, query;
};

#endif // OGROPTIMIZER_H
