#include "chart.h"

Chart::Chart(/*QString driver*/)
    : dataSource(0), getLayer(0) {
    OGRRegisterAll();

    debug = DEBUG;

    settingsManager = SettingsManager::getInstance();

    // Connects the names of the fields we're interested in to the layers.
    // Can later be queried by the layer name. See getFieldsForLayer(..)
    // Point layers
    QList<QString> tmp;
    tmp.append("ntxtst");
    tmp.append("OBJCLSNAME");
    fieldsForLayers.insert("ctext_p", tmp);

    tmp = QList<QString>();
    tmp.append("depth");
    fieldsForLayers.insert("signsound_p", tmp);

    tmp = QList<QString>();
    tmp.append("fi_typrock");
    fieldsForLayers.insert("rock_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMEBLDING");
    fieldsForLayers.insert("building_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPETOWMST");
    fieldsForLayers.insert("towernmast_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPEVEGATN");
    fieldsForLayers.insert("vegetation_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMEMORFAC");
    fieldsForLayers.insert("moorfacil_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPESEABED");
    fieldsForLayers.insert("natseabed_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPEOBSTRN");
    fieldsForLayers.insert("obstruct_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMEHARBOR");
    fieldsForLayers.insert("harbour_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMEMARSER");
    fieldsForLayers.insert("marservice_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPEWRECK");
    fieldsForLayers.insert("wreck_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMECHKPNT");
    fieldsForLayers.insert("checkpoint_p", tmp);

    tmp = QList<QString>();
    tmp.append("TYPELIMIT");
    fieldsForLayers.insert("limit_p", tmp);

    tmp = QList<QString>();
    tmp.append("namnavaid1");
    tmp.append("objectid");
    tmp.append("fi_typenav");
    tmp.append("ntypnavaid");
    tmp.append("fi_strcnav");
    fieldsForLayers.insert("navaid_p", tmp);

    tmp = QList<QString>();
    tmp.append("NAMEFORSTC");
    fieldsForLayers.insert("forstruct_p", tmp);

    // Line layers
    tmp  = QList<QString>();
    tmp.append("QUADEPCNT");
    tmp.append("valdco");
    fieldsForLayers.insert("depthcont_l", tmp);

    tmp  = QList<QString>();
    tmp.append("BEARING");
    fieldsForLayers.insert("navigline_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPEROAD");
    fieldsForLayers.insert("roadnrailw_l", tmp);

    tmp  = QList<QString>();
    tmp.append("NAMEBRIDGE");
    fieldsForLayers.insert("bridge_l", tmp);
    // New Line layers

    tmp  = QList<QString>();
    tmp.append("TYPESUBCBL");
    fieldsForLayers.insert("submcable_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPETRNLIN");
    fieldsForLayers.insert("transmline_l", tmp);

    tmp  = QList<QString>();
    // nothing for this layer at the moment
    fieldsForLayers.insert("closingline_l", tmp);

    tmp  = QList<QString>();
    tmp.append("SCAMAX");
    fieldsForLayers.insert("coastline_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPEFERRY");
    fieldsForLayers.insert("ferry_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPELOCDAM");
    fieldsForLayers.insert("lockndam_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPEPIPSOL");
    fieldsForLayers.insert("pipeline_l", tmp);

    tmp  = QList<QString>();
    tmp.append("NAMERIVER");
    fieldsForLayers.insert("river_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPECONSTR");
    fieldsForLayers.insert("shlnecon_l", tmp);

    tmp  = QList<QString>();
    tmp.append("BEARING");
    fieldsForLayers.insert("leadinglne_l", tmp);

    tmp  = QList<QString>();
    tmp.append("ELEVHGTCNT");
    fieldsForLayers.insert("heightcont_l", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPELIMIT");
    fieldsForLayers.insert("limit_l", tmp);

    // region layers
    tmp  = QList<QString>();
    tmp.append("TYPEGENARE");
    fieldsForLayers.insert("generarea_r", tmp);

    tmp  = QList<QString>();
    tmp.append("NAMEBUAARE");
    fieldsForLayers.insert("builtupare_r", tmp);

    tmp  = QList<QString>();
    tmp.append("NAMEBLDING");
    fieldsForLayers.insert("building_r", tmp);

    tmp  = QList<QString>();
    tmp.append("MINDEPTH");
    tmp.append("MAXDEPTH");
    tmp.append("TYPEDEPR");
    fieldsForLayers.insert("deptharea_r", tmp);
    fieldsForLayers.insert("lake_r", tmp);

    tmp  = QList<QString>();
    tmp.append("fi_oprperm");
    fieldsForLayers.insert("spoilgrnd_r", tmp);

    tmp  = QList<QString>();
    tmp.append("NAMEDOCK");
    fieldsForLayers.insert("dock_r", tmp);

    tmp  = QList<QString>();
    tmp.append("TYPELIMIT");
    fieldsForLayers.insert("limit_r", tmp);

}

Chart::~Chart() {
    OGRDataSource::DestroyDataSource( dataSource );
}

void Chart::setDriver(QString driver){
    qDebug()<<Q_FUNC_INFO;
    // destroy the possible previous data source before creating
    // new data source
    if(dataSource != 0 ) {
        OGRDataSource::DestroyDataSource(dataSource);
        dataSource = 0;
    }

    dataSource = OGRSFDriverRegistrar::Open(driver.toAscii(), TRUE);
    if(dataSource == NULL) {
        std::cerr << "Open failed.\n";
        exit(1); // something more graceful here?
    }

    getLayer = &Chart::getLayerFromDatabase;

}

void Chart::setOGRDriverQt(QString qSqlDriver) {
    QString ogrDriverStr;
    if (qSqlDriver == "QMYSQL") {
        ogrDriverStr = "MySQL:";
        ogrDriverStr.append(settingsManager->getDBName());
        ogrDriverStr.append(",user=");
        ogrDriverStr.append(settingsManager->getDBUser());
        ogrDriverStr.append(",password=");
        ogrDriverStr.append(settingsManager->getDBPass());
        ogrDriverStr.append(",port=");
        ogrDriverStr.append(settingsManager->getDBPort());
        ogrDriverStr.append(",host=");
        ogrDriverStr.append(settingsManager->getDBHost());
        this->setDriver(ogrDriverStr);
    }
    else if (qSqlDriver == "QPSQL")
    {
        ogrDriverStr = "PG:";
        ogrDriverStr.append("dbname=");
        ogrDriverStr.append(settingsManager->getDBName());
        ogrDriverStr.append(" user=");
        ogrDriverStr.append(settingsManager->getDBUser());
        ogrDriverStr.append(" password=");
        ogrDriverStr.append(settingsManager->getDBPass());
        ogrDriverStr.append(" port=");
        ogrDriverStr.append(settingsManager->getDBPort());
        ogrDriverStr.append(" host=");
        ogrDriverStr.append(settingsManager->getDBHost());
        this->setDriver(ogrDriverStr);
    }
}

QList<Point> Chart::getChartPoints(QString layerName, QString queryStr){
    //std::cout << "layerName: " << layerName.toStdString() << std::endl;

    QList<Point> chartPoints;

    OGRLayer *layer = (this->*getLayer)(queryStr);
    OGRFeature *feature = 0;

    QList<QString> fields = getFieldsForLayer(layerName);

#if 0
    // prints out some info for debugging
    std::cout << (const char *) layerName.toAscii() << std::endl;
    for(int cnt = 0; cnt < fields.size(); ++cnt) {
        std::cout << fields[cnt].toStdString() << std::endl;
    }
    std::cout << std::endl;
#endif

    layer->ResetReading();

    while( (feature = layer->GetNextFeature()) != NULL ) {
        Point point;
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();

        // For each feature go through the fields (AKA columns of relation)
        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn(iField);

            QString fieldName(fieldDef->GetNameRef());
            if( fields.contains("*") || fields.contains(fieldName.toLower()) || fields.contains(fieldName.toUpper()) ) {
                OGRFieldType type = fieldDef->GetType();

                if(type == OFTString ) {
                    point.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    point.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal) {
                    point.put(fieldName, feature->GetFieldAsDouble(iField));
                }

            }
        }

        // get the geometry for the feature
        OGRGeometry *geometry = feature->GetGeometryRef();
        if( (geometry != NULL) && (wkbFlatten(geometry->getGeometryType()) == wkbPoint )) {
            OGRPoint *ogrPoint = (OGRPoint *) geometry;
            QPointF pointF( ogrPoint->getX(), ogrPoint->getY());

            // For signnsound_p, we want to store its z value
            // (it means the "depth of water" at that point and
            // is going to be shown to the user.
            if(layerName == "signsound_p") {
                // TODO: Is the z still correct after flattening the geometry?
                qreal z = ogrPoint->getZ();
                point.setZ(z);
            }
            UwMath::toConformalInverted(pointF);
            point.setPoint(pointF);

        }
        else {
            //qWarning("No geometry, not really a warning");
        }

        chartPoints.append(point);

        OGRFeature::DestroyFeature( feature );
    }

    return chartPoints;
}

QList<Polygon> Chart::getChartLines(QString layerName, QString queryStr) {
    QList<Polygon> lines;
    OGRFeature *feature = 0;

    OGRLayer *layer = (this->*getLayer)(queryStr);
    QList<QString> fields = getFieldsForLayer(layerName);

    layer->ResetReading();
    while( (feature = layer->GetNextFeature()) != NULL ) {
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();
        Polygon tmpPolygon;

        // get the the information for the graphics element
        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn( iField );
            QString fieldName = fieldDef->GetNameRef();
            if(fields.contains(fieldName.toLower()) || fields.contains(fieldName.toUpper()) ) {
                OGRFieldType type = fieldDef->GetType();
                if(type == OFTString ) {
                    tmpPolygon.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    tmpPolygon.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal) {
                    tmpPolygon.put(fieldName, feature->GetFieldAsDouble(iField));
                }
            }
        }

        // get the geometry
        OGRGeometry *geometry = feature->GetGeometryRef();
        if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbLineString ) {
            // TODO: remove the C API and use only the C++ API
            int node, count = OGR_G_GetPointCount(geometry);
            QPolygonF tmpPolygonF;
            for (node = 0; node < count; node++) {
                QPointF p( OGR_G_GetX( geometry , node ), OGR_G_GetY( geometry, node ));
                tmpPolygonF << p;
            }
            UwMath::toConformalInverted(tmpPolygonF);
            tmpPolygon.setPolygon(tmpPolygonF);
            lines.append(tmpPolygon);
            tmpPolygonF.clear();
        }
        else {
            qWarning("file error");
        }
        OGRFeature::DestroyFeature(feature);
    }

    return lines;
}

QList<Polygon> Chart::getChartPolygons(QString layerName, QString queryStr) {

    if (debug) {
        QString dbg("Chart::getChartPolygons(): layerName:");
        dbg.append( layerName);
        dbg.append( " query: ");
        dbg.append( queryStr);
        qDebug() << dbg;
    }

    QList<Polygon> chartPolygons;
    OGRFeature *feature = 0;


    OGRLayer  *layer = (this->*getLayer)(queryStr);
    QList<QString> fields = getFieldsForLayer(layerName);

    int totalPolygonCount = 0;
    int totalRingCount = 0;
    int totalMultiPolygonCount = 0;

    bool debug = false;
    QString msg;
#if 0
    // prints out some info for debugging
    std::cout << "getChartPolygons(): " << (const char *) layerName.toAscii() << std::endl;
    std::cout << "getChartPolygons(): " <<(const char *) queryStr.toAscii() << std::endl;
    for(int cnt = 0; cnt < fields.size(); ++cnt) {
        if (debug) std::cout << fields[cnt].toStdString() << std::endl;
    }
    //std::cout << std::endl;
#endif

    layer->ResetReading();
    while( (feature = layer->GetNextFeature()) != NULL ) {
        Polygon pol;
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();

        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn( iField );
            QString fieldName = fieldDef->GetNameRef();
            fieldName = fieldName.toLower();

            if(fields.contains(fieldName.toLower()) || fields.contains(fieldName.toUpper()) ) {
                OGRFieldType type = fieldDef->GetType();
                if(type == OFTString ) {
                    pol.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    pol.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal)  {
                    pol.put(fieldName, feature->GetFieldAsDouble(iField));
                }
            }
            //msg.sprintf("%d: iField = %d, fieldName = ", polygonCount, iField);
            //msg.append(fieldName);
            //msg.append(", value = ");
            //msg.append(pol.get(fieldName).toString());
            //qDebug(msg);
        }

        QPolygonF polygon;

        OGRGeometry *geometry = feature->GetGeometryRef();
        if (debug) std::cout << "Getting feature with OGR_FID " << feature->GetFID() << std::endl;
        if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbPolygon ) {
            OGRGeometryH hRing;
            int iRing = 0, nRingCount = OGR_G_GetGeometryCount(geometry);
            for( iRing = 0; iRing < nRingCount; iRing++ ) {
                int node;
                hRing = OGR_G_GetGeometryRef( geometry, iRing );
                int vert_count = OGR_G_GetPointCount(hRing);
                for( node = vert_count - 1; node >= 0; node-- ) {
                    QPointF p(OGR_G_GetX(hRing , node),OGR_G_GetY( hRing, node ));
                    polygon << p;
                }
                UwMath::toConformalInverted(polygon);
                pol.setPolygon(polygon);
                chartPolygons.append(pol);
                polygon.clear();
                if (debug) std::cout << "Added wkbPolygon ring with " << vert_count << " points, ring count "
                                     << iRing << std::endl;
                totalRingCount++;
            }
            totalPolygonCount++;
        }
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
                    for( node = vert_count - 1; node >= 0; node-- ) {
                        nShapeRing++;  //not used
                        QPointF p(OGR_G_GetX(hRing , node), OGR_G_GetY( hRing, node ));
                        polygon << p;
                    }
                    UwMath::toConformalInverted(polygon);
                    pol.setPolygon(polygon);
                    chartPolygons.append(pol);
                    if (debug) std::cout << "Added wkbMultiPolygon ring with " << vert_count << " points, ring count "
                                         << iRing << std::endl;
                    totalRingCount++;
                }
                if (debug) std::cout << "Adding wkbMultiPolygon polygon, count " << iPoly << std::endl;
                totalPolygonCount++;
            }
            totalMultiPolygonCount++;
        }
        else {
            qWarning("file error");
        }

        OGRFeature::DestroyFeature( feature );
    }
    if (debug) std::cout << "Loaded " << totalPolygonCount << " polygons, " << totalRingCount
                         << " rings, " << totalMultiPolygonCount << " multipolygons." << std::endl;

    return chartPolygons;
}

QVector<QString*> Chart::getLayersNames()
{
    QVector<QString *> layers;

    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;

    QString sqlQuery;
    if (settingsManager->getDBDriver() == "PostgreSQL") {
        sqlQuery = "select table_name from information_schema.tables where table_schema='public' and table_type='BASE TABLE'";
    } else if (settingsManager->getDBDriver() == "MySQL") {
        sqlQuery = "show tables;";
    }
    OGRLayer * layersNames = dataSource->ExecuteSQL(sqlQuery.toAscii(),spatialFilter,dialect);

    //qDebug() << "Gets all the tablenames from DB";
    for( int i = 0; i < dataSource->GetLayerCount(); i++ )
    {
        OGRFeature* feat = layersNames->GetFeature(i);
        if(feat)
        {
            QString layerName = (QString)feat->GetFieldAsString(0);
            layers.push_back(new QString(layerName));
        }
    }

    return layers;
}

QStringList Chart::getSimplifiedLayers() {
    qDebug()<<Q_FUNC_INFO;
    QStringList layerList;
    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    QString sqlQuery;
    QString layerName;

    if (settingsManager->getDBDriver() == "PostgreSQL") {
        sqlQuery = "select table_name from information_schema.tables where table_schema='public' and table_type='BASE TABLE'";
    } else if (settingsManager->getDBDriver() == "MySQL") {
        sqlQuery = "show tables;";
    }
    qDebug()<<Q_FUNC_INFO<<"Before";
    OGRLayer * layersNames = dataSource->ExecuteSQL(sqlQuery.toAscii(), spatialFilter, dialect);
    qDebug()<<Q_FUNC_INFO<<"End";
    for( int i = 0; i < dataSource->GetLayerCount(); i++ ) {
        OGRFeature* feat = layersNames->GetFeature(i);
        if(feat) {
            layerName = (QString)feat->GetFieldAsString(0);
            if (layerName.endsWith("_l") || layerName.endsWith("_r")) {
                layerList << layerName;
            }
        }
    }
    layerList.append("lake_r");
    return layerList;
}

QStringList Chart::getNonSimplifiedLayers() {
    qDebug()<<Q_FUNC_INFO;
    QStringList layerList;
    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    QString sqlQuery;
    QString layerName;

    if (settingsManager->getDBDriver() == "PostgreSQL") {
        sqlQuery = "select table_name from information_schema.tables where table_schema='public' and table_type='BASE TABLE'";
    } else if (settingsManager->getDBDriver() == "MySQL") {
        sqlQuery = "show tables;";
    }
    qDebug()<<Q_FUNC_INFO<<"Before";
    OGRLayer * layersNames = dataSource->ExecuteSQL(sqlQuery.toAscii(), spatialFilter, dialect);
    qDebug()<<Q_FUNC_INFO<<"After";
    for( int i = 0; i < dataSource->GetLayerCount(); i++ ) {
        OGRFeature* feat = layersNames->GetFeature(i);
        if(feat) {
            layerName = (QString)feat->GetFieldAsString(0);
            if (layerName.endsWith("_p")) {
                layerList << layerName;
            }
        }
    }
    layerList.append("obstacles");
    layerList.append("lights");
    return layerList;
}

OGRLayer* Chart::getLayerCopy( QString srcLayer, QString copyLayerName )
{
    OGRLayer *poLayer_src;
    poLayer_src = dataSource->GetLayerByName( srcLayer.toAscii() );

    char ** papszOptions = NULL;

    papszOptions = CSLSetNameValue( papszOptions, "OVERWRITE", "YES" );
    OGRLayer* layer = dataSource->CopyLayer( poLayer_src, copyLayerName.toAscii(), papszOptions );

    //This line of code is commented because the plugin does not link otherwise, this should be fixed ASAP
    //CSLDestroy( papszOptions );
    return layer;
}

QList<QString> Chart::getFieldsForLayer( const QString &layerName) {
    if ( fieldsForLayers.contains( layerName) )
        return fieldsForLayers[layerName];
    else {
        QList<QString> temp;
        temp.append("*");
        return temp;
    }
}

OGRLayer *Chart::getLayerFromDatabase(QString sqlQuery) {
    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    OGRLayer *layer = dataSource->ExecuteSQL( sqlQuery.toAscii(), spatialFilter, dialect );
    if( layer == NULL ) {
        std::cerr << "layer not found or errors encountered\n";
    }
    return layer;
}

OGRLayer *Chart::getLayerFromSHPFile(QString fileName) {
    OGRLayer *layer = dataSource->GetLayerByName(fileName.toAscii());
    if(layer == NULL) {
        std::cerr << "Layer not found or errors encountered\n";
    }

    return layer;
}

QDir Chart::createDirectory(QString path)
{
    // TODO: do we really need this here?
    QDir directory;
    directory.cd(path);
    if(!directory.cd(".VDF"))
    {
        directory.mkdir(".VDF");
        directory.cd(".VDF");
    }
    return directory;
}
QList<Point> Chart::getChartPointsArea(QString lay,double x,double y,double i,double j){

    QList<Point> chartPoints;

    //Create the sql Query
    QString sql=" SELECT * FROM ";
    sql.append(lay);

    sql.append(" WHERE X(POINTFROMWKB(ASWKB(SHAPE),0)) < ");
    char newstring[50];
    sprintf(newstring,"%g",x);
    sql.append(newstring);

    sql.append(" AND Y(POINTFROMWKB(ASWKB(SHAPE),0)) <  ");
    char news[50];
    sprintf(news,"%g",y);
    sql.append(news);

    sql.append(" AND X(POINTFROMWKB(ASWKB(SHAPE),0)) > ");
    char newst[50];
    sprintf(newst,"%g",i);
    sql.append(newst);

    sql.append(" AND Y(POINTFROMWKB(ASWKB(SHAPE),0)) >  ");
    char nt[50];
    sprintf(nt,"%g",j);
    sql.append(nt);
    sql.append(" ;");




    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    OGRLayer *layer = dataSource->ExecuteSQL( sql.toAscii(), spatialFilter, dialect );
    if( layer == NULL ) {
        std::cerr << "layer not found or errors encountered\n";
    }
    OGRFeature *feature = 0;

    QList<QString> fields = getFieldsForLayer(lay);
    layer->ResetReading();
    while( (feature = layer->GetNextFeature()) != NULL ) {
        Point point;
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();

        // For each feature go through the fields (AKA columns of relation)
        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn(iField);

            QString fieldName(fieldDef->GetNameRef());
            if(fields.contains(fieldName) == true) {
                OGRFieldType type = fieldDef->GetType();
                if(type == OFTString ) {
                    point.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    point.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal) {
                    point.put(fieldName, feature->GetFieldAsDouble(iField));
                }
            }
        }

        // get the geometry for the feature
        OGRGeometry *geometry = feature->GetGeometryRef();
        if( (geometry != NULL) && (wkbFlatten(geometry->getGeometryType()) == wkbPoint )) {
            OGRPoint *ogrPoint = (OGRPoint *) geometry;
            QPointF pointF(ogrPoint->getY(), ogrPoint->getX());
            point.setPoint(pointF);
            chartPoints.append(point);
        }
        else {
            qWarning("file error");
        }

        OGRFeature::DestroyFeature( feature );
    }



    return chartPoints;
}

QList<Line> Chart::getChartLinesArea(QString layerName,double x,double y,double i,double j) {
    QList<Line> lines;

    QString sql=" SELECT * FROM ";
    sql.append(layerName);
    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    OGRLayer *layer = dataSource->ExecuteSQL( sql.toAscii(), spatialFilter, dialect );
    if( layer == NULL ) {
        std::cerr << "layer not found or errors encountered\n";
    }

    OGRFeature *feature = 0;
    QList<QString> fields = getFieldsForLayer(layerName);

    layer->ResetReading();
    while( (feature = layer->GetNextFeature()) != NULL ) {
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();
        Line addLine;

        // get the the information for the graphics element
        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn( iField );
            QString fieldName = fieldDef->GetNameRef();

            if(fields.contains(fieldName) == true) {
                OGRFieldType type = fieldDef->GetType();
                if(type == OFTString ) {
                    addLine.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    addLine.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal) {
                    addLine.put(fieldName, feature->GetFieldAsDouble(iField));
                }
            }
        }

        // get the geometry
        OGRGeometry *geometry = feature->GetGeometryRef();
        if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbLineString ) {
            // TODO: remove the C API and use only the C++ API
            //OGRLineString *ogrLine = (OGRLineString *) geometry;
            int node, count = OGR_G_GetPointCount(geometry);
            bool first = true;
            QLineF line, nextLine;

            for( node = count-1; node >= 0; node-- ) {
                QPointF p(OGR_G_GetY( geometry, node ), OGR_G_GetX( geometry , node ));
                double ypoint=p.rx();
                double xpoint=p.ry();
                if ((xpoint < x)&&(ypoint < y)&&(xpoint > i)&&(ypoint > j))
                {

                    if(first) {
                        line.setP1(p);
                        first = false;
                    }
                    else {
                        line.setP2(p);
                        nextLine.setP1(p);

                        addLine.setLine(line);
                        lines.append(addLine);

                        line = nextLine;
                    }
                }
            }
        }
        else {
            qWarning("file error");
        }

        OGRFeature::DestroyFeature(feature);
    }

    return lines;
}


QList<Polygon> Chart::getChartPolygonsArea(QString layerName,double x,double y,double i,double j) {
    QList<Polygon> chartPolygons;
    QString sql=" SELECT * FROM ";
    sql.append(layerName);
    const char *dialect = "";
    OGRGeometry *spatialFilter = NULL;
    OGRLayer *layer = dataSource->ExecuteSQL( sql.toAscii(), spatialFilter, dialect );
    if( layer == NULL ) {
        std::cerr << "layer not found or errors encountered\n";
    }

    OGRFeature *feature = 0;

    QList<QString> fields = getFieldsForLayer(layerName);

    layer->ResetReading();
    while( (feature = layer->GetNextFeature()) != NULL ) {
        Polygon pol;
        OGRFeatureDefn *featureDef = layer->GetLayerDefn();

        for( int iField = 0; iField < featureDef->GetFieldCount(); iField++ ) {
            OGRFieldDefn *fieldDef = featureDef->GetFieldDefn( iField );
            QString fieldName = fieldDef->GetNameRef();

            if(fields.contains(fieldName) == true) {
                OGRFieldType type = fieldDef->GetType();
                if(type == OFTString ) {
                    pol.put(fieldName, feature->GetFieldAsString(iField));
                } else if(type == OFTInteger) {
                    pol.put(fieldName, feature->GetFieldAsInteger(iField));
                } else if(type == OFTReal)  {
                    pol.put(fieldName, feature->GetFieldAsDouble(iField));
                }
            }
        }

        QPolygonF polygon;

        OGRGeometry *geometry = feature->GetGeometryRef();
        if( geometry && wkbFlatten(geometry->getGeometryType()) == wkbPolygon ) {
            OGRGeometryH hRing;
            int iRing = 0, nRingCount = OGR_G_GetGeometryCount(geometry);
            for( iRing = 0; iRing < nRingCount; iRing++ ) {
                int node;
                hRing = OGR_G_GetGeometryRef( geometry, iRing );
                int vert_count = OGR_G_GetPointCount(hRing);
                for( node = vert_count - 1; node >= 0; node-- ) {
                    QPointF p(OGR_G_GetY( hRing, node ), OGR_G_GetX(hRing , node));
                    double ypoint=p.rx();
                    double xpoint=p.ry();
                    if ((xpoint < x)&&(ypoint < y)&&(xpoint > i)&&(ypoint > j))
                    {
                        polygon << p;
                    }
                }
                pol.setPolygon(polygon);
                chartPolygons.append(pol);
                polygon.clear();
            }
        }
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
                    for( node = vert_count - 1; node >= 0; node-- ) {
                        nShapeRing++;  //not used
                        QPointF p(OGR_G_GetY( hRing, node ), OGR_G_GetX(hRing , node));
                        double ypoint=p.rx();
                        double xpoint=p.ry();
                        if ((xpoint < x)&&(ypoint < y)&&(xpoint > i)&&(ypoint > j))
                        {
                            polygon << p;
                        }
                    }
                    pol.setPolygon(polygon);
                    chartPolygons.append(pol);
                }
            }
        }
        else {
            qWarning("file error");
        }

        OGRFeature::DestroyFeature( feature );
    }

    return chartPolygons;
}
