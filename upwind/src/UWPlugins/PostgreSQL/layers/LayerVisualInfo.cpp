/**
 * @file LayerVisualInfo.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "LayerVisualInfo.h"

LayerVisualInfo::LayerVisualInfo() {
	layerId = "";
	zoomMin = 0;
	zoomMax = 0;
	styleId = 0;
	simpLevel = 0;
	epsilon = 0.0;
	srcTableName = "";
}

LayerVisualInfo::LayerVisualInfo( const LayerVisualInfo &other) {
	layerId = other.layerId;
	zoomMin = other.zoomMin;
	zoomMax = other.zoomMax;
	styleId = other.styleId;
	simpLevel = other.simpLevel;
	epsilon = other.epsilon;
	srcTableName = other.srcTableName;
}

QString LayerVisualInfo::getLayerId() {
	return this->layerId;
}

int LayerVisualInfo::getZoomMin() {
	return zoomMin;
}

int LayerVisualInfo::getZoomMax() {
	return zoomMax;
}

int LayerVisualInfo::getStyleId() {
	return styleId;
}

int LayerVisualInfo::getSimpLevel() {
	return simpLevel;
}

double LayerVisualInfo::getEpsilon() {
	return epsilon;
}

QString LayerVisualInfo::getSrcTableName() {
	return srcTableName;
}

void LayerVisualInfo::setLayerId(QString layerId) {
	this->layerId = layerId;
}

void LayerVisualInfo::setZoomMin(int zoomMin) {
	this->zoomMin = zoomMin;
}

void LayerVisualInfo::setZoomMax(int zoomMax) {
	this->zoomMax = zoomMax;
}

void LayerVisualInfo::setStyleId(int styleId) {
	this->styleId = styleId;
}

void LayerVisualInfo::setSimpLevel(int simpLevel) {
	this->simpLevel = simpLevel;
}

void LayerVisualInfo::setEpsilon(double epsilon) {
	this->epsilon = epsilon;
}

void LayerVisualInfo::setSrcTableName(QString srcTableName) {
	this->srcTableName = srcTableName;
}

QString LayerVisualInfo::toString() {
	return QString(layerId + " " + QString::number(zoomMin) + " " + QString::number(zoomMax) +
				   " " + QString::number(styleId) + " " + QString::number(simpLevel) +
				   " " + QString::number(epsilon) + " " + srcTableName);
}
