#include "LayersManager.h"
#include "threadfactory.h"
#include "zoomthread.h"
#include <QDebug>

LayersManager* LayersManager::instance = NULL;

LayersManager::LayersManager() {
	qDebug()<<Q_FUNC_INFO<<"Start";
	LayersManager::instance = this;
	handlers.clear();
	polygonLayers.clear();
	lineLayers.clear();
	pointLayers.clear();
	factory = ThreadFactory::getInstance();
	settingsManager = SettingsManager::getInstance();
	drawingLines = false;
	drawingPoints = false;

	QStringList simpTableList;
	bool ok = true;
	QStringList nonSimpTableList;
	QVector<LayerVisualInfo*> nonSimpLVIVector;

	QString qtSqlDriverStr;
	//TODO: These are here just for testing.
	settingsManager->setDBDriver("PostgreSQL");
	settingsManager->setDBHost("localhost");
	settingsManager->setDBPort("5432");
	settingsManager->setDBName("chart54");
	settingsManager->setDBUser("postgres");
	settingsManager->setDBPass("postgres");

	qDebug()<<Q_FUNC_INFO<<settingsManager->getDBDriver();
	if (settingsManager->getDBDriver() == "MySQL") {
		qtSqlDriverStr = "QMYSQL";
	} else if (settingsManager->getDBDriver() == "PostgreSQL") {
		qtSqlDriverStr = "QPSQL";
	}
	chart.setOGRDriverQt(qtSqlDriverStr);

	// get layerVisualInfo for polygon and line layers

	layerVisualInfoVector.empty();
	layerVisualInfoVector = chartTableGen.loadChartTableFromXML(settingsManager->getChartTableXMLFile(), &ok);

	if (true) {
		// Special cases
		// obstacles layer
		LayerVisualInfo	* tmpLVI = new LayerVisualInfo();
		tmpLVI->setLayerId("obstacles");
		tmpLVI->setZoomMin(0);
		tmpLVI->setZoomMax(2147483647);
		tmpLVI->setStyleId(0);
		tmpLVI->setSimpLevel(0);
		tmpLVI->setEpsilon(0);
		tmpLVI->setSrcTableName("obstacles");
		layerVisualInfoVector.append(tmpLVI);

	}


	chartTableSettings.setDefaultValues();

	// If there is no visual data coming from XML, generate it
	if (!ok || layerVisualInfoVector.isEmpty()) {
		qDebug() << "No valid chart table could be loaded for line and polygon layers, generating default table";
		this->chartTableGen.setSettings(chartTableSettings);
		simpTableList = chart.getSimplifiedLayers();
		layerVisualInfoVector = chartTableGen.generateLayerVisualInfoForAllTables(simpTableList, &ok);

		// Don't use XML saving yet --> it doesn't support threading at the moment
		//chartTable.saveChartTableToXML(layersVisualInfo,this->settingsManager->getDBName());
	}

	// create and add layerVisualInfo for point layers

	// TODO: chartTableGen sets minZoomLevel to 0 -- not desired in this situation

	nonSimpTableList = chart.getNonSimplifiedLayers();
	QListIterator<QString> it(nonSimpTableList);
	while (it.hasNext()) {
		QString layerId = it.next();
		LayerVisualInfo* lvi = new LayerVisualInfo();
		lvi->setLayerId(layerId);
		lvi->setSimpLevel(0);
		lvi->setZoomMax((double)LONG_MAX);
		lvi->setZoomMin(270000);	// see zoomThread for value
		lvi->setStyleId(0);
		lvi->setEpsilon(0);
		lvi->setSrcTableName(layerId); // same than layerId
		//qDebug() << lvi->toString();
		nonSimpLVIVector.append(lvi);
	}
	QVectorIterator<LayerVisualInfo*> vi(nonSimpLVIVector);
	while (vi.hasNext()) {
		LayerVisualInfo* lvi = vi.next();
		layerVisualInfoVector.append(lvi);
		//qDebug() << lvi->toString();
	}

	// TODO: cut line layers visibility from minZoom side... see zoomThread for values

	chartTable.setLayerVisualInfoVector(layerVisualInfoVector);

	layersSaver = new LayersSaver(chartTable.getLayerVisualInfoVector(), &chartTableSettings);
	layersSaver->loadDriver();

	zoomLevel = chartTableSettings.originalZoomLevel();

	queuedLoadingDebug = true;
	qDebug()<<Q_FUNC_INFO<<"End";
}

void LayersManager::lock()
{
	mutex.lock();
}

void LayersManager::unlock()
{
	mutex.unlock();
}

LayersManager::~LayersManager()
{
	if(factory)
		delete factory;
}


int LayersManager::getOptimizationStatus() {
	return layersSaver->getOptimizationStatus();
}


void LayersManager::setScene(QGraphicsScene* scene) {
	this->scene = scene;
}

LayersManager* LayersManager::getInstance() {
	if(instance == NULL) {
		instance = new LayersManager();
		return instance;
	}
	return instance;
}

void LayersManager::optimizeLayers() {
	qDebug() << "optimize layers called";
	layersSaver->setup();
	layersSaver->start();
}

void LayersManager::cancelBackgroundOperations() {
	factory->terminateAndDeleteAllThreads();
	mutex.unlock();
}

void LayersManager::loadLayer(Layer::LayerType tLayerId) {
	mutex.lock();
	QString layerId = Layer::toString(tLayerId);
	if (!handlers.contains(tLayerId)) {

		LayersLoader* loader = factory->getLayerLoader(tLayerId, layerVisualInfoVector, &chartTableSettings);

		// Add currently visible layer level into the primary load queue.
		// If there is no visible layers, getSimpLevelForZoom returns -1.
		// It should be added to primaryLoadQueue anyway so that loader can do the
		// initialization stuff for a layer.
		// The layers which are currently not visible are loaded in secondary queue.
		QVector<int> priLoadLevels;
		int currentSimpLevel = chartTable.getSimpLevelForZoom(layerId, this->getZoom());
		qDebug() << "LayersManager::loadLayer: layerId = "+layerId+
					", currentSimpLevel = "+QString::number(currentSimpLevel);
		priLoadLevels.insert(priLoadLevels.begin(), currentSimpLevel);
		this->primaryLoadQueue.insert(layerId, priLoadLevels);

		// add the rest of the layer levels into the secondary load queue
		// if there are any levels
		QVector<int> secLoadLevels = chartTable.getLevels(layerId);
		QMutableVectorIterator<int> it(secLoadLevels);
		int i;
		while (it.hasNext()) {
			i = it.next();
			if (i == currentSimpLevel ) it.remove();
		}
		if (!secLoadLevels.isEmpty()) {
			this->secondaryLoadQueue.insert(layerId, secLoadLevels);
		}

		// first only load the primary queue content.
		// This also creates the handler for the layer.
		loader->addToLoadQueue(layerId, priLoadLevels);

		// get a simplificationHandler if exists
		LayerSimplificationHandler * handler = NULL;
		if (this->handlers.contains(tLayerId)) {
			handler = this->handlers.value(tLayerId);
		}
		loader->setLayerSimplificationHandler(handler);

		if(!loader->isRunning()) {
			QObject::disconnect(loader, SIGNAL(loadingReady(LayerSimplificationHandler*)),
								this, SLOT(onLoadingReady(LayerSimplificationHandler*)));
			QObject::connect(loader, SIGNAL(loadingReady(LayerSimplificationHandler*)),
							 this, SLOT(onLoadingReady(LayerSimplificationHandler*)));
			loader->start();
			emit this->loadingLayer(layerId);
		}
	}
	mutex.unlock();
}

// TODO: if zoom changes during loading, the behaviour is not very well defined :S
void LayersManager::onLoadingReady(LayerSimplificationHandler* handler) {
	mutex.lock();
	if (queuedLoadingDebug) qDebug() << "LayersManager mutex lock";
	// if handler exists this was second time
	bool handlerExists = false;
	if (handler != NULL) {
		QString loadedLayerId = handler->getLayerName();
		Layer::LayerType type = Layer::fromString(loadedLayerId);
		if (queuedLoadingDebug) qDebug() << "LayersManager.onLoadingReady("+loadedLayerId+")";

		if (handlers.contains(type)) {
			handlerExists = true;
		}

		// insert/update handler

		handlers.insert(type, handler);

		// draw current layer

		Layer* layer = handler->getLayerForZoomLevel(zoomLevel);
		if (layer != NULL) {
			if ((type == Layer::generarea_r) ||(type == Layer::deptharea_r)
					|| (type == Layer::spoilgrnd_r) || (type == Layer::limit_r)
					|| (type == Layer::builtupare_r) || (type == Layer::fwayarea_r)
					|| (type == Layer::survqual_r) || (type == Layer::building_r)
					|| (type == Layer::dock_r) || (type == Layer::lake_r)
					|| (type == Layer::obstacles) )
			{
				polygonLayers.insert(type, layer);
				drawnLayers.insert(type, layer);

				if (!handlerExists) {
					scene->addItem(layer);
					qDebug() << "LayersManager::addNewHandler(): polygon layer loaded: "
							 <<  Layer::toString(type);
				}
			}
			else if ((type == Layer::depthcont_l) || (type == Layer::coastline_l)
					 || (type == Layer::closinglne_l) || (type == Layer::navigline_l)
					 || (type == Layer::ferry_l) || (type == Layer::limit_l)
					 || (type == Layer::heightcont_l) || (type == Layer::leadinglne_l)
					 || (type == Layer::lockndam_l) || (type == Layer::pipeline_l)
					 || (type == Layer::submcable_l) || (type == Layer::roadnrailw_l)
					 || (type == Layer::bridge_l) || (type == Layer::transmline_l)
					 || (type == Layer::river_l) || (type == Layer::shlnecon_l))
			{
				lineLayers.insert(type, layer);
				if (drawingLines && !handlerExists) {
					drawnLayers.insert(type, layer);
					scene->addItem(layer);
				}
				qDebug() << "LayersManager::addNewHandler(): line layer loaded: " << loadedLayerId;
			}
			else {
				pointLayers.insert(type, layer);
				if (drawingPoints && !handlerExists) {
					drawnLayers.insert(type, layer);
					if(Layer::toString(type) != "signsound_p")
						scaleSymbols(type);//TODO: maybe skip scaling on some layers
					scene->addItem(layer);
				}
				qDebug() << "LayersManager::addNewHandler(): point layer loaded: " << loadedLayerId;
			}
			if (!handlerExists) {
				scene->update();
			}
		} else {
			qDebug() << "LayersManager::addNewHandler(): NULL layer for current zoom level";
		}

		// process load queue

		if (this->primaryLoadQueue.contains(loadedLayerId)) {
			this->primaryLoadQueue.take(loadedLayerId);
		}

		if (this->primaryLoadQueue.isEmpty()) {
			if (queuedLoadingDebug) qDebug() << "LayersManager: primaryLoadQueue is empty";

			if (this->secondaryLoadQueue.isEmpty()) {
				if (queuedLoadingDebug) qDebug() <<
													"LayersManager: secondaryLoadQueue is also empty, loading is DONE";
			}
			else {
				QString layerToLoad;
				QVector<int> levels;

				if (this->secondaryLoadQueue.contains(loadedLayerId)) {
					layerToLoad = loadedLayerId;
				} else {
					// was already loaded, pop the next layer
					layerToLoad = this->secondaryLoadQueue.begin().key();
				}
				levels = this->secondaryLoadQueue.take(layerToLoad);
				Layer::LayerType tLayerId = Layer::fromString(layerToLoad);

				if (!levels.isEmpty()) {
					LayersLoader* secLoader = factory->getLayerLoader(tLayerId,
																	  this->layerVisualInfoVector, &(this->chartTableSettings));
					secLoader->addToLoadQueue(layerToLoad, levels);
					// add handler to be updated
					LayerSimplificationHandler* handler = this->handlers.value(tLayerId);
					secLoader->setLayerSimplificationHandler(handler);
					// poll loader termination
					// TODO: do this better :X
					while (secLoader->isRunning()) {
						// do nothing...
					}
					if (!secLoader->isRunning()) {
						if (queuedLoadingDebug) qDebug() << "secLoader.start";
						QObject::disconnect(secLoader, SIGNAL(loadingReady(LayerSimplificationHandler*)),
											this, SLOT(onLoadingReady(LayerSimplificationHandler*)));
						QObject::connect(secLoader, SIGNAL(loadingReady(LayerSimplificationHandler*)),
										 this, SLOT(onLoadingReady(LayerSimplificationHandler*)));
						secLoader->start();
						emit this->loadingLayer(layerToLoad);
					} else {
						if (queuedLoadingDebug) qDebug() << "secLoader NOT STARTED";
					}
				}
			}
		} else {
			// If the primary queue is not empty, the loaders just continue loading and
			// will come to this method after done.
			if (queuedLoadingDebug) qDebug() <<
												"LayersManager: primaryLoadQueue is not yet empty, continuing with primaryLoadQueue...";
		}
	}
	else {
		qDebug() << "onLoadingReady: NULL handler returned";
	}
	if (queuedLoadingDebug) qDebug() << "LayersManager mutex unlock";
	mutex.unlock();
	if (!handlerExists) {
		emit this->sceneChanged(handler->getLayerName());
	}
	emit this->chartLoaded(handler->getLayerName()); // also zoomingReady sends this
}

void LayersManager::unLoadLayer(Layer::LayerType type)
{
	mutex.lock();
	if (handlers.contains(type))
	{
		LayerSimplificationHandler* handler = handlers.take(type);
		if	((type == Layer::generarea_r) || (type == Layer::deptharea_r)
			 || (type == Layer::spoilgrnd_r) || (type == Layer::limit_r)
			 || (type == Layer::builtupare_r) || (type == Layer::fwayarea_r)
			 || (type == Layer::survqual_r) || (type == Layer::building_r)
			 || (type == Layer::dock_r) || (type == Layer::lake_r)
			 || (type == Layer::obstacles) )
		{
			polygonLayers.remove(type);   //TODO: can be done much better, hash?
			qDebug() << "polygon layer deleted: " << Layer::toString(type);

		}

		else if ((type == Layer::depthcont_l) || (type == Layer::coastline_l)
				 || (type == Layer::closinglne_l) || (type == Layer::navigline_l)
				 || (type == Layer::ferry_l) || (type == Layer::limit_l)
				 || (type == Layer::heightcont_l) || (type == Layer::leadinglne_l)
				 || (type == Layer::lockndam_l) || (type == Layer::pipeline_l)
				 || (type == Layer::submcable_l) || (type == Layer::roadnrailw_l)
				 || (type == Layer::bridge_l) || (type == Layer::transmline_l)
				 || (type == Layer::river_l) || (type == Layer::shlnecon_l))
		{
			lineLayers.remove(type);
			qDebug() << "line layer deleted: " << Layer::toString(type);
		}

		else
		{
			pointLayers.remove(type);
			qDebug() << "point layer deleted: " << Layer::toString(type);
		}


		if (drawnLayers.contains(type))
		{
			scene->removeItem(drawnLayers.value(type));
			drawnLayers.remove(type);
			qDebug() << "removing layer from drawn list: " << Layer::toString(type);
		}
		delete handler;

	}
	mutex.unlock();

}

void LayersManager::drawPolygonLayers() {
	QHash<Layer::LayerType, Layer*>::const_iterator i = polygonLayers.constBegin();
	while (i != polygonLayers.constEnd()) {
		Layer* layer = handlers.value(i.key())->getLayerForZoomLevel(zoomLevel);
		scene->addItem(layer);
		drawnLayers.insert(i.key(), layer);
		++i;
	}
}

void LayersManager::hidePolygonLayers() {
	QHash<Layer::LayerType, Layer *>::const_iterator i = polygonLayers.constBegin();
	while (i != polygonLayers.constEnd()) {
		scene->removeItem(drawnLayers.value(i.key()));
		drawnLayers.remove(i.key());
		++i;
	}
}

void LayersManager::drawLineLayers() {
	QHash<Layer::LayerType, Layer*>::const_iterator i = lineLayers.constBegin();
	while (i != lineLayers.constEnd()) {
		Layer* layer = handlers.value(i.key())->getLayerForZoomLevel(zoomLevel);
		scene->addItem(layer);
		drawnLayers.insert(i.key(), layer);
		++i;
	}
}

void LayersManager::hideLineLayers() {
	QHash<Layer::LayerType, Layer*>::const_iterator i = lineLayers.constBegin();
	while (i != lineLayers.constEnd()) {
		scene->removeItem(drawnLayers.value(i.key()));
		drawnLayers.remove(i.key());
		++i;
	}
}



void LayersManager::drawPointLayers() {
	QRectF rect = scene->sceneRect();

	QHash<Layer::LayerType, Layer*>::const_iterator i = pointLayers.constBegin();
	while (i != pointLayers.constEnd()) {
		scene->addItem(i.value());
		drawnLayers.insert(i.key(), i.value());
		++i;
	}
	scene->setSceneRect(rect); //bug, some points try to draw themselves outside and the scene rect is changed so i have to save the scenerect
}

void LayersManager::hidePointLayers() {
	QHash<Layer::LayerType, Layer *>::const_iterator i = pointLayers.constBegin();
	while (i != pointLayers.constEnd()) {
		scene->removeItem(i.value());
		drawnLayers.remove(i.key());
		++i;
	}
}

void LayersManager::zoomingReady(int zoom) {
	zoom = 0;
	emit chartLoaded("zoomLevels");
}

void LayersManager::addToScene(Layer *layer) {
	scene->addItem(layer);
}

void LayersManager::removeFromScene(Layer *layer) {
	scene->removeItem(layer);
}

void LayersManager::setZoom(int level) {
	zoomLevel= level;

	if(factory->isZoomThreadAvailable())
	{
		ZoomThread* zoomer = factory->getZoomThread(this);
		zoomer->start();
		emit loadingLayer("zoomLevels");
	}
}

//TODO: add layers which are not to be scaled
/**
 *Scales all elements that do not inherit transformations
 *in drawn pointlayers.
 */
void LayersManager::scaleSymbols()
{
    qreal scaleFactor = getScaleFactor();
    QHash<Layer::LayerType, Layer*>::const_iterator i = pointLayers.constBegin();

    while(i != pointLayers.constEnd())
    {
        if(drawnLayers.contains(i.key()) && Layer::toString(i.key()) != "signsound_p"
                && Layer::toString(i.key()) != "ctext_p") //TODO: perhaps use different scale
        {                                                                      //instead of skip
            Layer* lights = drawnLayers.value(i.key());
            qDebug()<<"scale layer"<<Layer::toString(i.key());
            QList<QGraphicsItem *> children = lights->childItems();
            QList<QGraphicsItem *>::iterator k = children.begin();

            while(k != children.end())
            {
                QGraphicsItem* temp = *k;
                QGraphicsItem::GraphicsItemFlags flags = temp->flags();
                if(flags.testFlag(QGraphicsItem::ItemIgnoresTransformations))
                    temp->setScale(scaleFactor);
                ++k;
            }
        }
        ++i;
    }
}

/**
 *Scales all elements that do not inherit transformations
 *in given layer.
 *@param type layer to be scaled
 */
void LayersManager::scaleSymbols(Layer::LayerType type)
{

    qDebug()<<"scaletest type"<<Layer::toString(type);

    if(drawnLayers.contains(type))
    {
        Layer* lights = drawnLayers.value(type);

        qreal scaleFactor = getScaleFactor();
        QList<QGraphicsItem *> children = lights->childItems();
        QList<QGraphicsItem *>::iterator k = children.begin();

        while(k != children.end())
        {
            QGraphicsItem* temp = *k;
            //qDebug()<<"temp ignores"<<temp->ItemIgnoresTransformations;
            QGraphicsItem::GraphicsItemFlags flags = temp->flags();
            if(flags.testFlag(QGraphicsItem::ItemIgnoresTransformations))
                temp->setScale(scaleFactor);
            ++k;
        }
    }
}

/**
 *Calculates scaling factor from zoom level.
 */
qreal LayersManager::getScaleFactor() {
    qreal scaleFactor = (double)zoomLevel / (double)500000;
    qDebug() << "zoomlevel:" << zoomLevel << "scalefactor:" << scaleFactor;
    if(scaleFactor < 1.0)
        scaleFactor = 1.0;
    else if( scaleFactor > 10.0)
        scaleFactor = 10.0;

    return scaleFactor;
}

//TODO:scaling on zoomout, zoomout not working at the moment



