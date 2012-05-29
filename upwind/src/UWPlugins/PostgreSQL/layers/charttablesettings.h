/**
 * @file charttablesettings.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef CHARTTABLESETTINGS_H
#define CHARTTABLESETTINGS_H

#include <QString>
#include "../shared/utils.h"
#include "../settings/SettingsManager.h"

/**
 * Settings used in automatic generation of chart table with ChartTableGen class.
 */
class ChartTableSettings {

public:
    ChartTableSettings();
	~ChartTableSettings();

	int originalZoomLevel();
	double scaleFactor();
	int simpLevelCount();
	int minZoom();
	double minZoomEpsilon();
	int maxZoom();
	double maxZoomEpsilon();

	void setDefaultValues();

	void setOriginalZoomLevel(int originalZoomLevel);
	void setScaleFactor(double scaleFactor);
	void setSimpLevelCount(int simpLevelCount);
	void setMinZoom(int minZoom);
	void setMinZoomEpsilon(double minZoomEpsilon);
	void setMaxZoom(int maxZoom);
	void setMaxZoomEpsilon(double maxZoomEpsilon);

	QString toString();

private:
	int m_originalZoomLevel;
	double m_scaleFactor;
	int m_simpLevelCount;
	int m_minZoom;
	double m_minZoomEpsilon;
	int m_maxZoom;
	double m_maxZoomEpsilon;

	SettingsManager *settingsManager;

};

#endif // CHARTTABLESETTINGS_H
