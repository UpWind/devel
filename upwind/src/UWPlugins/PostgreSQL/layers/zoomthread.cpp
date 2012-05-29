#include "zoomthread.h"
#include "layersimplificationhandler.h"
#include <QDebug>
#include "LayersManager.h"

ZoomThread::ZoomThread():zoomLevel(0)
{

}

void ZoomThread::setData(LayersManager* manager)
{
	this->manager = manager;
}

void ZoomThread::zoom()
{
	QHash<Layer::LayerType, Layer*>* drawnLayers = manager->getDrawnLayers();
	QHash<Layer::LayerType, LayerSimplificationHandler*> *handlers = manager->getHandlers();
	zoomLevel = manager->getZoom();
	qDebug() << "ZoomThread::zoom(): zoomLevel = " + QString::number(zoomLevel);
	if (zoomLevel>=150000)
	{
		if (!manager->isDrawingLines())
		{
			manager->lock();
			manager->setDrawingLines( true );
			manager->drawLineLayers();
			manager->unlock();
		}

	}
	if (zoomLevel>=270000)
	{
		if (!manager->isDrawingPoints())
		{
			manager->lock();
			manager->setDrawingPoints( true );
			manager->drawPointLayers();
			manager->unlock();
		}

	}


	if (zoomLevel<150000)
	{
		if (manager->isDrawingLines())
		{
			manager->lock();
			manager->setDrawingLines( false );
			manager->hideLineLayers();
			manager->unlock();
		}

	}
	if (zoomLevel<270000)
	{
		if (manager->isDrawingPoints())
		{
			manager->lock();
			manager->setDrawingPoints( false );
			manager->hidePointLayers();
			manager->unlock();
		}

	}
	manager->lock();
	QList<Layer::LayerType> keys = drawnLayers->keys();
	for(int i = 0; i< keys.size(); i++)
	{
		Layer::LayerType type = keys.at(i);

		if(handlers->contains(type))
		{
			Layer* layer = handlers->value(type)->getLayerForZoomLevel(zoomLevel);

			if(layer==NULL)
			{

				qDebug() << "layer was NULL!!! zoomLevel: " << zoomLevel << " chart: " << Layer::toString(type);
			}
			else
			{
				Layer* previousLayer = drawnLayers->value(type);
				if(layer!=previousLayer)
				{
					drawnLayers->insert(type, layer);
					emit this->removeLayer(previousLayer);
					emit this->addLayer(layer);
				}
			}
		}
	}
	manager->unlock();

}

void ZoomThread::run()
{
	do
	{
		zoom();
	} while(manager->getZoom()!=zoomLevel);

	emit this->zoomingReady(zoomLevel);
}

