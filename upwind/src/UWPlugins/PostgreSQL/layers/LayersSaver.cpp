
#include "LayersSaver.h"

#include <QDebug>

#include "ogrsf_frmts.h"

LayersSaver::LayersSaver(QVector<LayerVisualInfo*> chartTable, ChartTableSettings* chartTableSettings) {
	settingsManager = SettingsManager::getInstance();
	this->chartTable = chartTable;
	this->chartTableSettings = chartTableSettings;
	//loadChartTable();
}

LayersSaver::~LayersSaver() {
}

void LayersSaver::loadDriver() {
	if (settingsManager->getDBDriver() == "MySQL") {
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

void LayersSaver::run()
{
    saveLayers();
}

/*void LayersSaver::loadChartTable() {
	chartTable = new ChartTableGen();
	QStringList allLayersList;
	bool ok = true;

	// Loading directly from DB or XML can be added later,
	// but for now, we use default values when generating layersVisualInfo with ChartTable
	layersVisualInfo.empty();   // <-- For now, we use this one

	// Loading from XML file
	layersVisualInfo << chartTable->loadChartTableFromXML(settingsManager->getChartTableXMLFile(), &ok);


	// If there is no visual data coming from the DB or XML, generate them
	if (!ok || layersVisualInfo.isEmpty())
	{
		ChartTableSettings chartTableSettings;
		chartTableSettings.setDefaultValues();
		chartTable->setSettings(chartTableSettings);

		allLayersList << chart.getLayersForSimplification();

		//layersVisualInfo->clear();
		layersVisualInfo <<
				chartTable->generateLayerVisualInfoForAllTables(allLayersList, &ok);

		// Don't use XML saving yet --> it doesn't support threading at the moment
		//chartTable.saveChartTableToXML(layersVisualInfo,this->settingsManager->getDBName());
	}
}*/

void LayersSaver::setup() {
	this->totalNumberOfLayers = 0;
	this->layersOptimized = 0;
	this->optimLevels = chartTableSettings->simpLevelCount();
	this->selectedGeomTypes = "lr";
	this->appendLevelStr = "_level";

	QVector<QString *> layers = chart.getLayersNames();

	for( int i=0; i<layers.size(); ++i)
	{

		if( checkGeometryType( *layers.at(i) ) == 1 )
		{
			totalNumberOfLayers++;
		}
	}
	totalNumberOfLayers = totalNumberOfLayers*optimLevels;
}

void LayersSaver::optimize(OGRLayer *layer, OGRLayer *original_layer, qreal level) {
	OGROptimizer optimizer;
	optimizer.optimize(layer, original_layer, level, this);
	optimizer.run();
}

void LayersSaver::altOptimize( QString & layerId, QString &query, qreal level)
{
    mutex.lock();
    OGROptimizer optimizer;
    optimizer.altOptimize( layerId, query, level, this);
    optimizer.run();
}

SettingsManager * LayersSaver::getSettingsManager()
{
    return settingsManager;
}

void LayersSaver::saveLayers()
{
        QVector<QString *> layers  = chart.getLayersNames();

	// Runs through every layer that are selected to be optimized
	for( int i=0; i<layers.size(); ++i)
	{

		if( checkGeometryType( *layers.at(i) ) == 1 )
		{

			for( int j = 1; j<=optimLevels; ++j)
			{
                            mutex.lock();

				QString num;
				num.setNum(j);

				QString copyLayerName = *layers.at(i) + appendLevelStr + num;
                                //OGRLayer* copyLayer = chart.getLayerCopy( *layers.at(i), copyLayerName );
                                //OGRLayer* originalLayer = chart.getLayerFromDatabase( QString( "SELECT * FROM ") + *layers.at(i));

				qreal epsilon = getEpsilon(copyLayerName);
                                QList<QString> fields = chart.getFieldsForLayer( *layers.at(i));
                                QString query;
                                if ( fields.at( 0) == "*" || fields.contains("*") )
                                        query.append( "SELECT ");
                                else
                                        query.append( "SELECT OGC_FID, wkb_geometry, ");

                                for ( int z = 0; z < fields.size() - 1; z++ ) {
                                        query.append( fields.at( z));
                                        query.append( ", ");
                                }

                                query.append( fields.at( fields.size() - 1 ) );
                                query.append( " FROM " );
                                query.append( *layers.at(i));

                                //this->optimize(copyLayer, originalLayer, epsilon);
                                mutex.unlock();
                                this->altOptimize( copyLayerName, query, epsilon);

			}
		}
	}
}



void LayersSaver::optimizerReady()
{
mutex.unlock();
	layersOptimized++;
	qDebug() <<  totalNumberOfLayers << " , " << layersOptimized;

}

int LayersSaver::getOptimizationStatus()
{
	return totalNumberOfLayers - layersOptimized;
}

qreal LayersSaver::getEpsilon( QString layername )
{
	qreal epsilon       = 0.0;
	QVectorIterator<LayerVisualInfo*> i(chartTable);
	LayerVisualInfo tmp;

	while(i.hasNext() && epsilon == 0.0)
	{
		tmp = *i.next();
		if(tmp.getSrcTableName() == layername)
		{
			epsilon = (qreal)tmp.getEpsilon();
		}
	}

	return epsilon;
}

int LayersSaver::checkGeometryType( QString layerName )
{
	QString line = "_";

	if( layerName.size() > 0)
	{
		for( int i = 0; i < selectedGeomTypes.size(); i++ )
		{
			if( layerName.at(layerName.size()-2) == line.at(0) )
			{
				if( layerName.at(layerName.size()-1) == selectedGeomTypes.at(i) ) {
					return 1;
				}
			}
		}
	}
	return 0;
}
