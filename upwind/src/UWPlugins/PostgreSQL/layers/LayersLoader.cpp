#include "LayersLoader.h"
#include "LayersManager.h"
#include <QDebug>

LayersLoader::LayersLoader(QVector<LayerVisualInfo*> layerVisualInfoVector, ChartTableSettings* chartTableSettings) {
	settingsManager = SettingsManager::getInstance();
	this->layerVisualInfoVector = layerVisualInfoVector;
	this->chartTableSettings = chartTableSettings;
	this->loadDriver();
	this->debug = true;
}

// TODO: ChartTable::getSrcTableForLevel(QString layerId, int level)

LayersLoader::~LayersLoader()
{
}

void LayersLoader::loadDriver()
{
	if (settingsManager->getDBDriver() == "MySQL")
	{
		QString driver = "MySQL:";
		driver.append(settingsManager->getDBName());
		driver.append(",user=");
		driver.append(settingsManager->getDBUser());
		driver.append(",password=");
		driver.append(settingsManager->getDBPass());
		driver.append(",port=");
		driver.append(settingsManager->getDBPort());
		driver.append(",host=");
		driver.append(settingsManager->getDBHost());
		chart.setDriver(driver);
	}
	else if (settingsManager->getDBDriver() == "PostgreSQL")
	{
		QString driver = "PG:";
		driver.append("dbname=");
		driver.append(settingsManager->getDBName());
		driver.append(" user=");
		driver.append(settingsManager->getDBUser());
		driver.append(" password=");
		driver.append(settingsManager->getDBPass());
		driver.append(" port=");
		driver.append(settingsManager->getDBPort());
		driver.append(" host=");
		driver.append(settingsManager->getDBHost());
		chart.setDriver(driver);
	}

}

void LayersLoader::setLayerType(Layer::LayerType type)
{
	this->type = type;
}


void LayersLoader::run()
{
	qDebug() << "** LayersLoader: thread starting to load " << Layer::toString(this->type);

	QString layerId = Layer::toString(this->type);
	int level;

	if (queuedHandler == NULL) {
		bool isSimplified;
		// TODO: maybe do this by counting LayerVisualInfo items for this layer
		if ( Layer::toString(this->type).endsWith("_p") ||
                         Layer::toString(this->type) == "obstacles" ||
                         Layer::toString( this->type) == "lights" )
		{
			isSimplified = false;
		} else {
			isSimplified = true;
		}
		queuedHandler = new LayerSimplificationHandler(layerId,
			   this->getVisibilityRanges(layerId), isSimplified);
	}

	QVector<int> lv = loadQueue.take(layerId);
	QMutableVectorIterator<int> vi(lv);
	while (vi.hasNext()) {
		level = vi.next();
		if (level != -1) {
			if (debug) qDebug() << "LayersLoader: load layer " + layerId + " level " +
					QString::number(level) + " from queue...";
			Layer * layerLevel = this->loadLayerLevel(layerId, level);

                        QString levelName;
                        if (layerId == "lake_r") {
                            if (level != 0)
                                levelName = layerId + "_level" + QString::number(level);
                            else
                                levelName = layerId;
                        } else
                            levelName = this->getSrcTableForSimpLevel(layerId, level);

                        queuedHandler->addLayerLevel(levelName, layerLevel);
			vi.remove();
			if (debug) qDebug() << "LayersLoader: load layer " + layerId + " level " +
					QString::number(level) + " from queue... OK";
		}
	}

	if (debug) qDebug() << "LayersLoader::run(): Loaded layers: ";
	if (debug) queuedHandler->printLayerNames();

	qDebug() << "** LayersLoader: stopping thread: " << layerId;
	emit this->loadingReady(queuedHandler);
}

QString LayersLoader::getSrcTableForSimpLevel(QString layerId, int level) {
	if (layerId == "lake_r") layerId = "deptharea_r";
	QVectorIterator<LayerVisualInfo*> it(layerVisualInfoVector);
	LayerVisualInfo* tmpLvi;
	while (it.hasNext()) {
		tmpLvi = it.next();
		if (tmpLvi->getLayerId() == layerId) {
			if (tmpLvi->getSimpLevel() == level) {
				return tmpLvi->getSrcTableName();
			}
		}
	}
	return "";
}

QString LayersLoader::buildQuery(QString layername, QString layerId) {

	// This will save trafic with DB by not selecting * always
	// if you need any specific atribute from a layer, just
	// add it to the list of fields in chart class
        QString layerIdCopy;

        if ( layerId.isEmpty() ) {
            layerIdCopy = layername;
            int level = layerIdCopy.indexOf( "_le", 0, Qt::CaseInsensitive);
            if ( level != -1)
                layerIdCopy.truncate( level);
        } else {
            layerIdCopy = layerId;
        }

        QList<QString> fields = chart.getFieldsForLayer( layerIdCopy);
        qDebug()<<"fields"<<fields;
    QString query;
	if ( fields.at( 0) == "*" || fields.contains("*") )
		query.append( "SELECT ");
	else
		query.append( "SELECT OGC_FID, wkb_geometry, ");

	for ( int i = 0; i < fields.size() - 1; i++ ) {
		query.append( fields.at( i));
		query.append( ", ");
	}

	query.append( fields.at( fields.size() - 1 ) );
	query.append( " FROM " );
        query.append( layername);

	// typedepr 0= unknown 1= sea area 2= lake area 3= area not surveyed completely
	// some Lakes have typedepr 0, but is not safe to include those types into lake layer
        if ( layerIdCopy == "lake_r" )
            query.append( " WHERE TYPEDEPR=2");
        else if( layerIdCopy == "deptharea_r")
                query.append(" WHERE TYPEDEPR<>2 ORDER BY ST_Area( wkb_geometry ) DESC");
        qDebug()<<"buildqry:"<<query;
	return query;
}

Layer* LayersLoader::loadLayerLevel(QString layerId, int level)
{
	//QPen pen;
	Layer* layer = NULL;

	switch (type) {

	//POLYGON LAYERS
	case Layer::obstacles:
		layer = loadObstaclesLayerLevel();
		break;
	case Layer::generarea_r:
		layer = loadPolygonLayerLevel( layerId, level, 1, COLOR_GENERAREA_PEN, COLOR_GENERAREA_BRUSH );
		break;
	case Layer::deptharea_r:
		layer = loadDepthAreaLayerLevel( level);
		break;
	case Layer::lake_r:
		layer = loadPolygonLayerLevel( layerId, level, 1.5, COLOR_LAKE_PEN, COLOR_LAKE_BRUSH );
		break;
	case Layer::spoilgrnd_r:
		layer = loadPolygonLayerLevel( layerId, level, 2, COLOR_SPOILGRND_PEN );
		break;
	case Layer::limit_r:
		layer = loadPolygonLayerLevel( layerId, level, 2, COLOR_LIMIT_PEN );
		break;
	case Layer::builtupare_r:
		layer = loadPolygonLayerLevel( layerId, level, 4, COLOR_DEFAULT, COLOR_BUILTUPARE_BRUSH, OPACITY_BUILTUPARE  );
		break;
	case Layer::fwayarea_r:
		layer = loadPolygonLayerLevel( layerId, level, 2, COLOR_DEFAULT, COLOR_FWAYAREA_BRUSH,  OPACITY_FWAYAREA );
		break;
	case Layer::survqual_r:
		layer = loadPolygonLayerLevel( layerId, level, 3, COLOR_SURVQUAL_PEN, COLOR_DEFAULT, OPACITY_SURVQUAL );
		break;
	case Layer::building_r:
		layer = loadPolygonLayerLevel( layerId, level, 4, COLOR_DEFAULT, COLOR_BUILDING_BRUSH, OPACITY_BUILDING );
		break;
	case Layer::dock_r:
		layer = loadPolygonLayerLevel( layerId, level, 3, COLOR_DOCK_PEN );
		break;

	// LINE LAYERS

	case Layer::depthcont_l:
		layer = loadLineLayerLevel( layerId, level, 3, COLOR_DEPTHCONT_PEN );
		break;
	case Layer::coastline_l:
		layer = loadLineLayerLevel( layerId, level, 3.5, COLOR_COASTLINE_PEN );
		break;
	case Layer::closinglne_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_CLOSINGLNE_PEN );
		break;
	case Layer::navigline_l:
		layer = loadLineLayerLevel( layerId, level, 5, COLOR_NAVIGLINE_PEN );
		break;
	case Layer::ferry_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_FERRY_PEN );
		break;
	case Layer::limit_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_LIMIT_PEN, STYLE_LIMIT_PEN );
		break;
	case Layer::heightcont_l:
		layer = loadLineLayerLevel( layerId, level, 3.5, COLOR_HEIGHTCONT_PEN );
		break;
	case Layer::leadinglne_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_LEADINGLNE_PEN, STYLE_LEADINGLNE_PEN );
		break;
	case Layer::lockndam_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_LOCKNDAM_PEN );
		break;
	case Layer::pipeline_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_PIPELINE_PEN, STYLE_PIPELINE_PEN );
		break;
	case Layer::submcable_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_SUBMCABLE_PEN, STYLE_SUBMCABLE_PEN );
		break;
	case Layer::roadnrailw_l:
		layer = loadLineLayerLevel( layerId, level, 4.5, COLOR_ROADNRAILW_PEN  );
		break;
	case Layer::bridge_l:
		layer = loadLineLayerLevel( layerId, level, 4.5, COLOR_BRIDGE_PEN );
		break;
	case Layer::transmline_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_TRANSMLINE_PEN );
		break;
	case Layer::river_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_RIVER_PEN );
		break;
	case Layer::shlnecon_l:
		layer = loadLineLayerLevel( layerId, level, 4, COLOR_SHLNECON_PEN );
		break;

	// POINT LAYERS

	case Layer::navaid_p:
                layer = loadNavaId();
		break;
	case Layer::limit_p:
		layer = loadPointLayerLevel("l", layerId);
		break;
	case Layer::towernmast_p:
		layer = loadPointLayerLevel("|", layerId);
		break;
	case Layer::signsound_p:
		layer = loadSignSoundLayerLevel();
		break;
	case Layer::ctext_p:
		layer = loadTextLayerLevel();
		break;
	case Layer::vegetation_p:
		layer = loadPointLayerLevel("v", layerId);
		break;
	case Layer::moorfacil_p:
		layer = loadPointLayerLevel("m", layerId);
		break;
	case Layer::natseabed_p:
		layer = loadPointLayerLevel("n", layerId);
		break;
	case Layer::obstruct_p:
		layer = loadPointLayerLevel("o", layerId);
		break;
	case Layer::harbour_p:
		layer = loadPointLayerLevel("h", layerId);
		break;
	case Layer::marservice_p:
		layer = loadPointLayerLevel("<", layerId);
		break;
	case Layer::wreck_p:
		layer = loadWreckLayerLevel();
		break;
	case Layer::building_p:
		layer = loadPointLayerLevel(".", layerId);
		break;
	case Layer::rock_p:
		layer = loadRocksLayerLevel();
		break;
	case Layer::checkpoint_p:
                layer = loadPointLayerLevel("c", layerId);
		break;
	case Layer::fortstruct_p:
		layer = loadPointLayerLevel("f", layerId);
		break;

	case Layer::lights:
		layer = loadLightsLayerLevel();
		break;

	case Layer::unknown:
		break;

	default:
		break;

	}
	return layer;
}

void LayersLoader::setLayerSimplificationHandler(LayerSimplificationHandler *handler) {
	this->queuedHandler = handler;
}

void LayersLoader::addToLoadQueue(QString layerId, QVector<int> levels) {
	QString tmp = "LayerLoader: adding to load queue: layer " + layerId + ", levels ";
	QVectorIterator<int> vi(levels);
	while (vi.hasNext()) {
		tmp.append(QString::number(vi.next()));
		if (vi.hasNext()) tmp.append(", ");
	}
	if (debug) qDebug() << tmp;
	this->loadQueue.insert(layerId, levels);
}

void LayersLoader::removeFromLoadQueue(QString layerId, int level) {
	if (debug) qDebug() << "LayerLoader: removing from load queue: " + layerId + " " + QString::number(level);
	if (!this->loadQueue.contains(layerId)) return;
	QVector<int> levels = this->loadQueue.value(layerId);
	QMutableVectorIterator<int> vi(levels);
	while (vi.hasNext()) {
		int val = vi.next();
		if (val == level) {
			vi.remove();
			return; // assume non-duplicate values
		}
	}
}

QHash<QPair<int,int>,int> LayersLoader::getVisibilityRanges(QString layername)
{
	if (layername == "lake_r") layername = "deptharea_r";
	QPair<int,int> pairs;

	QHash<QPair<int,int>,int> levels;
	levels.clear();

	QVectorIterator<LayerVisualInfo*> i(layerVisualInfoVector);
	LayerVisualInfo * tmp;
	while(i.hasNext())
	{
		tmp = i.next();
		if(tmp->getLayerId() == layername)
		{
			pairs.first   = tmp->getZoomMin();
			pairs.second  = tmp->getZoomMax();
			levels[pairs] = tmp->getSimpLevel();
		}
	}
	return levels;
}

// Returns Polygon and layername --> bridge_l, bridge_l_level1 and so on..
QHash<QString, QList<Polygon> > LayersLoader::getPolygonVector(QString layername)
{
	QHash< QString,QList<Polygon> > list;
	list.clear();
	QList<Polygon> polyList;
	polyList.clear();
	QString query;

	// Gets all the layernames so we have to pick the right ones on the run
	QVector<QString*> layerNames = chart.getLayersNames();

	int i=0;
	while( i<layerNames.size() )
	{

		if( layerNames[i]->startsWith(layername,Qt::CaseInsensitive) ||
			( layername == "lake_r" &&
			  layerNames[i]->startsWith("deptharea",Qt::CaseInsensitive) ) )
		{

			query = ( layername == "lake_r" ) ?
					buildQuery( layername) :
					buildQuery( *layerNames[i]);

			// Can be fixed later to use LayersVisualInfo
			if(layername.endsWith("_r")) {
				polyList = chart.getChartPolygons(*layerNames[i], query);
			} else if(layername.endsWith("_l")) {
				polyList = chart.getChartLines(*layerNames[i], query);
			}

			list.insert(*layerNames[i],polyList);
		}
		i++;

	}

	return list;
}

QList<Polygon> LayersLoader::loadPolygonsFromTable(QString layerId, QString srcTableName) {
	QList<Polygon> list;
	list.clear();
	QString query;

        if (layerId == "lake_r")
            query = buildQuery( srcTableName, layerId);
        else
            query = buildQuery( srcTableName);

	// Can be fixed later to use LayersVisualInfo
	if(layerId.endsWith("_r")) {
		list = chart.getChartPolygons(layerId, query);
	} else if(layerId.endsWith("_l")) {
		list = chart.getChartLines(layerId, query);
	}
	return list;
}

Layer* LayersLoader::loadPolygonLayerLevel(QString layerId, int level, qreal zValue,
											QString pen_color, QString brush_color, float opacity)
{

	QString srcTableName = getSrcTableForSimpLevel(layerId, level);
	QList<Polygon> polyList = loadPolygonsFromTable(layerId, srcTableName);
	Layer* layer = new Layer();

	QPen local_pen;
	if ( pen_color == COLOR_DEFAULT )
		local_pen.setColor( QColor( Qt::transparent));
	else
		local_pen.setColor( QColor( pen_color));

	if(!polyList.isEmpty())	{
		for (int i=0; i<polyList.size(); i++ ) {
			QGraphicsPolygonItem *chartItem = new QGraphicsPolygonItem(polyList[i].getPolygon());
			if ( brush_color == COLOR_DEFAULT )
				chartItem->setBrush( QColor( Qt::transparent));
			else
				chartItem->setBrush( QColor( brush_color));
			chartItem->setPen( local_pen);
			chartItem->setZValue( zValue);
			layer->addToGroup(chartItem);
		}
		layer->setZValue(zValue);
		if ( opacity != OPACITY_DEFAULT )
			layer->setOpacity( opacity);
	}
	return layer;
}

Layer* LayersLoader::loadDepthAreaLayerLevel(int level) {
	QString layerId("deptharea_r");
	QString srcTableName = getSrcTableForSimpLevel(layerId, level);
	QList<Polygon> polyList = loadPolygonsFromTable(layerId, srcTableName);
	Layer* layer = new Layer();

	float ZValue = 0.0;
	QPen local_pen(QColor(COLOR_DEPTHCONT));

	if(!polyList.isEmpty())	{
		for (int i=0; i<polyList.size(); i++ ) {
			QGraphicsPolygonItem *chartItem = new QGraphicsPolygonItem(polyList[i].getPolygon());

			double depth = polyList[i].get("MINDEPTH").toDouble();
			if ( depth >= 10.0 )
				chartItem->setBrush(QColor(COLOR_DEEP_WATER));
			else
				chartItem->setBrush(QColor(COLOR_SALLOW_WATER));

			chartItem->setZValue(ZValue);
			ZValue = ZValue + 0.00001;
			chartItem->setPen( local_pen);

			layer->addToGroup(chartItem);

		}
		layer->setZValue(0);
	}
	return layer;
}

Layer* LayersLoader::loadObstaclesLayerLevel() {

	if (debug) qDebug() << "LayersLoader::loadObstaclesLayer()";
	QString layername("obstacles");

	QString layerId("obstacles_l");
	QString query = buildQuery(layerId);
	QList<Polygon> polyList = chart.getChartLines( layerId, query);

	Layer* layer = new Layer();

	QColor local_color_brush(COLOR_OBSTACLES_BRUSH);
	QPen local_pen(QColor(COLOR_OBSTACLES_PEN));

	if(!polyList.isEmpty())	{
		for (int i=0; i<polyList.size(); i++ ) {
			QPainterPath *path = new QPainterPath();
			QPolygonF oldPoly = polyList[i].getPolygon();
			path->addPolygon(oldPoly);
			QGraphicsPathItem *chartItem = new QGraphicsPathItem(*path, layer, layer->scene());
			chartItem->setPen( local_pen);
			layer->addToGroup(chartItem);
		}
	}

	layerId = "obstacles_r";
	query = buildQuery(layerId);
	polyList = chart.getChartPolygons( layerId, query);

	if(!polyList.isEmpty())	{
		for (int i=0; i<polyList.size(); i++ ) {

			QGraphicsPolygonItem *chartItem = new QGraphicsPolygonItem(polyList[i].getPolygon());
			chartItem->setPen( local_pen);
			chartItem->setBrush( local_color_brush);
			layer->addToGroup(chartItem);
		}
	}

	layer->setZValue( ZLEVEL_OBSTACLES);
	layer->setOpacity( OPACITY_OBSTACLES);

	return layer;
}

Layer* LayersLoader::loadLineLayerLevel(QString layerId, int level, qreal r, QString pen_color, int pen_style )
{
	QString layerSrcTable = this->getSrcTableForSimpLevel(layerId, level);
	QList<Polygon> lineList = this->loadPolygonsFromTable(layerId, layerSrcTable);

	Layer* layer;

	QPen local_pen;

	if ( pen_color == COLOR_DEFAULT )
		local_pen.setColor( QColor( Qt::transparent));
	else
		local_pen.setColor( QColor( pen_color));
	local_pen.setStyle( Qt::PenStyle( pen_style));
	int depth = 0;
	bool depthcont_l = ( layerId == "depthcont_l" );

	QPainterPath *path;	// QGraphicsLineItem and QGraphicsPolygonItem had problems
	QPolygonF oldPoly;
	QGraphicsPathItem *chartItem;

	layer = new Layer();
        /*if(layerId == "submcable_l"){
            //
            qDebug()<<"linesize"<<lineList.size();
            for (int i=0; i<lineList.size(); i++ ) {
                qDebug()<<"polygon count"<<lineList[i].getPolygon().count();

            }
        }else*/
	if(!lineList.isEmpty())	{
		for (int i=0; i<lineList.size(); i++ ) {
			path = new QPainterPath();

			oldPoly = lineList[i].getPolygon();

			path->addPolygon(oldPoly);
			chartItem = new QGraphicsPathItem(*path, layer, layer->scene());

			if ( depthcont_l ) {
				depth = lineList[i].get("VALDCO").toInt();
				if ( depth <= 3 )
					local_pen.setStyle( Qt::DotLine);
				else if ( depth == 6 )
					local_pen.setStyle( Qt::DashLine );
				else if ( depth > 6 )
					local_pen.setStyle( Qt::SolidLine);
			}
			chartItem->setPen( local_pen);
			//chartItem->setZValue(r);
			layer->addToGroup(chartItem);
		}
		//qDebug() << "LayersLoader::loadLineLayers(): loaded line layer " << layerId << " in "
		//		<< QString::number(timer.elapsed()) << " ms";
		layer->setZValue(r);
		//layer->layerFeatureType(Layer::layer_line);
	}
	return layer;
}

Layer* LayersLoader::loadPointLayerLevel(QString symbol, QString layername)
{
	Layer* layer = new Layer();
	QList<Point> pointList;
	pointList.clear();

	QString query = buildQuery( layername);
	QColor local_color( COLOR_SYMBOLS);

	pointList = chart.getChartPoints( layername, query);

	int i = 0;
	while (!pointList.isEmpty() && i < pointList.size())
	{
		QGraphicsSimpleTextItem *chartItem = new QGraphicsSimpleTextItem(symbol);
		//chartItem->setZValue( ZLEVEL_SYMBOLS);
		chartItem->setPos(pointList[i].getPoint());
		chartItem->setBrush( local_color);
		//chartItem->scale(2, 2);
		chartItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		layer->addToGroup(chartItem);
		i++;
	} //TODO: with pixmap is possible to set offset and the position is more exact with low zoom level
	layer->setZValue(ZLEVEL_SYMBOLS);

	return layer;
}

Layer* LayersLoader::loadWreckLayerLevel()
{
	Layer* layer = new Layer();
	QList<Point> pointList;
	pointList.clear();

	QString layerName("wreck_p");
	QString query = buildQuery( layerName);

	pointList = chart.getChartPoints( layerName, query);

	QPixmap wreck1;
	wreck1.load(":wreck1");
	QPixmap wreck2;
	wreck2.load(":wreck2");
	QPixmap wreck3;
	wreck3.load(":wreck3");

	int i = 0;
	while (!pointList.isEmpty() && i < pointList.size())
	{
		Point p = pointList[i];
		int type = p.get("TYPEWRECK").toInt();
		QGraphicsPixmapItem *chartPix = new QGraphicsPixmapItem();
		if(type==1)
		{
			chartPix = new QGraphicsPixmapItem(wreck1);
			chartPix->setOffset(-145, -86);

		}
		else if(type==2)
		{
			chartPix = new QGraphicsPixmapItem(wreck2);
			chartPix->setOffset(-162, -105);
		}
		else
		{
			chartPix = new QGraphicsPixmapItem(wreck3);
			chartPix->setOffset(-132, -69);

		}
		chartPix->setZValue(5);
		chartPix->setPos(pointList[i].getPoint());
		chartPix->scale(0.2,0.2);
		chartPix->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		layer->addToGroup(chartPix);
		i++;
	}
	layer->setZValue(5);

	return layer;
}

Layer* LayersLoader::loadSignSoundLayerLevel()
{
	Layer* layer = new Layer();
	QList<Point> pointList;
	pointList.clear();

	QString layerName("signsound_p");
	QString query = buildQuery( layerName);

	pointList = chart.getChartPoints( layerName, query);
	QFont signsound_big_font( FONT_FAMILY_SIGNSOUND, FONT_SIZE_SIGNSOUND + 3, QFont::Normal, true);
	signsound_big_font.setStretch(QFont::Condensed);
	QFont signsound_small_font( FONT_FAMILY_SIGNSOUND, FONT_SIZE_SIGNSOUND , QFont::Normal, true);
	signsound_small_font.setStretch(QFont::Condensed);

	int i = 0;
	while (!pointList.isEmpty() && i < pointList.size())
	{
		float depth = pointList[i].get("depth").toDouble();
		int decimal = round( ( depth - floor( depth) ) * 10);

		QString sbig = QString::number( floor( depth));
		QGraphicsSimpleTextItem *bigItem = new QGraphicsSimpleTextItem( sbig);

		bigItem->setBrush(QColor(COLOR_SIGNSOUND));
		//bigItem->setPen(QPen(QColor(COLOR_SALLOW_WATER),0.2));
		bigItem->setFont( signsound_big_font);

		bigItem->setPos(pointList[i].getPoint());
		bigItem->setZValue(5.1);

		bigItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

		if ( decimal ) {

			QString ssmall = QString::number( decimal);
			QGraphicsSimpleTextItem *smallItem = new QGraphicsSimpleTextItem( ssmall);

			smallItem->setBrush(QColor(COLOR_SIGNSOUND));
			//chartItem->setPen(QPen(QColor(COLOR_SALLOW_WATER),0.2));
			smallItem->setFont( signsound_small_font);

			QRectF box = bigItem->boundingRect();
			smallItem->setParentItem( bigItem);
			smallItem->setTransform(QTransform().translate( box.width(), box.height() / 4 ));

			smallItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

		}

		i++;
		layer->addToGroup( bigItem);
	} //TODO: with pixmap is possible to set offset and the position is more exact with low zoom level

	layer->setZValue(5);

	return layer;
}

Layer* LayersLoader::loadTextLayerLevel()
{
	Layer* layer = new Layer();
	QList<Point> pointList;
	pointList.clear();

	QString layerName( "ctext_p");
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForCStrings(codec);
	QString query = buildQuery( layerName);
	pointList = chart.getChartPoints( layerName, query);
	QFont text_font( FONT_FAMILY_TEXT, FONT_SIZE_TEXT );

	int i = 0;
	while (!pointList.isEmpty() && i < pointList.size())
	{
		QGraphicsSimpleTextItem *chartItem = new QGraphicsSimpleTextItem(pointList[i].get("ntxtst").toString());
		chartItem->setZValue(5);
		chartItem->setPos(pointList[i].getPoint());
		chartItem->setBrush(QColor(COLOR_TEXT));
		chartItem->setFont( text_font );

		chartItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		layer->addToGroup(chartItem);
		i++;
	}
	layer->setZValue(5);
        QTextCodec::setCodecForCStrings(0);

	return layer;
}

Layer* LayersLoader::loadRocksLayerLevel()
{
	Layer* layer = new Layer();
	QList<Point> pointList;
	pointList.clear();

	QString layerName( "rock_p");
	QString query = buildQuery( layerName);
	pointList = chart.getChartPoints( layerName, query);

	int i = 0;
	while (!pointList.isEmpty() && i < pointList.size())
	{
                QString symbolPath = "";
		Point p = pointList[i];
		int type = p.get("fi_typrock").toInt();

		if(type==1)
                        symbolPath = ":/symbols/rock1.png";
		else if(type==2)
                        symbolPath = ":/symbols/rock2.png";
		else if((type==3) || (type==6))
                        symbolPath = ":/symbols/rock3.png";
		else if(type==4)
                        symbolPath = ":/symbols/rock4.png";
		else if(type==5)
                        symbolPath = ":/symbols/rock5.png";
		else if(type==7)
                        symbolPath = ":/symbols/rock6.png";

                if(symbolPath != ""){
                        QGraphicsPixmapItem *chartItem = new QGraphicsPixmapItem(QPixmap(symbolPath));
                        chartItem->setZValue(5);
                        chartItem->setPos(pointList[i].getPoint());
                        chartItem->scale(SCALE_FACTOR,SCALE_FACTOR);
                        chartItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
                        chartItem->setOffset(-chartItem->boundingRect().width()/2,-chartItem->boundingRect().height()/2);
                        layer->addToGroup(chartItem);
                }
                i++;
	}
	layer->setZValue(5);

	return layer;
}

Layer* LayersLoader::loadLightsLayerLevel()
{

        QList<Lighthouse> lights;
        QList<Buoy> buoys;
        int count = 0;
        double defaultRotation = 90.0,startAngle = 0.0, endAngle = 360.0;
        double rotation = defaultRotation;
        Layer* layer = new Layer();
        QList<Point> navaidList = chart.getChartPoints( "navaid_p", "SELECT * FROM navaid_p");
        QList<Point> lighthouseList = chart.getChartPoints( "light", "SELECT * FROM light");
        int buoyType = 0, buoyOrientation = 0;

        for(QList<Point>::iterator iterator = lighthouseList.begin();
        iterator != lighthouseList.end(); ++iterator )
        {
                QString currentName = iterator->get("navaidoid").toString();

                QPointF position;
                qint32 type = 0;

                for(QList<Point>::iterator navaidIterator = navaidList.begin();
                navaidIterator != navaidList.end(); ++navaidIterator)
                {
                        QString navaidName = navaidIterator->get("objectid").toString();
                        if(currentName == navaidName) {
                                type = navaidIterator->get("fi_typenav").toInt();
                                position = navaidIterator->getPoint();
                                buoyType = navaidIterator->get("FI_strcnav").toInt();
                                buoyOrientation = navaidIterator->get("ntypnavaid").toInt();

                                if(type != LIGHTHOUSE && type != SECTORBEACON && type != HELPBEACON){
                                    startAngle = iterator->get("startang").toDouble();
                                    endAngle = iterator->get("cloangle").toDouble();
                                    rotation = startAngle + ((endAngle - startAngle) / 2);
                                    qDebug()<<"debug rotation"<<rotation;
                                }
                        }
                }
                switch(type)
                {
                case LIGHTHOUSE:  // merimajakka -> lightMajor
                case SECTORBEACON: // sektoriloisto -> lightMinor
                case HELPBEACON: // apuloisto -> lightMinor (and sectors) ?
                        {
                                bool lighthouseDidExist = false;
                                for(QList<Lighthouse>::iterator lighthouse = lights.begin();
                                lighthouse != lights.end(); ++lighthouse )
                                {
                                        if(lighthouse->getName() == currentName)
                                        {
                                                lighthouse->addSector(*iterator);
                                                lighthouseDidExist = true;
                                        }
                                }

                                QString symbolPath = (type == 1 ? ":/symbols/lighthouse_major.png" : ":/symbols/lighthouse_minor.png");

                                if(lighthouseDidExist == false)
                                {
                                        qint16 officialCharacter = iterator->get("FI_OFFLCHR").toInt();
                                        qint16 exactCharacter = iterator->get("FI_PRELCHR").toInt();
                                        qint32 lightNumber = iterator->get("LIGHTNUM").toInt();

                                        Lighthouse lighthouse(position, currentName, symbolPath,
                                                                                  (LightType) type, lightNumber, officialCharacter, exactCharacter);
                                        lighthouse.addSector(*iterator);
                                        lights.append(lighthouse);
                                }
                                break;
                        }
                case NAVLIGHT: // unknown meaning (none on chart 57?)
                        break;

                case BUOY: // poiju; red, green buoys (these mark the fairway?)
                        {
                                LightColor color = (LightColor) iterator->get("colorlight").toInt(); // COLORLIGHT
                                double rotation = 90.0f;
                                //NOTE: poijuviitta and jääpoiju do not differ in the charts
                                QString topmark = ntypnavaidToString(buoyOrientation);

                                QString path = ":/symbols/icebuoy_"+topmark+".png";

                                Buoy buoy(position, currentName, (LightType) type,path , color, rotation);
                                buoys.append(buoy);
                                break;
                        }

                case FAYWAREMARK: // linjamerkki; yellow, red, green buoys (and white buoys too!)
                        {
                                LightColor color = (LightColor) iterator->get("colorlight").toInt();

                                Buoy buoy(position, currentName, (LightType) type, ":/symbols/buoyX.png", color, rotation);
                                buoys.append(buoy);
                                break;
                        }
                case EDGEMARK:
                        {
                                QString topmark = ntypnavaidToString(buoyOrientation);

                                if(startAngle == 0.0 && endAngle == 360.0 ) //default
                                    rotation = 90.0;

                                LightColor color = (LightColor) iterator->get("colorlight").toInt();

                                //qDebug()<<"rotation edgemark"<<rotation;
                                QString path = ":/symbols/edgemark_"+topmark+".png";
                                qDebug()<<"rotation edgemark"<<rotation;
                                Buoy buoy(position, currentName, (LightType) type, path, color, rotation);
                                buoys.append(buoy);

                                break;
                        }
                        case RADARTARGET:
                            {
                                LightColor color = (LightColor) iterator->get("colorlight").toInt();
                                QString path = ":/symbols/radartarget.png";
                                //qDebug()<<"path"<<path<<"at"<<position;
                                Buoy buoy(position, currentName, (LightType) type, path, color, rotation);
                                buoys.append(buoy);
                                ++count;
                                break;
                            }
                default:
                        ;
                }

                rotation = defaultRotation;
        }

        for(QList<Lighthouse>::iterator lighthouse = lights.begin();
        lighthouse != lights.end(); ++lighthouse)
        {
                lighthouse->render(layer);
        }

        for(QList<Buoy>::iterator buoy = buoys.begin();
        buoy != buoys.end(); ++buoy)
        {
                buoy->render(layer);
        }
        layer->setZValue(6);
        //return layer;  //TODO: ignore transformations
        return layer;
}

Layer* LayersLoader::loadNavaId()
{
    QList<Point> navaidList = chart.getChartPoints( "navaid_p", "SELECT * FROM navaid_p");
    QList<Point> lightList = chart.getChartPoints( "light", "SELECT * FROM light");

    Layer* layer = new Layer();
    QString objectid = "",navaidoid = "";
    bool hasLights = false;
    int count = 0,radarcount = 0,edgemark = 0;

    enum Offset{Center, BottomCenter};

    for(QList<Point>::iterator iterator = navaidList.begin();
    iterator != navaidList.end(); ++iterator ){

        objectid = iterator->get("objectid").toString();

        for(QList<Point>::iterator lightIterator = lightList.begin();
        lightIterator != lightList.end(); ++lightIterator ){
            navaidoid = lightIterator->get("navaidoid").toString();

            if(objectid == navaidoid){
                hasLights = true;
                break;
            }
        }

        if(!hasLights){
            int fi_typenav = iterator->get("fi_typenav").toInt();
            int ntypnavaid = iterator->get("ntypnavaid").toInt();
            QPointF position = iterator->getPoint();
            QString symbolPath = "";
            Offset offset = BottomCenter;
            QString topmark = ntypnavaidToString(ntypnavaid);

            if(fi_typenav == SPAR && topmark != ""){
                symbolPath = ":/symbols/spar_"+topmark+".png";
            }else if(fi_typenav == RADARTARGET){
                symbolPath = ":/symbols/radartarget.png";
            }else if(fi_typenav == FAYWAREMARK){
                symbolPath = ":/symbols/buoyX.png";
            }else if(fi_typenav == CAIRN){

                /*NOTE: data missing in this table attribute??? no value in fi_strcnav,
                using ntypnavaid for an educated guess*/

                if(ntypnavaid == 99)        //seems correct
                    symbolPath = ":/symbols/cairn.png";
                //else if(ntypnavaid == 0)    //unverified
                    //symbolPath = ":/symbols/levycairn.png";

                offset = Center;
            }else if(fi_typenav == EDGEMARK){
                symbolPath = ":/symbols/edgemark_"+topmark+".png";
            }else if(fi_typenav == BUOY && topmark != ""){
                symbolPath = ":/symbols/icebuyo_"+topmark+".png";
            }else if(fi_typenav == 6){
                int fi_strcnav = iterator->get("fi_strcnav").toInt();
                qDebug()<<"fi_strcnav"<<fi_strcnav;
                if(fi_strcnav == 13){
                    symbolPath = ":/symbols/church.png";
                    offset = Center;

                }
            }

            if(symbolPath != ""){
                QGraphicsPixmapItem* symbol = new QGraphicsPixmapItem(QPixmap(symbolPath));
                //qDebug()<<"symbolpath"<<symbolPath<<"symbol"<<symbol<<"position"<<position;
                symbol->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
                symbol->setPos(position.x(),position.y());
                symbol->scale(SCALE_FACTOR,SCALE_FACTOR);

                if(offset == BottomCenter)
                    symbol->setOffset(-symbol->boundingRect().width()/2,-symbol->boundingRect().height()*0.8);
                else if(offset == Center){
                    symbol->setOffset(-symbol->boundingRect().width()/2,-symbol->boundingRect().height()/2);
                }

                layer->addToGroup(symbol);
            }
        }
        hasLights = false;
    }
    layer->setZValue(6.5);
    return layer;
}

QString LayersLoader::ntypnavaidToString(int value){
    QString string = "";

    if(value == 1)
        string = "left";
    else if(value == 2)
        string = "right";
    else if(value == 3)
        string = "northern";
    else if(value == 4)
        string = "southern";
    else if(value == 5)
        string = "west";
    else if(value == 6)
        string = "east";
    else if(value == 7)
        string = "dangermark";
    else if(value == 8)
        string = "safewatermark";
    else if(value == 9)
        string = "specialmark";

    return string;
}
