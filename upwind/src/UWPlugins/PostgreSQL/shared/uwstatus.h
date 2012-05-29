/**
 * @file uwstatus.h
 * @author Carles Escrig i Royo (UpWind::ShortTermPlanning)
 * @date 2009 / 2010
 */

#ifndef UWSTATUS_H
#define UWSTATUS_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QRect>
#include <QRectF>
#include <QDebug>
#include "../layers/LayersManager.h"

#define NO_VALUE 2000

/** \brief Class to share information about the state of the application.
  *
  * This class is used to share information about the state of
  * the application -and data- in between classes that are not connected.
  * It is intended to avoid the creation of too much \i similar local
  * variables in diferent but related classes.
  *
  * @author Carles Escrig Royo (UpWind::ShortTermPlanning)
  * @date 2010-01-28 (started)
  */
class UwStatus
{
public:
	static UwStatus * getInstance();  //Singleton class
	~UwStatus();
	void clearChartData();

	bool debug() const;
	bool rubberBandDrag() const;
	bool simulationMode() const;
	bool settingSimRoute() const;
	bool settingSimPos() const;
	bool newRoute() const;
	bool obstaclesTablesCreated() const;
	bool drawBoat() const;
	bool drawRoute() const;
	QGraphicsView * view() const;
	QGraphicsScene * scene() const;
	QMainWindow * window() const;
	bool boatInitialized() const;
	bool dataReaderInitialized() const;
	bool boatOn() const;
	bool toolBoxVisible() const;
	QRect screenGeometry() const;
	QRectF sceneGeometry() const;
	QRectF sceneWithMarginGeometry() const;
	LayersManager * layersManager() const;
	QPointF boatPos() const;
	int currentZoomLevel() const;
	bool boatLock() const;
	bool useAvgData() const;
	int usePolarDiagram() const;
	int currentRotation() const;

	void setDebug(bool);
	void setRubberBandDrag(bool);
	void setSimulationMode(bool);
	void setSettingSimRoute(bool);
	void setSettingSimPos(bool);
	void setNewRoute(bool);
	void setObstaclesTablesCreated(bool);
	void setDrawBoat(bool);
	void setDrawRoute(bool);
	void setView(QGraphicsView *);
	void setScene(QGraphicsScene *);
	void setWindow(QMainWindow *);
	void setBoatInitialized(bool);
	void setDataReaderInitialized(bool);
	void setBoatOn(bool);
	void setToolBoxVisible(bool);
	void setScreenGeometry(QRect);
	void setSceneGeometry(QRectF);
	void setSceneWithMarginGeometry(QRectF);
	void setLayersManager(LayersManager *);
	void setBoatPos(QPointF);
	void setCurrentZoomLevel(int);
	void setBoatLock(bool);
	void setUseAvgData(bool);
	void setUsePolarDiagram(int);
	void setCurrentRotation(int);

private:
	static UwStatus * instance;
	UwStatus();

	/*! debug the application */
	bool uwdebug;
	/*! view is in QGraphicsView::RubberBandDrag Mode */
	bool rubber;
	/*! app is in simulation mode */
	bool simMode;
	/*! user is setting the simulated boat route */
	bool setSimRoute;
	/*! user is setting the simulated boat position */
	bool setSimPos;//! the route is new
	bool nRoute; //! obstacles tables are created
	bool obsTablesCreated;  //! there is position for the boat and it should be shown
	bool drawingBoat; //! thre is a route stablished and it should be shown
	bool drawingRoute; //! the class boat is initialized
	bool boatInit; //! the data reader has been updated at least once
	bool dataReaderInit;//! the boat is on (the thread loop is running)
	bool uwBoatOn;  //! show the toolbox
	bool showingToolBox; //! the view of the ChartWindow
	QGraphicsView * uwview; //! the scene of the ChartWindow
	QGraphicsScene * uwscene; //! the ChartWindow
	QMainWindow * uwwindow; //! the LayersManager
	LayersManager * uwlayersmanager; //! the size of the screen
	QRect uwScreenGeometry; //! the size of the scene
	QRectF uwSceneGeometry; //! the size of the scene with margin
	QRectF uwSceneWithMarginGeometry;

	/*! The boat position in QtGraphicsScene coords */
	QPointF uwBoatPos;
	/*! Lock the view to the boat */
	int uwBoatLock;
	/*! The current zoom level in the chart */
	int uwCurrentZoomLevel;
	/*! The current rotation in the chart */
	int uwCurrentRotation;
	/*! If the boat should use AVG data */
	bool uwUseAvgData;
	/*! Number of the polar diagram to be used,
   polar diagrams are in a QList<PolarDiagram> contained
   in the BoatInfo structure on the SettingsManager */
    int polardiagram;
};

#endif // UWSTATUS_H
