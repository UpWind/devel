
#include "SettingsManager.h"

#define DEBUG 1

SettingsManager* SettingsManager::instance = NULL;

SettingsManager* SettingsManager::getInstance()
{
	if(instance == NULL)
		instance = new SettingsManager();
	return instance;
}

SettingsManager::~SettingsManager()
{
	if ( instance != NULL ) {
		delete xmlWriter;
        delete instance;
	}
}



SettingsManager::SettingsManager()
{
	debug = DEBUG;
    xmlWriter = new ConfigWriter();
    this->loadSettings();
}

bool SettingsManager::getAntialiasing() const
{
	return this->graphics.antialiasing;
}


bool SettingsManager::getOpenGL() const
{
	return this->graphics.openGL;
}

int SettingsManager::getRotateFactor() const
{
	return this->graphics.rotateFactor;
}

int SettingsManager::getInitialZoomLevel() const {
	return this->graphics.initialZoomLevel;
}

int SettingsManager::getZoomFactor() const
{
	return this->graphics.zoomFactor;
}

bool SettingsManager::getRotateMap() const
{
	return this->general.rotateMap;
}

QString SettingsManager::getBoatName() const
{
    return this->general.boatName;
}

int SettingsManager::getBoatRefresh() const
{
	return this->general.boatRefresh;
}

bool SettingsManager::getUseAvgData() const
{
	return this->general.boatAvgData;
}

float SettingsManager::getMinDepth() const
{
    return this->general.mindepth;
}

float SettingsManager::getSignsound() const
{
	return this->general.signsound;
}

int SettingsManager::getMaxTurn() const
{
    return this->general.maxturn;
}

float SettingsManager::getPOffset() const
{
    return this->general.poffset;
}

float SettingsManager::getAccu() const
{
    return this->general.accu;
}

bool SettingsManager::getUpWindPerformance() const {
	return this->general.upWindPerformance;
}

Log SettingsManager::getLogger()
{
	return this->log;
}
void SettingsManager::setLog(bool l)
{
	this->log.log= l;
}
void SettingsManager::setLongitude(bool longi)
{
	this->log.longitude= longi;
}
void SettingsManager::setLatitude(bool latitude)
{
	this->log.latitude= latitude;
}
void SettingsManager::setSpeedOverGround(bool speedOverGround)
{
	this->log.speedOverGround= speedOverGround;
}
void SettingsManager::setAppWindSpeed(bool appWindSpeed)
{
	this->log.appWindSpeed= appWindSpeed;
}
void SettingsManager::setAppWindAngle(bool appWindAngle)
{
	this->log.appWindAngle= appWindAngle;
}
void SettingsManager::setTWindSpeedM(bool tWindSpeedM)
{
	this->log.tWindSpeedM= tWindSpeedM;
}
void SettingsManager::setTWindSpeedKm(bool tWindSpeedKm)
{
	this->log.tWindSpeedKm= tWindSpeedKm;
}
void SettingsManager::setTWindSpeedKnots(bool tWindSpeedKnots)
{
	this->log.tWindSpeedKnots= tWindSpeedKnots;
}
void SettingsManager::setCompass(bool compass)
{
	this->log.compass= compass;
}
void SettingsManager::setWaterHeadingMag(bool waterHeadingMag)
{
	this->log.waterHeadingMag= waterHeadingMag;
}
void SettingsManager::setWaterHeadingT(bool waterHeadingT)
{
	this->log.waterHeadingT= waterHeadingT;
}
void SettingsManager::setWaterSpeedKnots(bool waterSpeedKnots)
{
	this->log.waterSpeedKnots= waterSpeedKnots;
}
void SettingsManager::setWaterSpeedKPH(bool waterSpeedKPH)
{
	this->log.waterSpeedKPH= waterSpeedKPH;
}
void SettingsManager::setDepth(bool depth)
{
	this->log.depth= depth;
}
bool SettingsManager::getLongitude()
{
	return this->log.longitude;
}
bool SettingsManager::getLatitude()
{
	return this->log.latitude;
}
bool SettingsManager::getSpeedOverGround()
{
	return this->log.speedOverGround;
}
bool SettingsManager::getAppWindSpeed()
{
	return this->log.appWindSpeed;
}
bool SettingsManager::getAppWindAngle()
{
	return this->log.appWindAngle;
}
bool SettingsManager::getTWindSpeedM()
{
	return this->log.tWindSpeedM;
}
bool SettingsManager::getTWindSpeedKm()
{
	return this->log.tWindSpeedKm;
}
bool SettingsManager::getTWindSpeedKnots()
{
	return this->log.tWindSpeedKnots;
}
bool SettingsManager::getCompass()
{
	return this->log.compass;
}
bool SettingsManager::getWaterHeadingMag()
{
	return this->log.waterHeadingMag;
}
bool SettingsManager::getWaterHeadingT()
{
	return this->log.waterHeadingT;
}
bool SettingsManager::getWaterSpeedKnots()
{
	return this->log.waterSpeedKnots;
}
bool SettingsManager::getWaterSpeedKPH()
{
	return this->log.waterSpeedKPH;
}
bool SettingsManager::getDepth()
{
	return this->log.depth;
}
bool SettingsManager::getLog()
{
	return this->log.log;
}
void SettingsManager::setLoadLogName(QString logname)
{
	this->log.logName=logname;
}

void SettingsManager::setOpenGL(bool openGL)
{
	this->graphics.openGL= openGL;
}

void SettingsManager::setAntialiasing(bool antialiasing)
{
	this->graphics.antialiasing= antialiasing;
}


void SettingsManager::setRotateFactor(int rotateFactor)
{
	this->graphics.rotateFactor= rotateFactor;
}

void SettingsManager::setInitialZoomLevel(int initialZoomLevel)
{
	this->graphics.initialZoomLevel = initialZoomLevel;
}

void SettingsManager::setZoomFactor(int zoomFactor)
{
	this->graphics.zoomFactor= zoomFactor;
}


QString SettingsManager::getDBName() const
{
	return this->db.DBName;
}

QString SettingsManager::getDBUser() const
{
	return this->db.DBUser;
}

QString SettingsManager::getDBPass() const
{
	return this->db.DBPass;
}


QString SettingsManager::getDBPort() const
{
	return this->db.DBPort;
}


QString SettingsManager::getDBHost() const
{
	return this->db.DBHost;
}


QString SettingsManager::getDBDriver() const
{
	return this->db.DBDriver;
}

QString SettingsManager::getChartTableXMLFile() const {
	return this->db.chartTableXMLFile;
}

void SettingsManager::setDBName(QString DBName)
{
	this->db.DBName= DBName;
}

void SettingsManager::setDBUser(QString DBUser)
{
	this->db.DBUser= DBUser;
}

void SettingsManager::setDBPass(QString DBPass)
{
	this->db.DBPass= DBPass;
}

void SettingsManager::setDBPort(QString DBPort)
{
	this->db.DBPort= DBPort;
}

void SettingsManager::setDBHost(QString DBHost)
{
	this->db.DBHost= DBHost;
}

void SettingsManager::setDBDriver(QString DBDriver)
{
	this->db.DBDriver= DBDriver;
}

void SettingsManager::setChartTableXMLFile(QString chartTableXMLFile) {
	this->db.chartTableXMLFile = chartTableXMLFile;
}

void SettingsManager::setRotateMap(bool rotateMap)
{
	this->general.rotateMap= rotateMap;
}

void SettingsManager::setBoatName(QString name)
{
    this->general.boatName = name;
    SettingsManager::setBoatInfo();
}

void SettingsManager::setBoatRefresh( int value)
{
	this->general.boatRefresh = value;
}

void SettingsManager::setUseAvgData( bool value)
{
	this->general.boatAvgData = value;
}

void SettingsManager::setMinDepth( float m_depth)
{
    this->general.mindepth = m_depth;
}

void SettingsManager::setSignsound( float value)
{
	this->general.signsound = value;
}

void SettingsManager::setMaxTurn( int turn)
{
    this->general.maxturn = turn;
}

void SettingsManager::setPOffset( float offset)
{
    this->general.poffset = offset;
}

void SettingsManager::setAccu( float offset)
{
    this->general.accu = offset;
}

void SettingsManager::setUpWindPerformance(bool upWindPerformance) {
	this->general.upWindPerformance = upWindPerformance;
}

void SettingsManager::setDevice(Device device)
{
	this->device = device;
}

Device SettingsManager::getDevice()
{
	return this->device;
}

void SettingsManager::setBoatInfo()
{
    this->boatinfo = xmlWriter->loadBoatInfo( this->general.boatName  );
}

BoatInfo SettingsManager::getBoatInfo()
{
        return this->boatinfo;
}

BoatInfo * SettingsManager::getBoatInfoPointer()
{
		return &boatinfo;
}

void SettingsManager::loadSettings()
{
	if (debug) qDebug() << "SettingsManager::loadSettings()";

	this->graphics = xmlWriter->loadGraphicalSettings();
	this->db = xmlWriter->loadDatabaseSettings();
	this->device = xmlWriter->loadDeviceSettings();
	this->general = xmlWriter->loadGeneralSettings();
	this->setBoatInfo();
}

void SettingsManager::saveSettings()
{
	xmlWriter->saveSettings(this->graphics, this->db, this->device, this->general);
}
