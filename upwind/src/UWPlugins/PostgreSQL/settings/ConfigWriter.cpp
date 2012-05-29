/**
 * ConfigWriter.cpp
 *
 *  Created on: 2009-04-15
 *      Author: Ola
 */
#include "ConfigWriter.h"

ConfigWriter::ConfigWriter() {
	file = new QFile("config.xml");
	if (!file->exists()) this->createXml();
}

ConfigWriter::ConfigWriter(QString filename) {
	file = new QFile(filename);
	if (!file->exists()) this->createXml();
}

ConfigWriter::~ConfigWriter() {
	delete file;
}

void ConfigWriter::setFileName(QString filename) {
	file->setFileName(filename);
}

/**
 * Creates xml config file with only root element: <settings>
 */
void ConfigWriter::createXml() {
	if (file->open(QIODevice::WriteOnly)) {
		writer = new QXmlStreamWriter();
		writer->setAutoFormatting(true);
		writer->setDevice(file);
		writer->writeStartDocument();
		writer->writeStartElement("settings");
		writer->writeEndElement();
		writer->writeEndDocument();
		file->close();
		delete (writer);
	}
}
void ConfigWriter::saveSettings(Graphics gr, Database db, Device dev, General gen) {
	this->saveGraphicalSettings(gr);
	this->saveDatabaseSettings(db);
	this->saveDeviceSettings(dev);
	this->saveGeneralSettings(gen);
}

/**
 * Adds <graphics> element to the config file
 */
void ConfigWriter::saveGraphicalSettings(Graphics graphics) {
	/*
	 * Remove old version
	 */
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	bool found = false;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="graphics") {
			found = true;
			break;
		}
		n = n.nextSibling();
	}
	/*
	 * Save new version
	 */
	QDomElement gr = doc.createElement("graphics");
	QString openGL = (graphics.openGL) ? "true" : "false";
	gr.setAttribute("openGL", openGL);
	QString antialiasing = (graphics.antialiasing) ? "true" : "false";
	gr.setAttribute("antialiasing", antialiasing);
	if (found) root.replaceChild(gr, n);
	else root.appendChild(gr);

	QDomElement rotate = doc.createElement("rotate_factor");
	gr.appendChild(rotate);
	rotate.appendChild(doc.createTextNode(QString::number(graphics.rotateFactor)));

	QDomElement zoomLevel = doc.createElement("initial_zoom_level");
	gr.appendChild(zoomLevel);
	zoomLevel.appendChild(doc.createTextNode(QString::number(graphics.initialZoomLevel)));

	QDomElement zoom = doc.createElement("zoom_factor");
	gr.appendChild(zoom);
	zoom.appendChild(doc.createTextNode(QString::number(graphics.zoomFactor)));

	QString xml = doc.toString();

	if (file->open(QIODevice::WriteOnly)) {
		QTextStream out(file);
		out << xml;
		file->close();
	}
}

/**
 * Adds <database> element to the config file
 */
void ConfigWriter::saveDatabaseSettings(Database database) {
	/*
	 * Remove old version
	 */
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	bool found = false;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="database") {
			found = true;
			break;
		}
		n = n.nextSibling();
	}
	/*
	 * Save new version
	 */
	QDomElement db = doc.createElement("database");
	if (found) root.replaceChild(db, n);
	else root.appendChild(db);

	QDomElement name = doc.createElement("name");
	db.appendChild(name);
	name.appendChild(doc.createTextNode(database.DBName));

	QDomElement user = doc.createElement("user");
	db.appendChild(user);
	user.appendChild(doc.createTextNode(database.DBUser));

	QDomElement pass = doc.createElement("password");
	db.appendChild(pass);
	pass.appendChild(doc.createTextNode(database.DBPass));

	QDomElement port = doc.createElement("port");
	db.appendChild(port);
	port.appendChild(doc.createTextNode(database.DBPort));

	QDomElement host = doc.createElement("host");
	db.appendChild(host);
	host.appendChild(doc.createTextNode(database.DBHost));

	QDomElement driver = doc.createElement("driver");
	db.appendChild(driver);
	driver.appendChild(doc.createTextNode(database.DBDriver));

	QDomElement chartTableXMLFile = doc.createElement("charttablexmlfile");
	db.appendChild(chartTableXMLFile);
	chartTableXMLFile.appendChild(doc.createTextNode(database.chartTableXMLFile));

	QString xml = doc.toString();

	if (file->open(QIODevice::WriteOnly)) {
		QTextStream out(file);
		out << xml;
		file->close();
	}
}

/**
 * Adds <device> element to the config file
 */
void ConfigWriter::saveDeviceSettings(Device device) {
	/*
	 * Remove old version
	 */
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	bool found = false;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="device") {
			found = true;
			break;
		}
		n = n.nextSibling();
	}
	/*
	 * Save new version
	 */
	QDomElement dev = doc.createElement("device");
	QString autoCon = (device.autoconnect) ? "true" : "false";
	dev.setAttribute("autoconnect", autoCon);
	if (found) root.replaceChild(dev, n);
	else root.appendChild(dev);

	QDomElement pn = doc.createElement("port_name");
	dev.appendChild(pn);
	pn.appendChild(doc.createTextNode(device.portName));

	QDomElement br = doc.createElement("baud_rate");
	dev.appendChild(br);
	br.appendChild(doc.createTextNode(QString::number(device.baudRate)));

	QDomElement p = doc.createElement("parity");
	dev.appendChild(p);
	p.appendChild(doc.createTextNode(QString::number(device.parity)));

	QDomElement sb = doc.createElement("stop_bits");
	dev.appendChild(sb);
	sb.appendChild(doc.createTextNode(QString::number(device.stopBits)));

	QDomElement bs = doc.createElement("byte_size");
	dev.appendChild(bs);
	bs.appendChild(doc.createTextNode(QString::number(device.byteSize)));
	QString xml = doc.toString();

	if (file->open(QIODevice::WriteOnly)) {
		QTextStream out(file);
		out << xml;
		file->close();
	}
}

/**
 * Adds <general> element to the config file
 */
void ConfigWriter::saveGeneralSettings(General general) {
	/*
	 * Remove old version
	 */
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	bool found = false;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="general") {
			found = true;
			break;
		}
		n = n.nextSibling();
	}
	/*
	 * Save new version
	 */
	QDomElement gen = doc.createElement("general");
	if (found) root.replaceChild(gen, n);
	else root.appendChild(gen);

	QString rotateMap = (general.rotateMap) ? "true" : "false";
	//gen.setAttribute("rotateMap", rotateMap);
	QDomElement rotate = doc.createElement("rotateMap");
	gen.appendChild(rotate);
	rotate.appendChild(doc.createTextNode(rotateMap));

	QDomElement boat_name = doc.createElement("boat");
	gen.appendChild( boat_name);
	boat_name.appendChild( doc.createTextNode( general.boatName) );

	QDomElement boat_refresh = doc.createElement("boat_refresh");
	gen.appendChild( boat_refresh);
	boat_refresh.appendChild( doc.createTextNode( QString::number( general.boatRefresh)) );

	QString avg_data = (general.boatAvgData) ? "true" : "false";
	QDomElement use_avg_data = doc.createElement("use_avg_data");
	gen.appendChild( use_avg_data);
	use_avg_data.appendChild( doc.createTextNode( avg_data) );

	QDomElement min_depth = doc.createElement("min_depth");
	gen.appendChild( min_depth);
	min_depth.appendChild( doc.createTextNode( QString::number( general.mindepth, 'f', 1)) );

	QDomElement min_signsound = doc.createElement("min_signsound");
	gen.appendChild( min_signsound);
	min_signsound.appendChild( doc.createTextNode( QString::number( general.signsound, 'f', 1)) );

	QDomElement max_turns = doc.createElement("max_turns");
	gen.appendChild( max_turns);
	max_turns.appendChild( doc.createTextNode( QString::number( general.maxturn) ) );

	QDomElement point_offset = doc.createElement("point_obstacles_offset");
	gen.appendChild( point_offset);
	point_offset.appendChild( doc.createTextNode( QString::number( general.poffset, 'f', 1)) );

	QDomElement accuracy = doc.createElement("laylines_accuracy");
	gen.appendChild( accuracy);
	accuracy.appendChild( doc.createTextNode( QString::number( general.accu, 'f', 1)) );

	QDomElement perf = doc.createElement("upWindPerformance");
	gen.appendChild(perf);
	QString upWindPerformance = (general.upWindPerformance) ? "true" : "false";
	perf.appendChild(doc.createTextNode(upWindPerformance));

	QString xml = doc.toString();

	if (file->open(QIODevice::WriteOnly)) {
		QTextStream out(file);
		out << xml;
		file->close();
	}
}

/**
 * Returns root element <settings>
 */
QDomElement ConfigWriter::getRoot() {
	if (file->open(QIODevice::ReadOnly)) {
		if (doc.setContent(file)) {
			file->close();
			// get root element (settings)
			QDomElement docElem = doc.documentElement();
			return docElem;
		}
		file->close();
	}
}

/**
 * Returns Device object loaded from config file - <device> element
 */
Device ConfigWriter::loadDeviceSettings() {
	Device device;
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="device") {
			device.autoconnect = (e.attribute("autoconnect")=="true") ? true : false;
			QDomNode set = e.firstChild();
			while (!set.isNull()) {
				QDomElement dev = set.toElement(); // try to convert the node to an element.
				if(!dev.isNull()) {
					if (dev.tagName()=="port_name")
						device.portName = dev.text();
					else if (dev.tagName()=="baud_rate")
						device.baudRate = dev.text().toInt();
					else if (dev.tagName()=="parity")
						device.parity = dev.text().toInt();
					else if (dev.tagName()=="stop_bits")
						device.stopBits = dev.text().toInt();
					else if (dev.tagName()=="byte_size")
						device.byteSize = dev.text().toInt();
				}
				set = set.nextSibling();
			}
		}
		n = n.nextSibling();
	}
	return device;
}

/**
 * Returns Database object loaded from config file - <database> element
 */
Database ConfigWriter::loadDatabaseSettings() {
	Database db;
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="database") {
			QDomNode set = e.firstChild();
			while (!set.isNull()) {
				QDomElement dev = set.toElement(); // try to convert the node to an element.
				if(!dev.isNull()) {
					if (dev.tagName()=="name")
						db.DBName = dev.text();
					else if (dev.tagName()=="user")
						db.DBUser = dev.text();
					else if (dev.tagName()=="password")
						db.DBPass = dev.text();
					else if (dev.tagName()=="port")
						db.DBPort = dev.text();
					else if (dev.tagName()=="host")
						db.DBHost = dev.text();
					else if (dev.tagName()=="driver")
						db.DBDriver = dev.text();
					else if (dev.tagName()=="charttablexmlfile")
						db.chartTableXMLFile = dev.text();
				}
				set = set.nextSibling();
			}
		}
		n = n.nextSibling();
	}
	return db;
}

/**
 * Returns Graphics object loaded from config file - <graphics> element
 */
Graphics ConfigWriter::loadGraphicalSettings() {
	Graphics graphics;
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="graphics") {
			graphics.openGL = (e.attribute("openGL")=="true") ? true : false;
			graphics.antialiasing = (e.attribute("antialiasing")=="true") ? true : false;
			QDomNode set = e.firstChild();
			while (!set.isNull()) {
				QDomElement dev = set.toElement(); // try to convert the node to an element.
				if(!dev.isNull()) {
					if (dev.tagName()=="rotate_factor")
						graphics.rotateFactor = dev.text().toInt();
					else if (dev.tagName()=="initial_zoom_level")
						graphics.initialZoomLevel = dev.text().toInt();
					else if (dev.tagName()=="zoom_factor")
						graphics.zoomFactor = dev.text().toInt();
				}
				set = set.nextSibling();
			}
		}
		n = n.nextSibling();
	}
	return graphics;
}


General ConfigWriter::loadGeneralSettings() {
	General general;
	QDomElement root = this->getRoot();
	QDomNode n = root.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()&&e.tagName()=="general")	{
			QDomNode set = e.firstChild();
			while (!set.isNull()) {
				QDomElement dev = set.toElement();
				if (!dev.isNull()) {
					if (dev.tagName() == "rotateMap")
						general.rotateMap = (dev.text()=="true") ? true : false;
					else if (dev.tagName()=="boat")
						general.boatName = dev.text();
					else if (dev.tagName()=="boat_refresh")
						general.boatRefresh = dev.text().toInt();
					else if (dev.tagName()=="use_avg_data")
						general.boatAvgData = ( dev.text() =="true") ? true : false;
					else if (dev.tagName()=="min_depth")
						general.mindepth = dev.text().toFloat();
					else if (dev.tagName()=="min_signsound")
						general.signsound = dev.text().toFloat();
					else if (dev.tagName()=="max_turns")
						general.maxturn = dev.text().toInt();
					else if (dev.tagName()=="point_obstacles_offset")
						general.poffset = dev.text().toFloat();
					else if (dev.tagName()=="laylines_accuracy")
						general.accu = dev.text().toFloat();
					else if (dev.tagName() == "upWindPerformance")
						general.upWindPerformance = (dev.text() == "true") ? true : false;

				}
				set = set.nextSibling();
			}
			// Let's make sure we have reasonable values
			if ( general.boatName == "" )
				general.boatName = "Dummy";
			if ( general.boatRefresh < 1 || general.boatRefresh > 1000 )
				general.boatRefresh = 5;
			if ( general.boatAvgData < 0 || general.boatAvgData > 1000 )
				general.boatAvgData = 4;
			if ( general.mindepth < 3.0 || general.mindepth > 6.0 )
				general.mindepth = 3.0;
			if ( general.signsound < 0 || general.signsound > 50 )
				general.signsound = 2.5;
			if ( general.maxturn > 20 || general.maxturn < 1 )
				general.maxturn = 3;
			if ( general.poffset < 0.1 || general.poffset > 200 )
				general.poffset = 5.0;
			if ( general.accu < 1 || general.accu > 200 )
				general.accu = 20.0;
		}
		n = n.nextSibling();
	}
	return general;
}

BoatInfo ConfigWriter::loadBoatInfo( QString name)
{
	BoatInfo myboat;

	// Failsafe boat
	if ( name == "Dummy" || name.isEmpty() ) {

		// Every boat has to have at least one polar diagram!
		PolarDiagram mypolar;
		mypolar.populate();
        myboat.name = "Dummy";
		myboat.pdiagrams.append( mypolar);
		myboat.default_pd = 0;
		myboat.length = 0;
		myboat.height = 0;
		myboat.draught = 0;
		myboat.width = 0;

	} else {

		// if the given name contains ".xml" should be a full path
		// else try to load from the local boats directory
		if ( !name.contains( ".xml", Qt::CaseInsensitive) ) {

			name.insert( 0, "boats/");
			name.append( ".xml");
		}

		QFile *boatFile = new QFile( name);

		QDomDocument boatDoc;
		QDomElement boatRoot;

		if ( boatFile->open(QIODevice::ReadOnly) ) {
			if ( boatDoc.setContent( boatFile) ) {
				boatFile->close();
				// get root element (UpWindBoat)
				boatRoot = boatDoc.documentElement();
			}
			boatFile->close();
		}

		QDomNode boatNode = boatRoot.firstChild();

		while ( !boatNode.isNull() ) {

			QDomElement boatElement = boatNode.toElement(); // try to convert the node to an element.

			if ( !boatElement.isNull() ) {

				if ( boatElement.tagName() == "name" )
					myboat.name = boatElement.text();
				else if ( boatElement.tagName() == "model" )
					myboat.model = boatElement.text();
				else if ( boatElement.tagName() == "lenght" )
					myboat.length = boatElement.text().toFloat();
				else if ( boatElement.tagName() == "width" )
					myboat.width = boatElement.text().toFloat();
				else if ( boatElement.tagName() == "draught" )
					myboat.draught = boatElement.text().toFloat();
				else if ( boatElement.tagName() == "height" )
					myboat.height = boatElement.text().toFloat();
				else if ( boatElement.tagName() == "register_number" )
					myboat.registerNumber = boatElement.text();
				else if ( boatElement.tagName() == "owner" )
					myboat.owner = boatElement.text();
				else if ( boatElement.tagName() == "country" )
					myboat.country = boatElement.text();


				else if ( boatElement.tagName() == "polar_diagrams" ) {

					myboat.default_pd = boatElement.attribute( "default").toInt();

					QDomNode polarsNode = boatElement.firstChild();

					while ( !polarsNode.isNull() ) {

						QDomElement pd = polarsNode.toElement();

						//if ( pd.tagName() == "polar_diagram" )

						PolarDiagram temp;
                        temp.clear();
                        temp.setName( pd.attribute( "name"));
                        bool twa = ( pd.attribute("TWA") == "true" ) ? true : false;
                        temp.setTWA( twa);

						QDomNode nspd = pd.firstChild();

						while ( !nspd.isNull() ) {

							QDomElement espd = nspd.toElement();

							//if ( espd.tagName() == "speed" )

							int speed = espd.attribute( "knots").toInt();

							QDomNode li = espd.firstChild();

							Angles atemp;

							while ( !li.isNull() ) {

								QDomElement item = li.toElement();

								atemp.insert( item.attribute( "angle").toFloat(),
											  item.attribute( "TA").toFloat() );

								li = li.nextSibling();
							}

                            temp.setAngles( speed, atemp);
							nspd = nspd.nextSibling();
						}

						myboat.pdiagrams.append( temp);
						polarsNode = polarsNode.nextSibling();
					}
				}




			}

			boatNode = boatNode.nextSibling();
		}

		delete boatFile;
	}

	return myboat;
}

void ConfigWriter::saveBoatInfo( BoatInfo boatinfo)
{
	QString name = boatinfo.name;

	name.insert( 0, "boats/");
	name.append( ".xml");

	QFile *boatFile = new QFile( name);
	QDomDocument boatDoc;
	QDomElement boatRoot;

    if ( boatFile->open( QIODevice::WriteOnly) ) {

			QXmlStreamWriter * boatWriter = new QXmlStreamWriter();
			boatWriter->setAutoFormatting(true);
			boatWriter->setDevice(boatFile);
			boatWriter->writeStartDocument();
			boatWriter->writeStartElement("UpWindBoat");
			boatWriter->writeEndElement();
			boatWriter->writeEndDocument();
			boatFile->close();
			delete (boatWriter);
		}


	if ( boatFile->open(QIODevice::ReadOnly) ) {
		if ( boatDoc.setContent( boatFile) ) {
			boatFile->close();
			// get root element (UpWindBoat)
			boatRoot = boatDoc.documentElement();
		}
		boatFile->close();
	}

	QDomElement bname = boatDoc.createElement( "name");
	boatRoot.appendChild( bname);
	bname.appendChild( boatDoc.createTextNode( boatinfo.name ));

	QDomElement model = boatDoc.createElement( "model");
	boatRoot.appendChild( model);
	model.appendChild( boatDoc.createTextNode( boatinfo.model ));

	QDomElement length = boatDoc.createElement( "lenght");
	boatRoot.appendChild( length);
	length.appendChild( boatDoc.createTextNode( QString::number( boatinfo.length, 'f', 1) ));

	QDomElement width = boatDoc.createElement( "width");
	boatRoot.appendChild( width);
	width.appendChild( boatDoc.createTextNode( QString::number( boatinfo.width, 'f', 1) ));

	QDomElement draught = boatDoc.createElement( "draught");
	boatRoot.appendChild( draught);
	draught.appendChild( boatDoc.createTextNode( QString::number( boatinfo.draught, 'f', 1) ));

	QDomElement height = boatDoc.createElement( "height");
	boatRoot.appendChild( height);
	height.appendChild( boatDoc.createTextNode( QString::number( boatinfo.height, 'f', 1) ));

	QDomElement rn = boatDoc.createElement( "register_number");
	boatRoot.appendChild( rn);
	rn.appendChild( boatDoc.createTextNode( boatinfo.registerNumber ));

	QDomElement ow = boatDoc.createElement( "owner");
	boatRoot.appendChild( ow);
	ow.appendChild( boatDoc.createTextNode( boatinfo.owner ));

	QDomElement cn = boatDoc.createElement( "country");
	boatRoot.appendChild( cn);
	cn.appendChild( boatDoc.createTextNode( boatinfo.country ));

	QDomElement pds = boatDoc.createElement( "polar_diagrams");
	if ( boatinfo.default_pd < boatinfo.pdiagrams.size() )
		pds.setAttribute( "default", QString::number( boatinfo.default_pd ) );
	else
		pds.setAttribute( "default", "0" );
	boatRoot.appendChild( pds);

	// for each polar diagram
	for ( int i = 0; i < boatinfo.pdiagrams.size(); i++ ) {

		// create it and set the attributes
		QDomElement pd = boatDoc.createElement( "polar_diagram");
        pd.setAttribute( "name", boatinfo.pdiagrams[i].getName());
        QString TWA = ( boatinfo.pdiagrams[i].isTWA() ) ? "true" : "false";
		pd.setAttribute( "TWA", TWA);

		// go through the diagram speeds
        QList<float> speeds = boatinfo.pdiagrams[i].getSpeeds();
        for ( int j = 0; j < speeds.size(); j++ ) {

            float speed = speeds[j];

			QDomElement sp = boatDoc.createElement( "speed");
            sp.setAttribute( "knots", speed);

            Angles angles = boatinfo.pdiagrams[i].getAngles( speed);
            Angles::const_iterator z = angles.constBegin();

            while ( z != angles.constEnd() ) {
				QDomElement dt = boatDoc.createElement( "data");
				dt.setAttribute( "angle", z.key());
				dt.setAttribute( "TA", z.value());
				z++;
				sp.appendChild( dt);
            }

			pd.appendChild( sp);
		}

		pds.appendChild( pd);
	}

	QString xml = boatDoc.toString();

	if ( boatFile->open( QIODevice::WriteOnly) ) {
		QTextStream out( boatFile);
		out << xml;
		boatFile->close();
	}
	delete boatFile;
}
