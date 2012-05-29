#include "ogroptimizer.h"
#include <QDebug>
#include "LayersSaver.h"
#include <QLineF>
#include <iostream>

OGROptimizer::OGROptimizer()
{
}


qreal OGROptimizer::pointsDistanceFromLine(QPointF const point, QPointF const lineStart, QPointF const lineEnd)
{
    qreal startY = lineStart.y();
    qreal startX = lineStart.x();
    qreal endY = lineEnd.y();
    qreal endX = lineEnd.x();
    qreal pointY = point.y();
    qreal pointX = point.x();

    qreal A = pointX - startX;
    qreal B = pointY - startY;
    qreal C = endX - startX;
    qreal D = endY - startY;

    //Dot product of the vectors AB and CD
    qreal dot = A * C + B * D;
    //square length
    qreal len_sq = C * C + D * D;

    //nearest point to pointX, pointY
    QPointF nearestPoint;

    //if len_sq==0, then points startX, startY == endX, endY and we don't want to divide with 0.
    if(len_sq != 0){
        //param =  "how many times apply vector CD to point startX, startY to get position of the point in segment that
        //is nearest to pointX,pointY"
        qreal param = dot / len_sq;

        //param < 0 means nearest point to the pointX, pointY is starting point of the segment
        if (param < 0){
            nearestPoint.setX(startX);
            nearestPoint.setY(startY);
        }

        //param > 0 means nearest point to the pointX, pointY is ending point of the segment
        else if (param > 1){
            nearestPoint.setX(endX);
            nearestPoint.setY(endY);
        }
        // nearest point is somewhere in segment,  startX, startY + param(CD)
        else{
            nearestPoint.setX(startX+param*C);
            nearestPoint.setY(startY+param*D);
        }
    }
    else{
        nearestPoint.setX(startX);
        nearestPoint.setY(startY);
    }
    //return the distance between 2 points
    return sqrt(pow(nearestPoint.x() - pointX, 2) + pow(nearestPoint.y() - pointY, 2));

}

void OGROptimizer::optimize(OGRLayer* layer, OGRLayer * original_layer, qreal level, LayersSaver* callback){
    this->callback = callback;
    this->layer = layer;
    this->original_layer = original_layer;
    this->level = level;
}

void OGROptimizer::altOptimize( QString &layerId, QString &query, qreal &level, LayersSaver* callback){
    this->callback = callback;
    this->layerId = layerId;
    this->query = query;
    this->level = level;
}

void OGROptimizer::run(){
    alternativeOptimizeLayer();
    //optimizeOGRLayer();
    callback->optimizerReady();
    this->exit(NULL);

}

void OGROptimizer::optimizeOGRLayer(){
    OGRFeature *feature = NULL;
    OGRFeature *original_feature = NULL;
    while((feature = layer->GetNextFeature()) != NULL && (original_feature = original_layer->GetNextFeature() ) != NULL){
        OGRGeometry* geometry = feature->GetGeometryRef();
        QPolygonF polygon;
        QPolygonF *optimizedPoly;

        //Polygons
        if(geometry && wkbFlatten(geometry->getGeometryType()) == wkbPolygon) {
            OGRGeometryH hRing;
            int iRing = 0, nRingCount = OGR_G_GetGeometryCount(geometry);
            for(iRing = 0; iRing < nRingCount; iRing++) {
                int node;
                hRing = OGR_G_GetGeometryRef( geometry, iRing );

                int vert_count = OGR_G_GetPointCount(hRing);
                for(node = vert_count - 1; node >= 0; node--) {
                    QPointF p(OGR_G_GetY( hRing, node ), OGR_G_GetX(hRing , node));
                    polygon << p;
                }

                //optimize polygon, clear the previous geometry and add new points to it
                optimizedPoly = RDPOptimize(polygon, level);
                OGR_G_Empty(hRing);

                // This should be a better way of having a more than 4 points polygon, the above method just fake points
                qreal temp_level( level);
                while (optimizedPoly->size() < 4 )
                {
                    delete optimizedPoly;
                    temp_level = temp_level / 2;
                    optimizedPoly = RDPOptimize(polygon, temp_level);
                }

                for(int i=optimizedPoly->size()-1; i >= 0; i--)
                {
                    OGR_G_SetPoint_2D(hRing,i, optimizedPoly->at(i).y(),optimizedPoly->at(i).x());

                }

                delete optimizedPoly;


                polygon.clear();
            }
        }
        //Multipoly
        else if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon ) {
            int iPoly, nShapeRing = 0;
            for( iPoly = 0; iPoly < OGR_G_GetGeometryCount( geometry );iPoly++ ) {
                OGRGeometryH hPoly, hRing;
                int iRing, nRingCount;

                hPoly = OGR_G_GetGeometryRef(geometry, iPoly );
                nRingCount = OGR_G_GetGeometryCount( hPoly );
                for( iRing = 0; iRing < nRingCount; iRing++ ) {
                    int node;
                    hRing      = OGR_G_GetGeometryRef( hPoly, iRing );
                    int vert_count = OGR_G_GetPointCount(hRing);
                    polygon.clear();
                    for( node = vert_count - 1; node >= 0; node--){
                        QPointF p(OGR_G_GetY( hRing, node ), OGR_G_GetX(hRing , node));
                        polygon << p;
                    }
                    //optimize polygon, clear the previous geometry and add new points to it
                    optimizedPoly = RDPOptimize(polygon, level);
                    OGR_G_Empty(hRing);

                    // This should be a better way of having a more than 4 points polygon, the above method just fake points
                    qreal temp_level( level);
                    while (optimizedPoly->size() < 4 ){
                        delete optimizedPoly;
                        temp_level = temp_level / 2;
                        optimizedPoly = RDPOptimize(polygon, temp_level);
                    }

                    for(int i=optimizedPoly->size()-1;i >=0; i--)
                    {
                        OGR_G_SetPoint_2D(hRing,i, optimizedPoly->at(i).y(), optimizedPoly->at(i).x());
                    }
                    delete optimizedPoly;

                }
            }
        }
        ///Lines
        else if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbLineString ) {
            // TODO: remove the C API and use only the C++ API
            //OGRLineString *ogrLine = (OGRLineString *) geometry;
            int node, count = OGR_G_GetPointCount(geometry);

            for( node = count-1; node >= 0; node-- ) {
                QPointF p(OGR_G_GetY( geometry, node ), OGR_G_GetX( geometry , node ));
                polygon << p;
            }
            //optimize polygon, clear the previous geometry and add new points to it
            optimizedPoly = RDPOptimize(polygon, level);
            geometry->empty();

            for(int i=optimizedPoly->size()-1;i >=0; i--)
            {
                OGR_G_SetPoint_2D(geometry,i, optimizedPoly->at(i).y(),optimizedPoly->at(i).x());
            }
            delete optimizedPoly;
        }
        else {
            qWarning("file error");
        }

        OGRFeatureDefn *featureDef = original_layer->GetLayerDefn();
        OGRFeatureDefn *copy_featureDef = layer->GetLayerDefn();

        // For each feature go through the fields (AKA columns of relation)
        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn(iField);

            OGRFieldType type = fieldDef->GetType();

            int copy_iField;
            for( copy_iField = 0; copy_iField < copy_featureDef->GetFieldCount(); copy_iField++ ){
                OGRFieldDefn *copy_fieldDef = copy_featureDef->GetFieldDefn(copy_iField);
                //qDebug() << "Original column: " << fieldDef->GetNameRef() << " comparing with copy column: " << copy_fieldDef->GetNameRef();
                if ( QString(copy_fieldDef->GetNameRef()) == QString(fieldDef->GetNameRef()) ) {
                    //qDebug() << "Success! copy_iField: " << copy_iField;
                    break;
                }
            }

            OGRFieldDefn *copy_fieldDef = copy_featureDef->GetFieldDefn(copy_iField);


            if(type == OFTReal) {
                feature->SetField( copy_iField, original_feature->GetFieldAsDouble( iField));
                //qDebug() << "Copy_layer_col_name: " << copy_fieldDef->GetNameRef() << "Original_layer_col_name: " <<fieldDef->GetNameRef() << " (" << fieldDef->GetFieldTypeName( type)<< ")" << " OFTReal: " << original_feature->GetFieldAsDouble( iField);
            }
            else if(type == OFTInteger) {
                feature->SetField( copy_iField, original_feature->GetFieldAsInteger( iField));
                //qDebug() << "Copy_layer_col_name: " << copy_fieldDef->GetNameRef() << "Original_layer_col_name: " << fieldDef->GetNameRef() << " (" << fieldDef->GetFieldTypeName( type)<< ")" <<  " OFTInteger: " << original_feature->GetFieldAsInteger( iField);
            } else {
                feature->SetField( copy_iField, original_feature->GetFieldAsString( iField));
                //qDebug() << "Copy_layer_col_name: " << copy_fieldDef->GetNameRef() << "Original_layer_col_name: " << fieldDef->GetNameRef() << " (" << fieldDef->GetFieldTypeName( type)<< ")" <<  " Other: " << original_feature->GetFieldAsString( iField);
            }

        }

        layer->SetFeature(feature);

        OGRFeature::DestroyFeature(feature);
        OGRFeature::DestroyFeature(original_feature);
    }
}


void OGROptimizer::alternativeOptimizeLayer()
{

    if ( callback->getSettingsManager()->getDBDriver() == "PostgreSQL" && !layerId.startsWith( "obstacles") )
    {
        QString table_name = layerId;

        layerId.truncate(  layerId.indexOf( "_le", 0, Qt::CaseInsensitive));

        // I'm sorry not to use OGR
        PGconn     *conn;
        PGresult   *res;
        QPolygonF * optimizedPoly;
        bool polyLayer = layerId.endsWith("_r");
        bool lineLayer = layerId.endsWith("_l");

        QString sdriver = "dbname=";
        sdriver.append(callback->getSettingsManager()->getDBName());
        sdriver.append(" user=");
        sdriver.append(callback->getSettingsManager()->getDBUser());
        sdriver.append(" password=");
        sdriver.append(callback->getSettingsManager()->getDBPass());
        sdriver.append(" port=");
        sdriver.append(callback->getSettingsManager()->getDBPort());
        sdriver.append(" host=");
        sdriver.append(callback->getSettingsManager()->getDBHost());


        conn = PQconnectdb( sdriver.toAscii());

        if (PQstatus(conn) != CONNECTION_OK)
        {
            qDebug() << "Connection to database failed: " << PQerrorMessage(conn);
            PQfinish( conn);
        }

        QString sql("DROP TABLE IF EXISTS ");
        sql.append( table_name);
        sql.append( ";");
        res = PQexec(conn, sql.toAscii() );
        sql.clear();
        PQclear(res);



        sql.append( "CREATE TABLE ");
        sql.append( table_name);
        sql.append( " AS (");
        sql.append( query);
        sql.append( ");");
        res = PQexec( conn, sql.toAscii());
        sql.clear();
        PQclear(res);

        sql.append("SELECT ogc_fid,ST_AsText(wkb_geometry) FROM ");
        sql.append( layerId);

        res = PQexec(conn, sql.toAscii() );
        sql.clear();

        for ( int i = 0; i < PQntuples(res); i++ ) {

            if ( lineLayer) {

                QPolygonF line = getFromWKTLinestring( PQgetvalue( res, i, 1));

                optimizedPoly = RDPOptimize( line, level);

                sql.append( "UPDATE TABLE ");
                sql.append( table_name);
                sql.append( " wkb_geometry=(");
                sql.append( setToWKTLinestring( *optimizedPoly));
                sql.append( ") WHERE ogc_fid=");
                sql.append( PQgetvalue(res, i, 0));
                sql.append( ";");
                delete optimizedPoly;



            } else if (polyLayer) {

                QVector<QPolygonF> poly = getFromWKTPolygon( PQgetvalue( res, i, 1));

                for ( int j = 0; j < poly.size(); j++ ) {

                    optimizedPoly = RDPOptimize( poly.at(j), level);

                    qreal temp_level( level);
                    while (optimizedPoly->size() < 4 )
                    {
                        delete optimizedPoly;
                        temp_level = temp_level / 2;
                        optimizedPoly = RDPOptimize( poly.at(j), temp_level);
                    }

                    // first and last point of the polygon must be the same point
                    // if not PostGIS will not accept it -> crash
                    if ( optimizedPoly->at(0) != optimizedPoly->at( optimizedPoly->size() - 1 ) ) {
                        optimizedPoly->append( optimizedPoly->at( 0));
                    }

                    poly[j] = *optimizedPoly;

                    delete optimizedPoly;
                }

                sql.append( "UPDATE ");
                sql.append( table_name);
                sql.append( " SET wkb_geometry=(");
                sql.append( setToWKTPolygon( poly));
                sql.append( ") WHERE ogc_fid=");
                sql.append(PQgetvalue( res, i, 0));
                sql.append( ";");

            }
        }
        PQclear(res);

        res = PQexec( conn, sql.toAscii());
        PQclear(res);
        PQfinish(conn);
    }

}

QPolygonF* OGROptimizer::RDPOptimize(QPolygonF const points, qreal level)
{
    qreal dmax=-1, index=0,i;
    for(i = 1; i < (points.size() - 1); i++)
    {
        qreal d = pointsDistanceFromLine( points.at(i), points.first(), points.last() );
        if(d>dmax)
        {
            dmax=d;
            index=i;
        }
    }

    QPolygonF* result = new QPolygonF;

    if(dmax>=level)
    {
        QPolygonF *result1 = RDPOptimize(static_cast<QPolygonF>(points.mid(0, index+1)), level);
        QPolygonF *result2 = RDPOptimize(static_cast<QPolygonF>(points.mid(index)), level);



        //duplicate point
        result2->pop_front();

        *result += *result1;
        *result += *result2;
        delete result1;
        delete result2;
    }
    else
    {
        result->push_front(points.first());
        result->append(points.last());
    }

    return result;
}
