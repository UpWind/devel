#include "layersimplificationhandler.h"
#include <QDebug>

// DEPRECATED: for line and polygon layers
LayerSimplificationHandler::LayerSimplificationHandler(QString name,  QHash<QString, Layer*> layers,
		QHash<QPair<int,int>, int> levels, bool simplified=true)
: optimized(simplified),chartName(name), layers(layers), modifier(1), zoomLevels(levels),
currentLevel(QPair<int, int>(0,0))
{
	this->debug = true;
	findMinAndMax();
}

// DEPRECATED: for point layers
LayerSimplificationHandler::LayerSimplificationHandler(QString name,  QHash<QString, Layer*> layers)
: optimized(false),chartName(name), layers(layers), modifier(1)
{
	this->debug = true;
	findMinAndMax();
}

// for queued loading of line and polygon layers
LayerSimplificationHandler::LayerSimplificationHandler(QString layerId,
		QHash<QPair<int, int>, int > zoomLevels, bool simplified=true)
: optimized(simplified), chartName(layerId), modifier(1), zoomLevels(zoomLevels)
{
	this->debug = true;
	findMinAndMax();
}

LayerSimplificationHandler::~LayerSimplificationHandler()
{
	QList<Layer*> l = layers.values();

	for(int i=0; i<l.size();i++)
	{
		delete l.at(i);
	}
}

void LayerSimplificationHandler::findMinAndMax()
{
	// if there is only one table, no zoomlevels is needed
	//if(!optimized)
	//{
	//	minOpLevel = 0;
	//	maxOpLevel = 0;
	//	minZoom = 0;
	//	maxZoom = 0;
	//	return;
	//}

	QHash<QPair<int,int>,int>::const_iterator i = zoomLevels.constBegin();
	minZoom = i.key().first;
	maxZoom = i.key().second;
	minOpLevel = maxOpLevel = i.value();
	++i;

	while(i != zoomLevels.constEnd())
	{
		if(minZoom > i.key().first)
		{
			minZoom = i.key().first;
			maxOpLevel = i.value();
		}
		if(maxZoom < i.key().second)
		{
			maxZoom = i.key().second;
			minOpLevel = i.value();
		}
		++i;
	}
}

QString LayerSimplificationHandler::getLayerName()
{
	return chartName;
}

Layer* LayerSimplificationHandler::modifierChanged(int zoom, int modifier)
{
	this->modifier = modifier;
	return getLayerForZoomLevel(zoom);
}

//void LayerSimplificationHandler::setOptimized(bool opt) {
//	this->optimized = opt;
//}

//void LayerSimplificationHandler::setZoomLevels(QHash<QPair<int, int>, int> levels) {
//	this->zoomLevels = levels;
//	this->findMinAndMax();
//}

void LayerSimplificationHandler::addLayerLevel(QString layerLevelName, Layer* layer) {
	this->layers.insert(layerLevelName, layer);
	this->loadedLayerLevels.insert(layerLevelName, true);
}

bool LayerSimplificationHandler::isLevelLoaded(QString layerLevelName) {
	return this->loadedLayerLevels.value(layerLevelName);
}

Layer* LayerSimplificationHandler::getLayerForZoomLevel(int zoom)
{
	Layer* returnLayer = NULL;
	zoom *= modifier;
	int simpLevel=0;

	QString tmp = "LayerSimplificationHandler::getLayerForZoomLevel(" +
			QString::number(zoom) + "), layer = "+this->chartName;

	//if(!optimized) {
	//	if (debug) qDebug() << tmp + ", not simplified";
	//	returnLayer = layers.begin().value();
	//	return returnLayer;
	//}

	if(currentLevel.first <= zoom && zoom <= currentLevel.second)
	{
		simpLevel = zoomLevels.value(currentLevel);
		returnLayer = getLayerWithSimplification(simpLevel);
	}
	else
	{
		if(zoom <= minZoom || maxZoom <= zoom)
		{
			if(zoom <= minZoom)
			{
				simpLevel = maxOpLevel;
				returnLayer = getLayerWithSimplification(simpLevel);
			}
			if(zoom >= maxZoom)
			{
				simpLevel = minOpLevel;
				returnLayer = getLayerWithSimplification(simpLevel);
			}

		}
		else
		{
			QList<QPair<int,int> > keys = zoomLevels.keys();
			for(int i=0; i<keys.size(); i++)
			{
				QPair<int,int> zoomlvl = keys.at(i);
				if(zoomlvl.first <= zoom && zoom <= zoomlvl.second)
				{
					currentLevel = zoomlvl;
					simpLevel = zoomLevels.value(zoomlvl);
					returnLayer = getLayerWithSimplification(simpLevel);
					break;
				}
			}
		}
	}
	if (debug) qDebug() << tmp + ", level = " + QString::number(simpLevel);
	return returnLayer;
}

Layer* LayerSimplificationHandler::getLayerWithSimplification(int simpLevel)
{
	Layer* returnLayer = NULL;
	QList<QString> keys = layers.keys();
	if(simpLevel == 0)
	{
		returnLayer = layers.value(chartName);
	}
	else
	{
		for(int i=0;i<keys.size();i++)
		{
			if(keys.at(i).endsWith(QString::number(simpLevel)))
			{
				returnLayer = layers.value(keys.at(i));
				break;
			}
		}
	}
	return returnLayer;
}

void LayerSimplificationHandler::printLayerNames() {
	QStringListIterator lit = QStringListIterator(layers.keys());
	while (lit.hasNext()) {
		QString layerId = lit.next();
		qDebug() << layerId;
	}
}
