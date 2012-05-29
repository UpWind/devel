/**
 * @file ChartTable.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "ChartTableGen.h"

ChartTableGen::ChartTableGen() {
	//settingsManager = SettingsManager::getInstance();

	// If you want to modify ChartTable to use database connections as MySQL or PostgreSQL
	// Current db connection is working MySQL only (methods which are commented)

	/*if (settingsManager->getDBDriver() == "MySQL") {
		db = QSqlDatabase::addDatabase("QMYSQL");
	} else if (settingsManager->getDBDriver() == "PostgreSQL") {
		db = QSqlDatabase::addDatabase("QPSQL");
	}
	db.setHostName(settingsManager->getDBHost());
	db.setPort((settingsManager->getDBPort()).toInt());
	db.setDatabaseName(settingsManager->getDBName());
	db.setUserName(settingsManager->getDBUser());
	db.setPassword(settingsManager->getDBPass());
	db.open();
	if (!db.isOpen()) {
		qDebug() << "Error opening database. " << db.lastError().text();
		qDebug() << "Continuing without database access.";
	}*/

}

ChartTableGen::~ChartTableGen() {
	//if (db.isOpen()) { db.close(); }
}

QVector<LayerVisualInfo*> ChartTableGen::generateLayerVisualInfoForLayer(QString layerId) {
	QVector<LayerVisualInfo*> tmpLVI;
	int i=0;	// level number
	LayerVisualInfo* tmpInfo;
	QString simpStr = "_level";
	double e=0;
	double maxZoom=0;
	double minZoom=0;
	QString srcTableName;

	for (i = 0; i <= this->getSimpLevelCount(); i++) {
		tmpInfo = new LayerVisualInfo();

		if (this->getSimpLevelCount() > 0) {
			e = this->getEpsilonForLevel(i);
			maxZoom = this->getZoomForLevel(i);
		}

		if (i == 0) {
			// level 0: no simplification, maximum zoom level (= max zoom in)
			if (this->getSimpLevelCount()==0) {
				// and if no other layers, ensure the whole zoom range cover
				minZoom = 0;
			} else {
				minZoom = getMaxZoomLevel();
			}
			maxZoom = (double)LONG_MAX; // --> 2147483647
		} else if (i == this->getSimpLevelCount()) {
			// level = max: heaviest simplification, minimum zoom level (= max zoom out)
			maxZoom = minZoom;
			minZoom = 0;
		} else {
			minZoom = this->getZoomForLevel(i + 1);
		}

		if (i == 0) {
			srcTableName = layerId;		// level 0: original data set with no simplification
		} else {
			srcTableName = layerId + simpStr + QString::number(i);
		}

		tmpInfo->setLayerId(layerId);
		tmpInfo->setZoomMin(minZoom);
		tmpInfo->setZoomMax(maxZoom);
		tmpInfo->setStyleId(0);
		tmpInfo->setSimpLevel(i);
		tmpInfo->setEpsilon(e);
		tmpInfo->setSrcTableName(srcTableName);

		tmpLVI << tmpInfo;
	} // for
	return tmpLVI;
}

QVector<LayerVisualInfo*> ChartTableGen::generateLayerVisualInfoForAllTables(QStringList layerList, bool *ok) {
	QVector<LayerVisualInfo*> tmpLVI;
	QList<QString> tableNames;
	// TODO: convert settings manager driver Qt-compatible
	//QSqlDatabase db = QSqlDatabase::addDatabase(settingsManager->getDBDriver());

	//QString queryStr;
	//QSqlQuery query;
	//QSqlError queryErr;
	//int rowCount=0;
	//QString res;
	*ok = false;

	// TODO: first check whether the chart table contains some data -> fail
	//QString chartTableName = settingsManager->getDBChartTableName();
	//queryStr = "SELECT * FROM " + chartTableName + ";";
	//qDebug() << queryStr;
	//query = db.exec(queryStr);
	//rowCount = query.size();
	//if (rowCount > 0) {
		//qDebug() << chartTableName << " contains data";
		//*ok = false;
		//return tmpLVI;
	//}
	//query.finish();

	/*queryStr = "SHOW TABLES;";
	query = db.exec(queryStr);
	rowCount = query.size();
	//qDebug() << QString::number(rowCount) << " tables in the database";
	while (query.next()) {
		QString name = query.value(0).toString();
		if (name.endsWith("_l") || name.endsWith("_r")) {
			tableNames << name;
		} else {
			//qDebug() << "rejected: " << name;
		}
	}
	query.finish();*/

	//qDebug() << QString::number(tableNames.size()) << " tables to be simplified";

	QListIterator<QString> tablesIt(layerList);


	while (tablesIt.hasNext()) {
		QString layerId = tablesIt.next();
		tmpLVI << this->generateLayerVisualInfoForLayer(layerId);
	}
	*ok = true;
	return tmpLVI;
}

/*bool ChartTable::saveChartTableToDB(QVector<LayerVisualInfo*> data, QString tableName) {
	QVectorIterator<LayerVisualInfo*> lviIt(data);
	LayerVisualInfo* tmpLVI;
	QString queryStr;
	QSqlQuery query;
	QString table = tableName;
	int rowCount = 0;
	int rowId = 0;
	bool ok;

	// return if no table name specified anywhere
	if (table == "") {
		//table = settingsManager->getDBChartTableName();
		table = settingsManager->getDBName();
		if (table == "") {
			return false;
		}
	}

	// first check whether the target table exists by trying to count the rows
	ok = query.exec("SELECT COUNT(*) FROM " + table + ";");
	query.next();
	if (query.isValid()) {
		rowCount = query.value(0).toInt();
		//qDebug() << "rowcount = " << rowCount;
	}

	// if row counting didn't work then maybe the table doesn't exist -> attempt to create it
	if (!ok) {
		if (db.driverName() == "QMYSQL") {
			queryStr = "CREATE TABLE " + table + " ("
				   "id int not null primary key auto_increment, " +
				   "layer_id varchar(1024), " +
				   "zoom_min double, " +
				   "zoom_max double, " +
				   "style_id int, " +
				   "simp_level int, " +
				   "epsilon double, " +
				   "src_table_name varchar(1024));";
		}
		ok = query.exec(queryStr);
		// fail if couldn't create chart table
		if (!ok) {
			return false;
		}
	}

	// if there is old stuff in the table, remove all
	if (rowCount > 0) {;
		ok = query.exec("DELETE FROM " + table + ";");
		if (!ok) {
			return false;
		}
	}

	// save LVI's into the database
	rowId = 1;
	while (lviIt.hasNext()) {
		tmpLVI = lviIt.next();
		queryStr = "INSERT INTO " + table + " VALUES ('" +
				   QString::number(rowId) + "', '" +
				   tmpLVI->getLayerId() + "', '" +
				   QString::number(tmpLVI->getZoomMin()) + "', '" +
				   QString::number(tmpLVI->getZoomMax()) + "', '" +
				   QString::number(tmpLVI->getStyleId()) + "', '" +
				   QString::number(tmpLVI->getSimpLevel()) + "', '" +
				   QString::number(tmpLVI->getEpsilon()) + "', '" +
				   tmpLVI->getSrcTableName() + "');";
		//qDebug() << queryStr;
		ok = query.exec(queryStr);
		if (!ok) {
			qDebug() << "SQL query failed: " << query.lastError().text();
			return false;
		}
		rowId++;
	}
	return true;
}*/

/*QVector<LayerVisualInfo*> ChartTable::loadChartTableFromDB(QString tableName, bool *ok) {
	QVector<LayerVisualInfo*> tmpLVI;
	QString table = tableName;

	*ok = false;

	// if no table name defined, try using the table name from settings
	if (table == "") {
		//table = settingsManager->getDBChartTableName();
		table = settingsManager->getDBName();
		// if not defined in settings, quit
		if (table == "") {
			*ok = false;
			return tmpLVI;
		}
	}
	bool qok;
	QString queryStr = "";
	queryStr = "SELECT id, layer_id, zoom_min, zoom_max, style_id, simp_level, epsilon, src_table_name FROM " +
							table + ";";
	QSqlQuery query;
	qok = query.exec(queryStr);
	if (!qok) {
		*ok = false;
		return tmpLVI;
	}
	// add recently read LVI's to the vector
	while (query.next()) {
		if (!query.isValid()) {
			tmpLVI.clear();
			return tmpLVI;
		}
		LayerVisualInfo* lvi = new LayerVisualInfo();
		// value(0) = id
		lvi->setLayerId(query.value(1).toString());
		lvi->setZoomMin(query.value(2).toDouble());
		lvi->setZoomMax(query.value(3).toDouble());
		lvi->setStyleId(query.value(4).toInt());
		lvi->setSimpLevel(query.value(5).toInt());
		lvi->setEpsilon(query.value(6).toDouble());
		lvi->setSrcTableName(query.value(7).toString());

		//qDebug() << lvi->toString();

		tmpLVI << lvi;
	}
	*ok = true;
	return tmpLVI;
}*/

void ChartTableGen::setOriginalZoomLevel(double originalZoomLevel) {
	this->originalZoomLevel = originalZoomLevel;
}

double ChartTableGen::getOriginalZoomLevel() {
	return this->originalZoomLevel;
}

void ChartTableGen::setScaleFactor(double scaleFactor) {
	this->scaleFactor = scaleFactor;
}

double ChartTableGen::getScaleFactor() {
	return this->scaleFactor;
}

void ChartTableGen::setSimpLevelCount(int count) {
	this->simpLevelCount = count;
}

int ChartTableGen::getSimpLevelCount() {
	return this->simpLevelCount;
}

void ChartTableGen::setMinZoomLevel(double minZoomLevel) {
	this->minZoomLevel = minZoomLevel;
}

void ChartTableGen::setMaxZoomLevel(double maxZoomLevel) {
	this->maxZoomLevel = maxZoomLevel;
}

void ChartTableGen::setMinEpsilon(double minEpsilon) {
	this->minEpsilon = minEpsilon;
}

void ChartTableGen::setMaxEpsilon(double maxEpsilon) {
	this->maxEpsilon = maxEpsilon;
}

double ChartTableGen::getMinZoomLevel() {
	return this->minZoomLevel;
}

double ChartTableGen::getMaxZoomLevel() {
	return this->maxZoomLevel;
}

double ChartTableGen::getMinEpsilon() {
	return this->minEpsilon;
}

double ChartTableGen::getMaxEpsilon() {
	return this->maxEpsilon;
}

void ChartTableGen::setSettings(ChartTableSettings settings) {
	this->settings = settings;
	this->setOriginalZoomLevel(settings.originalZoomLevel());
	this->setScaleFactor(settings.scaleFactor());
	this->setSimpLevelCount(settings.simpLevelCount());
	this->setMinZoomLevel(settings.minZoom());
	this->setMinEpsilon(settings.maxZoomEpsilon());
	this->setMaxZoomLevel(settings.maxZoom());
	this->setMaxEpsilon(settings.minZoomEpsilon());
}

ChartTableSettings ChartTableGen::getSettings() {
	return this->settings;
}

double ChartTableGen::getEpsilonForLevel(int simpLevel) {
	if (simpLevel == 0) return 0.0;
	double epsilonMinPow = log(minEpsilon) / log (10); // assume that the user wants epsilon with base 10 :P
	double epsilonMaxPow = log(maxEpsilon) / log (10);
	double epsilonPowRange = epsilonMaxPow-epsilonMinPow;
	double currEpsilonPow = epsilonMaxPow - ((double)(simpLevelCount-simpLevel)/((double)simpLevelCount-1))*epsilonPowRange;
	double currEpsilon = pow(10, currEpsilonPow);
	return currEpsilon;
}

double ChartTableGen::getZoomForLevel(int simpLevel) {
	double zoomLevelMinTimes = log(minZoomLevel / originalZoomLevel) / log(scaleFactor);
	double zoomLevelMaxTimes = log(maxZoomLevel / originalZoomLevel) / log(scaleFactor);
	double zoomLevelTimesRange = zoomLevelMaxTimes-zoomLevelMinTimes;
	double currZoomLevelPow = zoomLevelMinTimes + (double)(simpLevelCount-simpLevel)*zoomLevelTimesRange/(simpLevelCount-1);
	double currZoomLevel = originalZoomLevel * pow(scaleFactor, currZoomLevelPow);
	return currZoomLevel;
}

void ChartTableGen::createXML() {
	qDebug() << "createXML";
	if (XMLFile->open(QIODevice::WriteOnly)) {
		XMLWriter = new QXmlStreamWriter();
		XMLWriter->setAutoFormatting(true);
		XMLWriter->setDevice(XMLFile);
		XMLWriter->writeStartDocument();
		XMLWriter->writeStartElement("charttable");
		XMLWriter->writeEndElement();
		XMLWriter->writeEndDocument();
		XMLFile->close();
		delete (XMLWriter);
	}
}

QDomElement ChartTableGen::getXMLRoot() {
	QDomElement docElem;
	if (XMLFile->open(QIODevice::ReadOnly)) {
		if (XMLDoc.setContent(XMLFile)) {
			XMLFile->close();
			// get root element <charttable>
			docElem = XMLDoc.documentElement();
			return docElem;
		}
		XMLFile->close();
	}
	return docElem; // returned empty?
}

bool ChartTableGen::saveChartTableToXML(QVector<LayerVisualInfo*> data, QString fileName) {
	XMLFile = new QFile(fileName);
	if (!XMLFile->exists()) this->createXML();
	QString xml;

	QDomElement root = this->getXMLRoot();
	QDomNode n = root.firstChild();
	bool found = false;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="charttable") {
			found = true;
			break;
		}
		n = n.nextSibling();
	}

	// remove old layer visual info nodes
	QDomNode rn;
	do {
		rn = root.removeChild(root.lastChild());
	} while (!rn.isNull());

	QVectorIterator<LayerVisualInfo*> lviIt(data);
	LayerVisualInfo* lvi;

	while (lviIt.hasNext()) {
		lvi = lviIt.next();
		QDomElement lviElem = XMLDoc.createElement("layervisualinfo");

		root.appendChild(lviElem);

		QDomElement layerId = XMLDoc.createElement("layerid");
		lviElem.appendChild(layerId);
		layerId.appendChild(XMLDoc.createTextNode(lvi->getLayerId()));

		QDomElement minZoom = XMLDoc.createElement("minzoom");
		lviElem.appendChild(minZoom);
		minZoom.appendChild(XMLDoc.createTextNode(QString::number(lvi->getZoomMin())));

		QDomElement maxZoom = XMLDoc.createElement("maxzoom");
		lviElem.appendChild(maxZoom);
		maxZoom.appendChild(XMLDoc.createTextNode(QString::number(lvi->getZoomMax())));

		QDomElement styleId = XMLDoc.createElement("styleid");
		lviElem.appendChild(styleId);
		styleId.appendChild(XMLDoc.createTextNode(QString::number(lvi->getStyleId())));

		QDomElement simpLevel = XMLDoc.createElement("simplevel");
		lviElem.appendChild(simpLevel);
		simpLevel.appendChild(XMLDoc.createTextNode(QString::number(lvi->getSimpLevel())));

		QDomElement epsilon = XMLDoc.createElement("epsilon");
		lviElem.appendChild(epsilon);
		epsilon.appendChild(XMLDoc.createTextNode(QString::number(lvi->getEpsilon())));

		QDomElement srcTable = XMLDoc.createElement("srctable");
		lviElem.appendChild(srcTable);
		srcTable.appendChild(XMLDoc.createTextNode(lvi->getSrcTableName()));

	}

	xml = XMLDoc.toString(4);

	if (XMLFile->open(QIODevice::WriteOnly)) {
		QTextStream out(XMLFile);
		out << xml;
		XMLFile->close();
	}

	return true;
}

QVector<LayerVisualInfo*> ChartTableGen::loadChartTableFromXML(QString fileName, bool *ok) {
	XMLFile = new QFile(fileName);
	if (!XMLFile->exists()) this->createXML();

	QVector<LayerVisualInfo*> lvi;
	LayerVisualInfo* tmpLVI;
	*ok=false;
	QDomElement root = this->getXMLRoot();
	if (root.tagName() != "charttable") {
		return lvi;
	}
	QDomNode n = root.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull() && e.tagName()=="layervisualinfo") {
			QDomNode set = e.firstChild();
			tmpLVI = new LayerVisualInfo();
			while (!set.isNull()) {
				QDomElement lviElem = set.toElement(); // try to convert the node to an element.
				if(!lviElem.isNull()) {
					if (lviElem.tagName()=="layerid")
						tmpLVI->setLayerId(lviElem.text());
					else if (lviElem.tagName()=="minzoom")
						tmpLVI->setZoomMin(lviElem.text().toDouble());
					else if (lviElem.tagName()=="maxzoom")
						tmpLVI->setZoomMax(lviElem.text().toDouble());
					else if (lviElem.tagName()=="styleid")
						tmpLVI->setStyleId((int)lviElem.text().toInt());
					else if (lviElem.tagName()=="simplevel")
						tmpLVI->setSimpLevel((int)lviElem.text().toInt());
					else if (lviElem.tagName()=="epsilon")
						tmpLVI->setEpsilon(lviElem.text().toDouble());
					else if (lviElem.tagName()=="srctable")
						tmpLVI->setSrcTableName(lviElem.text());
				}
				set = set.nextSibling();
			}
			lvi << tmpLVI;
		}
		n = n.nextSibling();
	}
	*ok = true;
	return lvi;
}

