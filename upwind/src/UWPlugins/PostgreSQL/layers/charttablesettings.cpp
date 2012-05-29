/**
 * @file charttablesettings.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "charttablesettings.h"

ChartTableSettings::ChartTableSettings() {
	settingsManager = SettingsManager::getInstance();
}

ChartTableSettings::~ChartTableSettings() {
}

void ChartTableSettings::setDefaultValues() {
	this->setOriginalZoomLevel(settingsManager->getInitialZoomLevel());
	this->setScaleFactor((double)(100 + settingsManager->getZoomFactor())/(double)100);
	this->setMinZoom(45000);
	this->setMaxZoom(1440000);
	this->setMinZoomEpsilon(0.002);
	this->setMaxZoomEpsilon(0.00005);
	this->setSimpLevelCount(6);
}

int ChartTableSettings::originalZoomLevel() {
	return this->m_originalZoomLevel;
}

double ChartTableSettings::scaleFactor() {
	return this->m_scaleFactor;
}

int ChartTableSettings::simpLevelCount() {
	return this->m_simpLevelCount;
}

int ChartTableSettings::minZoom() {
	return this->m_minZoom;
}

double ChartTableSettings::minZoomEpsilon() {
	return this->m_minZoomEpsilon;
}

int ChartTableSettings::maxZoom() {
	return this->m_maxZoom;
}

double ChartTableSettings::maxZoomEpsilon() {
	return this->m_maxZoomEpsilon;
}

void ChartTableSettings::setOriginalZoomLevel(int originalZoomLevel) {
	this->m_originalZoomLevel = originalZoomLevel;
}

void ChartTableSettings::setScaleFactor(double scaleFactor) {
	this->m_scaleFactor = scaleFactor;
}

void ChartTableSettings::setSimpLevelCount(int simpLevelCount) {
	this->m_simpLevelCount = simpLevelCount;
}

void ChartTableSettings::setMinZoom(int minZoom) {
	this->m_minZoom = minZoom;
}

void ChartTableSettings::setMinZoomEpsilon(double minZoomEpsilon) {
	this->m_minZoomEpsilon = minZoomEpsilon;
}

void ChartTableSettings::setMaxZoom(int maxZoom) {
	this->m_maxZoom = maxZoom;
}

void ChartTableSettings::setMaxZoomEpsilon(double maxZoomEpsilon) {
	this->m_maxZoomEpsilon = maxZoomEpsilon;
}

QString ChartTableSettings::toString() {
	QString tmp = "";
	tmp += "origZL: " + QString::number(this->m_originalZoomLevel) + ", ";
	tmp += "scaleF: " + clearZeroes(QString::number(this->m_scaleFactor, 'f')) + ", ";
	tmp += "levels: " + QString::number(this->m_simpLevelCount) + ", ";
	tmp += "minZ: " + QString::number(this->m_minZoom) + ", ";
	tmp += "minZE: " + clearZeroes(QString::number(this->m_minZoomEpsilon, 'f')) + ", ";
	tmp += "maxZ: " + QString::number(this->m_maxZoom) + ", ";
	tmp += "maxZE: " + clearZeroes(QString::number(this->m_maxZoomEpsilon, 'f'));
	return tmp;
}
