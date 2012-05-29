#include "ChartTable.h"

ChartTable::ChartTable() {

}

ChartTable::~ChartTable() {
	this->layerVisualInfoVector.clear();
}

void ChartTable::setLayerVisualInfoVector(QVector<LayerVisualInfo*> lviVector) {
	this->layerVisualInfoVector = lviVector;
}

QVector<LayerVisualInfo*> ChartTable::getLayerVisualInfoVector() {
	return this->layerVisualInfoVector;
}

int ChartTable::getSimpLevelCount(QString layerId) {
	if ( layerId == "lake_r" ) layerId = "dephtarea_r";
	QVectorIterator<LayerVisualInfo*> vi(this->layerVisualInfoVector);
	int count = 0;
	while (vi.hasNext()) {
		LayerVisualInfo* lvi = vi.next();
		if (lvi->getLayerId() == layerId) count++;
	}
	return count;
}

QString ChartTable::getLayerLevelName(QString layerId, int level) {
	if (layerId == "lake_r" ) layerId = "deptharea_r";
	QVectorIterator<LayerVisualInfo*> vi(this->layerVisualInfoVector);
	while (vi.hasNext()) {
		LayerVisualInfo* lvi = vi.next();
		if (lvi->getLayerId() == layerId && lvi->getSimpLevel() == level) {
			return lvi->getSrcTableName();
		}
	}
}


int ChartTable::getSimpLevelForZoom(QString layerId, int zoomLevel) {
	if (layerId == "lake_r" ) layerId = "deptharea_r";
	QVectorIterator<LayerVisualInfo*> vi(this->layerVisualInfoVector);
	while (vi.hasNext()) {
		LayerVisualInfo* lvi = vi.next();
		if (lvi->getLayerId() == layerId &&
		  zoomLevel >= lvi->getZoomMin() && zoomLevel <= lvi->getZoomMax()) {
			return lvi->getSimpLevel();
		}
	}
	return -1;
}

QVector<int> ChartTable::getLevels(QString layerId) {
	if (layerId == "lake_r" ) layerId = "deptharea_r";
	QVector<int> v;
	QVectorIterator<LayerVisualInfo*> vi(this->layerVisualInfoVector);
	while (vi.hasNext()) {
		LayerVisualInfo* lvi = vi.next();
		if (lvi->getLayerId() == layerId) {
			v.append(lvi->getSimpLevel());
		}
	}
	return v;
}
