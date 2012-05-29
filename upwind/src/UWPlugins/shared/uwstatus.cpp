#include "uwstatus.h"

UwStatus * UwStatus::instance = NULL;

UwStatus * UwStatus::getInstance(){
	if(instance == NULL)
		instance = new UwStatus();
	return instance;
}

UwStatus::~UwStatus(){
	if(instance != NULL)
		delete instance;
}

UwStatus::UwStatus(){
	uwdebug = true;
	rubber = false;
	simMode = false;
	setSimRoute = false;
	setSimPos = false;
	obsTablesCreated = false;
	drawingBoat = false;
	drawingRoute = false;
	nRoute = false;
	boatInit = false;
	dataReaderInit = false;
	uwBoatOn = false;
	showingToolBox = true;
	uwview = NULL;
	uwscene = NULL;
	uwwindow = NULL;
	uwlayersmanager = NULL;
	uwBoatLock = false;
	uwUseAvgData = false;
	polardiagram = 0;
	uwCurrentRotation = 0;
}

void UwStatus::clearChartData(){
    rubber = false;
    simMode = false;
    setSimRoute = false;
    setSimPos = false;
    obsTablesCreated = false;
    drawingBoat = false;
    drawingRoute = false;
    nRoute = false;
    boatInit = false;
    dataReaderInit = false;
    uwBoatOn = false;
    showingToolBox = true;
    uwview = NULL;
    uwscene = NULL;
    uwwindow = NULL;
    uwCurrentZoomLevel = 0;
    uwBoatLock = false;
    uwUseAvgData = false;
    polardiagram = 0;
    uwCurrentRotation = 0;
}

bool UwStatus::debug() const{
    return this->uwdebug;
}

bool UwStatus::rubberBandDrag() const{
	if(uwdebug)
		qDebug() << "UwStatus::rubberBandDrag(): " << this->rubber;
	return this->rubber;
}

bool UwStatus::simulationMode() const{
	if(uwdebug)
		qDebug() << "UwStatus::simulationMode(): " << this->simMode;
	return this->simMode;
}

bool UwStatus::settingSimRoute() const{
	if (uwdebug)
		qDebug() << "UwStatus::settingSimRoute(): " << this->setSimRoute;
	return this->setSimRoute;
}

bool UwStatus::settingSimPos() const{
	if(uwdebug)
		qDebug() << "UwStatus::settingSimPos(): " << this->setSimPos;
	return this->setSimPos;
}

bool UwStatus::newRoute() const{
	if(uwdebug)
		qDebug() << "UwStatus::newRoute(): " << this->setSimPos;
	return this->nRoute;
}

bool UwStatus::obstaclesTablesCreated() const{
	if(uwdebug)
		qDebug() << "UwStatus::obstaclesTablesCreated(): " << this->obsTablesCreated;
	return this->obsTablesCreated;
}

bool UwStatus::drawBoat() const{
	if(uwdebug)
		qDebug() << "UwStatus::drawBoat(): " << this->drawingBoat;
	return this->drawingBoat;
}

bool UwStatus::drawRoute() const{
	if(uwdebug)
		qDebug() << "UwStatus::drawRoute(): " << this->drawingRoute;
	return this->drawingRoute;
}

QGraphicsView * UwStatus::view() const{
	return this->uwview;
}

QGraphicsScene * UwStatus::scene() const{
	return this->uwscene;
}

QMainWindow * UwStatus::window() const{
	return this->uwwindow;
}

bool UwStatus::boatInitialized() const{
	if(uwdebug)
		qDebug() << "UwStatus::boatInitialized(): " << this->boatInit;
	return this->boatInit;
}

bool UwStatus::dataReaderInitialized() const{
	if(uwdebug)
		qDebug() << "UwStatus::dataReaderInitialized(): " << this->dataReaderInit;
	return this->dataReaderInit;
}

bool UwStatus::boatOn() const{
	if(uwdebug)
		qDebug() << "UwStatus::boatOn(): " << this->uwBoatOn;
	return this->uwBoatOn;
}

bool UwStatus::toolBoxVisible() const{
	if(uwdebug)
		qDebug() << "UwStatus::toolBoxVisible(): " << this->showingToolBox;
	return this->showingToolBox;
}

QRect UwStatus::screenGeometry() const{
	return this->uwScreenGeometry;
}

QRectF UwStatus::sceneGeometry() const{
	return this->uwSceneGeometry;
}

QRectF UwStatus::sceneWithMarginGeometry() const{
	return this->uwSceneWithMarginGeometry;
}

LayersManager * UwStatus::layersManager() const{
	return this->uwlayersmanager;
}

QPointF UwStatus::boatPos() const{
	return this->uwBoatPos;
}

int UwStatus::currentZoomLevel() const{
	return this->uwCurrentZoomLevel;
}

bool UwStatus::boatLock() const{
	return this->uwBoatLock;
}

bool UwStatus::useAvgData() const{
	return this->uwUseAvgData;
}

int UwStatus::usePolarDiagram() const{
	return this->polardiagram;
}

int UwStatus::currentRotation() const{
	return this->uwCurrentRotation;
}

void UwStatus::setDebug( bool value){
	this->uwdebug = value;
}

void UwStatus::setRubberBandDrag(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setRubberBandDrag(): " << value;
	this->rubber = value;
}

void UwStatus::setSimulationMode(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setSimulationMode(): " << value;
	this->simMode = value;
}

void UwStatus::setSettingSimRoute(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setSettingSimRoute(): " << value;
	this->setSimRoute = value;
}

void UwStatus::setSettingSimPos(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setSettingSimPos(): " << value;
	this->setSimPos = value;
}

void UwStatus::setNewRoute(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setNewRoute(): " << value;
	this->nRoute = value;
}

void UwStatus::setObstaclesTablesCreated(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setObstaclesTablesCreated(): " << value;
	this->obsTablesCreated = value;
}

void UwStatus::setDrawBoat(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setDrawBoat(): " << value;
	this->drawingBoat = value;
}

void UwStatus::setDrawRoute(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setDrawRoute(): " << value;
	this->drawingRoute = value;
	if (value)
		this->setNewRoute(true);
}

void UwStatus::setView(QGraphicsView * other_view){
	this->uwview = other_view;
}

void UwStatus::setScene(QGraphicsScene * other_scene){
	this->uwscene = other_scene;
}

void UwStatus::setWindow(QMainWindow * other_window){
	this->uwwindow = other_window;
}

void UwStatus::setBoatInitialized(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setBoatInitialized(): " << value;
	this->boatInit = value;
}

void UwStatus::setDataReaderInitialized(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setDataReaderInitialized(): " << value;
	this->dataReaderInit = value;
}

void UwStatus::setBoatOn(bool value){
	if(uwdebug)
		qDebug() << "UwStatus::setBoatOn(): " << value;
	this->uwBoatOn = value;
	if(!value){
		this->setDrawRoute(false);
		this->setDrawBoat(false);
	}
}

void UwStatus::setToolBoxVisible(bool value){
	if (uwdebug)
		qDebug() << "UwStatus::setToolBoxVisible(): " << value;
	this->showingToolBox = value;
}

void UwStatus::setScreenGeometry(QRect value){
	this->uwScreenGeometry = value;
}

void UwStatus::setSceneGeometry(QRectF value){
	this->uwSceneGeometry = value;
}

void UwStatus::setSceneWithMarginGeometry(QRectF value){
	this->uwSceneWithMarginGeometry = value;
}

void UwStatus::setLayersManager(LayersManager * other_lm){
	this->uwlayersmanager = other_lm;
}

void UwStatus::setBoatPos(QPointF other_point){
	this->uwBoatPos = other_point;
}

void UwStatus::setCurrentZoomLevel(int value){
	this->uwCurrentZoomLevel = value;
	this->uwlayersmanager->setZoom(value);
}

void UwStatus::setBoatLock(bool value){
	this->uwBoatLock = value;
}

void UwStatus::setUseAvgData(bool value){
	this->uwUseAvgData = value;
}

void UwStatus::setUsePolarDiagram(int value){
	this->polardiagram = value;
}

void UwStatus::setCurrentRotation(int value){
	this->uwCurrentRotation = value;
}
