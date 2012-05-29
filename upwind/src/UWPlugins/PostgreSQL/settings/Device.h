/*
 * Device.h
 *
 *  Created on: 2009-04-15
 *      Author: Ola
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <QString>
#include <QList>
#include "../shared/polardiagram.h"

struct Device {
public:
	bool autoconnect;
	QString portName;
	int baudRate;
	int parity;
	int stopBits;
	int byteSize;
};

struct Database {
public:
	QString DBName;
	QString DBUser;
	QString DBPass;
	QString DBPort;
	QString DBHost;
	QString DBDriver;
	/**
	 * The name of the file containing the visual info of layers.
	 */
	QString chartTableXMLFile;
};

struct Graphics {
public:
	bool openGL;
	bool antialiasing;
	int rotateFactor;
	int initialZoomLevel;
	int zoomFactor;
};

struct GeneralStruct {
public:
	bool rotateMap;
	/*! The name of the boat to use by default */
    QString boatName;
	/*! The refresh time of the boat (graphic, checkpoint & lay lines path)*/
	int boatRefresh;
	/*! Use an average of the last X values coming from the instruments.
		Just implemented for TWA, TW, and boat angle (heading) */
	bool boatAvgData;
	/*! The depth of the depthcont lines to be considered as obstacles */
	float mindepth;
	/*! The depth of signsound points to be considered as obstacles */
	float signsound;
	/*! The maximum number of turning points to find between
		our position and the destiny checkpoint */
	int maxturn;
	/*! The offset to be added to the point obstacles */
	float poffset;
	/*! The accuracy (offset) when finding the short term path.
		lower values output a more close to obstacles paths and
		use much more loops in the algorithm */
	float accu;
	bool upWindPerformance; // just for benchmarking
} typedef General;

struct Log {
public:
	bool log;
	QString logName;
	bool longitude;
	bool latitude;
	bool speedOverGround;
	bool appWindSpeed;
	bool appWindAngle;
	bool tWindSpeedM;
	bool tWindSpeedKm;
	bool tWindSpeedKnots;
	bool compass;
	bool waterHeadingMag;
	bool waterHeadingT;
	bool waterSpeedKnots;
	bool waterSpeedKPH;
	bool depth;
};

/** \struct BoatInfo
  * A structure for keeping the boat information.
  */
struct BoatInfoStruct {
public:
	/*! The name of the boat */
    QString name;
	/*! The model of the boat */
    QString model;
	/*! The lenght of the boat */
    float length;
	/*! The width of the boat */
    float width;
	/*! The draught of the boat */
    float draught;
	/*! The height of the boat */
    float height;
	/*! The register number of the boat */
    QString registerNumber;
	/*! The owner of the boat */
    QString owner;
	/*! The country of the boat */
    QString country;
	/*! A vector of polar diagram for the boat */
	QList<PolarDiagram> pdiagrams;
	/*! The polar diagram used by default */
	int default_pd;

} typedef BoatInfo;



#endif /* DEVICE_H_ */
