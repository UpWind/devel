/*
 * ConfigWriter.h
 *
 *  Created on: 2009-04-15
 *      Author: Ola
 */

#ifndef CONFIGWRITER_H_
#define CONFIGWRITER_H_

#include <QtGui>
#include <QFile>
#include <QString>
#include <QtXml>
#include "Device.h"
#include "../shared/polardiagram.h"

class ConfigWriter {
public:
	ConfigWriter();
	ConfigWriter(QString);
	~ConfigWriter();

	void setFileName(QString);

	void saveGraphicalSettings(Graphics);
	Graphics loadGraphicalSettings();
	void saveDatabaseSettings(Database);
	Database loadDatabaseSettings();
	void saveDeviceSettings(Device);
	Device loadDeviceSettings();
	void saveGeneralSettings(General);
	General loadGeneralSettings();

	/**
	  * \brief Saves the information of a boat to an XML file.
	  * \note It will use a relative directory called "boats/" for the
	  * saving and It will use the name of the boat as a file name.
	  */
	static void saveBoatInfo( BoatInfo boatinfo);

	/**
	  * \brief Loads the information of a boat returns a BoatInfo struct.
	  * \note It will return a "failsafe" boat called Dummy with a failsafe
	  * PolarDiagram if the input name is empty OR is Dummy.
	  */
	static BoatInfo loadBoatInfo( QString name );

	void saveSettings(Graphics, Database,  Device, General);

private:
	void createXml();
	QDomElement getRoot();
	QFile *file;
	QXmlStreamWriter *writer;
	QDomDocument doc;
};

#endif /* CONFIGWRITER_H_ */
