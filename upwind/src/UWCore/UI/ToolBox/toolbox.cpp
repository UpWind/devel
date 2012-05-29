#include "toolbox.h"
#include "uwcore.h"
#include <QGridLayout>
#include <QString>

ToolBox::ToolBox(/*LayersManager* layersManager,*/ QWidget *parent)
{
    this->setParent(parent);
    ui.setupUi(this);

    //this->status = UwStatus::getInstance();
    //debug = status->debug();
    //  this->settingsManager = SettingsManager::getInstance();

    //loadStyleSheet();

    //this->layersManager = layersManager;
    //   this->dataSimulator = DataSimulator::getInstance();

    this->actual_profile = NULL;
    ui.upButton->setEnabled(false);
    ui.downButton->setEnabled(false);

    connect(Tab::group,SIGNAL(buttonClicked(QAbstractButton*)),
            this,SLOT(change_profile_menu(QAbstractButton*)));

    connect(ui.downButton,SIGNAL(clicked()),this,SLOT(next_tab()));
    connect(ui.upButton,SIGNAL(clicked()),this,SLOT(previous_tab()));

    ui.spoilgrnd_rCB->setEnabled(false);
    ui.dock_rCB->setEnabled(false);
    ui.closinglne_lCB->setEnabled(false);
    ui.ferry_lCB->setEnabled(false);
    ui.rock_pCB->setEnabled(false); //this should be undisabled
    ui.checkpoint_pCB->setEnabled(false);
    ui.fortstruct_pCB->setEnabled(false);
    ui.obstacles_CB->setEnabled( false);

    ui.lockButton->setEnabled( false);
    ui.destinyButton->setEnabled( true);
    ui.zoomBoatButton->setEnabled( true);
    ui.obstaclesButton->setEnabled( false);
    ui.polarDiagramNamesComboBox->setEnabled( false);
    ui.simPosButton->setEnabled( false);
    ui.simRouteButton->setEnabled( false);

    QStringList routeOptions;
    routeOptions << "The Shortest Route" << "The Fastest Route";
    ui.routeComboBox->addItems(routeOptions);
    ui.routeComboBox->setCurrentIndex(0);
    ui.routeComboBox->setEnabled(false);
    ui.routeButton->setEnabled(false);

    connect( ui.zoomInButton, SIGNAL( clicked() ), this, SIGNAL( zoomInS() ) );
    connect( ui.zoomOutButton, SIGNAL( clicked() ), this, SIGNAL( zoomOutS() ) );
    connect( ui.rotateLeftButton, SIGNAL( clicked() ), this, SIGNAL( rotateLeftS() ) );
    connect( ui.rotateRightButton, SIGNAL( clicked() ), this, SIGNAL( rotateRightS() ) );
    connect( ui.zoomChartButton, SIGNAL( clicked() ), this, SIGNAL( zoomChart()));
    connect( ui.zoomBoatButton, SIGNAL( clicked() ), this, SIGNAL( zoomBoat()));
    connect( ui.zoomToolButton, SIGNAL( clicked() ), this, SLOT(on_zoomToolButton_clicked()));

    connect( ui.obstacles_CB, SIGNAL( stateChanged(int) ), this, SLOT( obstacles_CBChanged(int) ) );
    connect( ui.generarea_rCB, SIGNAL( stateChanged(int) ), this, SLOT( generarea_rCBChanged(int) ) );
    connect( ui.deptharea_rCB, SIGNAL( stateChanged(int) ), this, SLOT( deptharea_rCBChanged(int) ) );
    connect( ui.lake_rCB, SIGNAL( stateChanged(int) ), this, SLOT( lake_rCBChanged(int) ) );
    connect( ui.spoilgrnd_rCB, SIGNAL( stateChanged(int) ), this, SLOT( spoilgrnd_rCBChanged(int) ) );
    connect( ui.limit_rCB, SIGNAL( stateChanged(int) ), this, SLOT( limit_rCBChanged(int) ) );
    connect( ui.builtupare_rCB, SIGNAL( stateChanged(int) ), this, SLOT( builtupare_rCBChanged(int) ) );
    connect( ui.fwayarea_rCB, SIGNAL( stateChanged(int) ), this, SLOT( fwayarea_rCBChanged(int) ) );
    connect( ui.survqual_rCB, SIGNAL( stateChanged(int) ), this, SLOT( survqual_rCBChanged(int) ) );
    connect( ui.building_rCB, SIGNAL( stateChanged(int) ), this, SLOT( building_rCBChanged(int) ) );
    connect( ui.dock_rCB, SIGNAL( stateChanged(int) ), this, SLOT( dock_rCBChanged(int) ) );
    connect( ui.depthcont_lCB, SIGNAL( stateChanged(int) ), this, SLOT( depthcont_lCBChanged(int) ) );
    connect( ui.coastline_lCB, SIGNAL( stateChanged(int) ), this, SLOT( coastline_lCBChanged(int) ) );
    connect( ui.closinglne_lCB, SIGNAL( stateChanged(int) ), this, SLOT( closinglne_lCBChanged(int) ) );
    connect( ui.navigline_lCB, SIGNAL( stateChanged(int) ), this, SLOT( navigline_lCBChanged(int) ) );
    connect( ui.ferry_lCB, SIGNAL( stateChanged(int) ), this, SLOT( ferry_lCBChanged(int) ) );
    connect( ui.limit_lCB, SIGNAL( stateChanged(int) ), this, SLOT( limit_lCBChanged(int) ) );
    connect( ui.heightcont_lCB, SIGNAL( stateChanged(int) ), this, SLOT( heightcont_lCBChanged(int) ) );
    connect( ui.leadinglne_lCB, SIGNAL( stateChanged(int) ), this, SLOT( leadinglne_lCBChanged(int) ) );
    connect( ui.lockndam_lCB, SIGNAL( stateChanged(int) ), this, SLOT( lockndam_lCBChanged(int) ) );
    connect( ui.pipeline_lCB, SIGNAL( stateChanged(int) ), this, SLOT( pipeline_lCBChanged(int) ) );
    connect( ui.submcable_lCB, SIGNAL( stateChanged(int) ), this, SLOT( submcable_lCBChanged(int) ) );
    connect( ui.roadnrailw_lCB, SIGNAL( stateChanged(int) ), this, SLOT( roadnrailw_lCBChanged(int) ) );
    connect( ui.bridge_lCB, SIGNAL( stateChanged(int) ), this, SLOT( bridge_lCBChanged(int) ) );
    connect( ui.transmline_lCB, SIGNAL( stateChanged(int) ), this, SLOT( transmline_lCBChanged(int) ) );
    connect( ui.river_lCB, SIGNAL( stateChanged(int) ), this, SLOT( river_lCBChanged(int) ) );
    connect( ui.shlnecon_lCB, SIGNAL( stateChanged(int) ), this, SLOT( shlnecon_lCBChanged(int) ) );
    connect( ui.navaid_pCB, SIGNAL( stateChanged(int) ), this, SLOT( navaid_pCBChanged(int) ) );
    connect( ui.limit_pCB, SIGNAL( stateChanged(int) ), this, SLOT( limit_pCBChanged(int) ) );
    connect( ui.towernmast_pCB, SIGNAL( stateChanged(int) ), this, SLOT( towernmast_pCBChanged(int) ) );
    connect( ui.signsound_pCB, SIGNAL( stateChanged(int) ), this, SLOT( signsound_pCBChanged(int) ) );
    connect( ui.ctext_pCB, SIGNAL( stateChanged(int) ), this, SLOT( ctext_pCBChanged(int) ) );
    connect( ui.vegetation_pCB, SIGNAL( stateChanged(int) ), this, SLOT( vegetation_pCBChanged(int) ) );
    connect( ui.moorfacil_pCB, SIGNAL( stateChanged(int) ), this, SLOT( moorfacil_pCBChanged(int) ) );
    connect( ui.natseabed_pCB, SIGNAL( stateChanged(int) ), this, SLOT( natseabed_pCBChanged(int) ) );
    connect( ui.obstruct_pCB, SIGNAL( stateChanged(int) ), this, SLOT( obstruct_pCBChanged(int) ) );
    connect( ui.harbour_pCB, SIGNAL( stateChanged(int) ), this, SLOT( harbour_pCBChanged(int) ) );
    connect( ui.marservice_pCB, SIGNAL( stateChanged(int) ), this, SLOT( marservice_pCBChanged(int) ) );
    connect( ui.wreck_pCB, SIGNAL( stateChanged(int) ), this, SLOT( wreck_pCBChanged(int) ) );
    connect( ui.building_pCB, SIGNAL( stateChanged(int) ), this, SLOT( building_pCBChanged(int) ) );
    connect( ui.rock_pCB, SIGNAL( stateChanged(int) ), this, SLOT( rock_pCBChanged(int) ) );
    connect( ui.checkpoint_pCB, SIGNAL( stateChanged(int) ), this, SLOT( checkpoint_pCBChanged(int) ) );
    connect( ui.fortstruct_pCB, SIGNAL( stateChanged(int) ), this, SLOT( fortstruct_pCBChanged(int) ) );
    connect( ui.lightsCB, SIGNAL( stateChanged(int) ), this, SLOT( lightsCBChanged(int) ) );

    connect( ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(brightnessChanged(int)));

    this->initializeInstruments();
    this->initializeLayerBoxes();
}

ToolBox::~ToolBox(){}

void ToolBox::instrument_Clicked(bool value){
    QObject *sender = QObject::sender();

    QCheckBox *cb = qobject_cast<QCheckBox *>(sender);
    QString name = cb->text();

    if(value)UWCore::getInstance()->getPluginManager()->getInstrument(name)->hidePlugin();
    else UWCore::getInstance()->getPluginManager()->getInstrument(name)->showPlugin();
}

void ToolBox::initializeInstruments(){

    QWidget *instrumentWidget = qFindChild<QWidget*>(this, "page3");

    QGridLayout *layout = new QGridLayout();
    instrumentWidget->setLayout(layout);

    QList<NMEAInstrumentInterface*> instruments = UWCore::getInstance()->getPluginManager()->getInstruments();

    foreach(NMEAInstrumentInterface* instrument,instruments){
        QCheckBox *cb = new QCheckBox(instrument->getName());

        layout->addWidget(cb);
        connect( cb ,SIGNAL(toggled(bool)),this,SLOT(instrument_Clicked(bool)));
    }
}

// Sets checkboxes state as checked for the selected layers.
void ToolBox::initializeLayerBoxes()
{
    QSet<QString> selectedLayers;

    selectedLayers = UWCore::getInstance()->getPluginManager()->getChartProvider()->getSelectedLayers();

    foreach(QString layerName, selectedLayers)
    {
        if (layerName == "bridge_l")        { ui.bridge_lCB->setChecked(true); }
        if (layerName == "building_p")      { ui.building_pCB->setChecked(true); }
        if (layerName == "building_r")      { ui.building_rCB->setChecked(true); }
        if (layerName == "builtupare_r")    { ui.builtupare_rCB->setChecked(true); }
        if (layerName == "checkpoint_p")    { ui.checkpoint_pCB->setChecked(true); }
        if (layerName == "closinglne_l")    { ui.closinglne_lCB->setChecked(true); }
        if (layerName == "coastline_l")     { ui.coastline_lCB->setChecked(true); }
        if (layerName == "deptharea_r")     { ui.deptharea_rCB->setChecked(true); }
        if (layerName == "depthcont_l")     { ui.depthcont_lCB->setChecked(true); }
        if (layerName == "dock_r")          { ui.dock_rCB->setChecked(true); }
        if (layerName == "ferry_l")         { ui.ferry_lCB->setChecked(true); }
        if (layerName == "fortstruct_p")    { ui.fortstruct_pCB->setChecked(true); }
        if (layerName == "fwayarea_r")      { ui.fwayarea_rCB->setChecked(true); }
        if (layerName == "generarea_r")     { ui.generarea_rCB->setChecked(true); }
        if (layerName == "harbour_p")       { ui.harbour_pCB->setChecked(true); }
        if (layerName == "heightcont_l")    { ui.heightcont_lCB->setChecked(true); }
        if (layerName == "lake_r")          { ui.lake_rCB->setChecked(true); }
        if (layerName == "leadinglne_l")    { ui.leadinglne_lCB->setChecked(true); }
        if (layerName == "lights")          { ui.lightsCB->setChecked(true); }
        if (layerName == "limit_l")         { ui.limit_lCB->setChecked(true); }
        if (layerName == "limit_p")         { ui.limit_pCB->setChecked(true); }
        if (layerName == "limit_r")         { ui.limit_rCB->setChecked(true); }
        if (layerName == "lockndam_l")      { ui.lockndam_lCB->setChecked(true); }
        if (layerName == "marservice_p")    { ui.marservice_pCB->setChecked(true); }
        if (layerName == "moorfacil_p")     { ui.moorfacil_pCB->setChecked(true); }
        if (layerName == "natseabed_p")     { ui.natseabed_pCB->setChecked(true); }
        if (layerName == "navaid_p")        { ui.navaid_pCB->setChecked(true); }
        if (layerName == "navigline_l")     { ui.navigline_lCB->setChecked(true); }
        if (layerName == "obstacles")       { ui.obstacles_CB->setChecked(true); }
        if (layerName == "obstruct_p")      { ui.obstruct_pCB->setChecked(true); }
        if (layerName == "pipeline_l")      { ui.pipeline_lCB->setChecked(true); }
        if (layerName == "river_l")         { ui.river_lCB->setChecked(true); }
        if (layerName == "roadnrailw_l")    { ui.roadnrailw_lCB->setChecked(true); }
        if (layerName == "rock_p")          { ui.rock_pCB->setChecked(true); }
        if (layerName == "shlnecon_l")      { ui.shlnecon_lCB->setChecked(true); }
        if (layerName == "signsound_p")     { ui.signsound_pCB->setChecked(true); }
        if (layerName == "spoilgrnd_r")     { ui.spoilgrnd_rCB->setChecked(true); }
        if (layerName == "submcable_l")     { ui.submcable_lCB->setChecked(true); }
        if (layerName == "survqual_r")      { ui.survqual_rCB->setChecked(true); }
        if (layerName == "towernmast_p")    { ui.towernmast_pCB->setChecked(true); }
        if (layerName == "transmline_l")    { ui.transmline_lCB->setChecked(true); }
        if (layerName == "vegetation_p")    { ui.vegetation_pCB->setChecked(true); }
        if (layerName == "wreck_p")         { ui.wreck_pCB->setChecked(true); }
    }
}

void ToolBox::loadStyleSheet(){
    QFile qss("ui/toolBox.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();
}

void ToolBox::add_profile(QString *profile, QString * image_path){
    qDebug()<<"toolBox.cpp add_profile ---->   profile : "<< *profile<<" image_path: "<<*image_path;

    if(list.isEmpty()){
        Tab * t = new Tab(ui.stackedWidget);
        t->add_profile(profile,image_path);

        ui.stackedWidget->addWidget(t);
        ui.stackedWidget->setCurrentWidget(t);

        list.append(t);

        ui.divDownLabel->setText(QString::number(1));
        ui.divUpLabel->setText(QString::number(1));

    }else{
        bool exit = false;
        //Find a Tab with less than 6 items
        QListIterator<Tab *> it(list);
        while(it.hasNext() && !exit){
            Tab * t = it.next();
            if(t->get_count()<MAX_PROFILE){
                t->add_profile(profile, image_path);
                list.append(t);
                exit = true;
            }
        }

        //All the tabs contain 6 items. Create a new one tab
        if(!exit){
            Tab * t = new Tab(ui.stackedWidget);
            t->add_profile(profile,image_path);

            ui.stackedWidget->addWidget(t);

            list.append(t);

            int num = ui.divDownLabel->text().toInt(&exit)+1;
            ui.divDownLabel->setText(QString::number(num));
        }
    }
}

void ToolBox::next_tab(){

    int tab_number = ui.stackedWidget->count();
    int index = ui.stackedWidget->currentIndex();

    if(index+1<tab_number){
        ui.stackedWidget->setCurrentIndex(index+1);
        ui.upButton->setEnabled(true);

        if(ui.stackedWidget->currentIndex()+1>=tab_number)
            ui.downButton->setEnabled(false);

        bool exit = true;
        int num = ui.divUpLabel->text().toInt(&exit)+1;
        ui.divUpLabel->setText(QString::number(num));
    }else{
        ui.downButton->setEnabled(false);
    }
}

void ToolBox::previous_tab(){
    //int tab_number = ui.stackedWidget->count();
    int index = ui.stackedWidget->currentIndex();

    if(index>2){
        ui.stackedWidget->setCurrentIndex(index-1);
        ui.downButton->setEnabled(true);

        if(ui.stackedWidget->currentIndex()<=2)
            ui.upButton->setEnabled(false);

        bool exit = true;
        int num = ui.divUpLabel->text().toInt(&exit)-1;
        ui.divUpLabel->setText(QString::number(num));
    }else{
        ui.upButton->setEnabled(false);
    }
}


void ToolBox::change_profile_menu(QAbstractButton * button){
    QString *profile_name = new QString(button->text());
    ui.active_profile->setText(*profile_name);

    bool exit = false;
    QListIterator<Tab *> it(list);

    while(it.hasNext() && !exit){
        Tab * t = it.next();
        QString  *image_path = new QString(t->get_icon_path(profile_name));
        //if(image_path!=NULL && *image_path!=""){
        if(image_path!=NULL && !(*image_path).isEmpty()){
            qDebug()<<"toolBox.cpp change_profile_menu image_path: "<<*image_path;
            ui.image_icon->setPixmap(QPixmap(*image_path));
            exit=true;
            image_path->~QString();
        }

        if((*image_path).isEmpty())
            ui.image_icon->setText("NoImage");
    }

    emit load_profile(*profile_name);
}

void ToolBox::compassCBChanged(int state)
{
    emit compassStateChangedS(state);

}

void ToolBox::windCBChanged(int state)
{
    emit windStateChangedS(state);
}

void ToolBox::gpsCoordsCBChanged(int state)
{
    emit gpsCoordsStateChangedS(state);
}

void ToolBox::boatCBChanged(int state)
{
    if ( state ) {
        ui.lockButton->setEnabled( true);
        ui.zoomBoatButton->setEnabled( true);
        //pBoatInfo = settingsManager->getBoatInfoPointer();
        this->loadPolarDiagramList();
        ui.polarDiagramNamesComboBox->setEnabled( true);
        ui.obstaclesButton->setEnabled( true);
        ui.obstacles_CB->setEnabled( true);
        //ui.destinyButton->setEnabled(true);
    } else {
        ui.lockButton->setEnabled( false);
        ui.zoomBoatButton->setEnabled( false);
        ui.polarDiagramNamesComboBox->setEnabled( false);
    }

    emit boatStateChangedS(state);

}

void ToolBox::angleToolCBChanged(int state)
{
    emit angleToolStateChangedS(state);

}

void ToolBox::hideRotateButtons()
{
    ui.rotateLeftButton->hide();
    ui.rotateRightButton->hide();

}

void ToolBox::bridge_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("bridge_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::building_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("building_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::building_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layerLevel = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("building_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::builtupare_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("builtupare_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::checkpoint_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("checkpoint_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::closinglne_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("closinglne_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::coastline_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("coastline_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::ctext_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("ctext_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::deptharea_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("deptharea_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::depthcont_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("depthcont_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::dock_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("dock_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::ferry_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("ferry_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::fortstruct_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("fortstruct_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::fwayarea_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("fwayarea_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::generarea_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("generarea_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::harbour_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("harbour_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::heightcont_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("heightcont_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::lake_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("lake_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::leadinglne_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("leadinglne_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::lightsCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("lights", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::limit_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("limit_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::limit_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("limit_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::limit_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("limit_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::lockndam_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("lockndam_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::marservice_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("marservice_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::moorfacil_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("moorfacil_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::natseabed_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("natseabed_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::navaid_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("navaid_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::navigline_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("navigline_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::obstacles_CBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("obstacles", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::obstruct_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("obstruct_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::pipeline_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("pipeline_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::river_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("river_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::roadnrailw_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("roadnrailw_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::rock_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("rock_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::shlnecon_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("shlnecon_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::signsound_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("signsound_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::spoilgrnd_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("spoilgrnd_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::submcable_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("submcable_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::survqual_rCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("survqual_r", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::towernmast_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("towernmast_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::transmline_lCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("transmline_l", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::vegetation_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("vegetation_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

void ToolBox::wreck_pCBChanged(int state)
{
    if (state == 2)
    {
        // OGRLayer *layer = UWCore::getInstance()->getPluginManager()->getChartProvider()->getLayerLevel("wreck_p", 5);
    }

    if (state == 0)
    {
        // Un-checked
    }
}

// STATIC SIMULATOR
void ToolBox::on_simPosButton_clicked()
{
    /*if ( status->simulationMode() ) {
                dataSimulator->clearSimBoat();
                status->setSettingSimPos( true);
                emit changeCursor();
        }*/
}

void ToolBox::on_simRouteButton_clicked()
{

    emit drawRoute(true);

    /*if( status->settingSimRoute() || status->drawRoute() ) {
        dataSimulator->clearSimRoute();
        ui.simRouteButton->setIcon( QIcon(":/buttons/set_sim_route.svg"));
        emit changeCursor();
    } else {
        dataSimulator->clearSimRoute();
        status->setSettingSimRoute( true);
        ui.simRouteButton->setIcon( QIcon(":/buttons/clear.svg"));
        emit changeCursor();
    }*/

}

void ToolBox::on_simGeneralButton_clicked()
{
    //if (debug) qDebug() << "ToolBox::on_simGeneralButton_clicked(): " << status->simulationMode();


    if((ui.simGeneralButton->text()).compare(QString("Turn Off")) == 0)
    {
        qDebug() << "ToolBox::on_simGeneralButton_clicked(): turning off";
        emit simModeChanged(false);
        ui.simGeneralButton->setText( "Turn On");

        //emit changeCursor();
        ui.simPosButton->setEnabled( false);
        ui.simRouteButton->setEnabled( false);
      //  ui.destinyButton->setEnabled(false);
        ui.routeComboBox->setEnabled(false);
        ui.routeButton->setEnabled(false);

    }else
    {
        qDebug() << "ToolBox::on_simGeneralButton_clicked(): turning on";
        emit simModeChanged(true);

//        ui.simHeadSpinBox->setValue( dataSimulator->getSimHeading() );
 //       ui.simTWDSpinBox->setValue( dataSimulator->getSimTWD() );
 //       ui.simWinSpeedSpinBox->setValue( dataSimulator->getSimWindSpeed() );
        ui.simGeneralButton->setText( "Turn Off");
 //       ui.boatCB->setCheckState(Qt::Checked);
        ui.simPosButton->setEnabled( true);
        ui.simRouteButton->setEnabled( true);
      //  ui.destinyButton->setEnabled(true);
        ui.routeComboBox->setEnabled(true);
        ui.routeButton->setEnabled(true);


    }

    /*if ( status->simulationMode()) {

            if (debug) qDebug() << "ToolBox::on_simGeneralButton_clicked(): turning off";

            if ( status->drawRoute() || status->settingSimRoute() ) {
                ui.simRouteButton->setIcon( QIcon(":/buttons/set_sim_route.svg"));

            }
          //  dataSimulator->clearAll();
            status->setSettingSimPos( false);
            status->setSettingSimRoute( false);
           // status->setSettingDestiny(false);
            status->setSimulationMode( false);
            ui.simGeneralButton->setText( "Turn On");
            ui.boatCB->setCheckState(Qt::Unchecked);


            emit changeCursor();
            ui.simPosButton->setEnabled( false);
            ui.simRouteButton->setEnabled( false);
          //  ui.destinyButton->setEnabled(false);
            ui.routeComboBox->setEnabled(false);
            ui.routeButton->setEnabled(false);


        } else {

                if (debug) qDebug() << "ToolBox::on_simGeneralButton_clicked(): turning on";

                ui.simHeadSpinBox->setValue( dataSimulator->getSimHeading() );
                ui.simTWDSpinBox->setValue( dataSimulator->getSimTWD() );
                ui.simWinSpeedSpinBox->setValue( dataSimulator->getSimWindSpeed() );
                status->setSimulationMode( true);
                ui.simGeneralButton->setText( "Turn Off");
                ui.boatCB->setCheckState(Qt::Checked);
                ui.simPosButton->setEnabled( true);
                ui.simRouteButton->setEnabled( true);
              //  ui.destinyButton->setEnabled(true);
                ui.routeComboBox->setEnabled(true);
                ui.routeButton->setEnabled(true);
        }*/
}

void ToolBox::on_simCompassSpinBox_valueChanged(int value)
{
    emit compassSpinBoxMovedS( value);
}

void ToolBox::on_zoomToolButton_clicked()
{
    qDebug()<<Q_FUNC_INFO<<ui.zoomToolButton->isChecked();
    emit zoomToolActivated(ui.zoomToolButton->isChecked());
    /*if ( status->rubberBandDrag() ) {
                status->setRubberBandDrag( false);
                status->view()->setDragMode( QGraphicsView::ScrollHandDrag);
                ui.zoomToolButton->setIcon( QIcon(":/buttons/zoom-tool.png"));
        } else {
                status->setRubberBandDrag( true);
                status->view()->setDragMode( QGraphicsView::RubberBandDrag);
                ui.zoomToolButton->setIcon( QIcon(":/buttons/hand.svg"));
        }*/
}

void ToolBox::on_lockButton_clicked()
{
    /*if ( status->boatLock() ) {
                status->setBoatLock( false);
                ui.lockButton->setIcon( QIcon(":/buttons/lock_boat_view.svg"));
                ui.lockButton->setToolTip( "Follow the boat");
                if ( status->rubberBandDrag())
                    status->view()->setDragMode( QGraphicsView::RubberBandDrag);
                else
                    status->view()->setDragMode( QGraphicsView::ScrollHandDrag);
        } else {
                ui.lockButton->setIcon( QIcon(":/buttons/unlock_boat_view.svg"));
                ui.lockButton->setToolTip( "Unlock the view from the boat");
                status->setBoatLock( true);
                status->view()->setDragMode( QGraphicsView::NoDrag);
        }*/
}

void ToolBox::on_obstaclesButton_clicked()
{
    ui.obstacles_CB->setChecked( !ui.obstacles_CB->isChecked());
}

void ToolBox::loadPolarDiagramList( int to_load) {

    if (debug) qDebug() << "ToolBox::loadPolarDiagramList(" << to_load << ")";
    //if (debug) qDebug() << "ToolBox::loadPolarDiagramList() boat: "<< pBoatInfo->name;
    //if (debug) qDebug() << "ToolBox::loadPolarDiagramList() polars: " << pBoatInfo->pdiagrams.size();
    //if (debug) qDebug() << "ToolBox::loadPolarDiagramList() default: " << pBoatInfo->default_pd;

    ui.polarDiagramNamesComboBox->clear();
    /*for ( int i = 0; i < this->pBoatInfo->pdiagrams.size(); i++ ) {
                ui.polarDiagramNamesComboBox->addItem( this->pBoatInfo->pdiagrams[i].getName() );
        }

        if ( to_load < 0 || to_load >= this->pBoatInfo->pdiagrams.size()  )
                to_load = this->pBoatInfo->default_pd;*/

    ui.polarDiagramNamesComboBox->setCurrentIndex( to_load);
    //status->setUsePolarDiagram( to_load);

}

void ToolBox::on_polarDiagramNamesComboBox_currentIndexChanged(int index)
{
    //status->setUsePolarDiagram( index);
}

/*void ToolBox::changeMultipleCheckStates(QList<Layer::LayerType> layers, bool checked){

    for(int i=0;i<layers.size();i++){
        if(layers[i] == Layer::obstacles){
            ui.obstaclesButton->setChecked(checked);
        }
        else if(layers[i] == Layer::generarea_r){
            ui.generarea_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::deptharea_r){
            ui.deptharea_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::lake_r){
            ui.lake_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::spoilgrnd_r){
            ui.spoilgrnd_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::limit_r){
            ui.limit_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::builtupare_r){
            ui.builtupare_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::fwayarea_r){
            ui.fwayarea_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::survqual_r){
            ui.survqual_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::building_r){
            ui.building_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::dock_r){
            ui.dock_rCB->setChecked(checked);
        }
        else if(layers[i] == Layer::depthcont_l){
            ui.depthcont_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::coastline_l){
            ui.coastline_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::closinglne_l){
            ui.closinglne_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::ferry_l){
            ui.ferry_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::limit_l){
            ui.limit_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::heightcont_l){
            ui.heightcont_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::leadinglne_l){
            ui.leadinglne_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::lockndam_l){
            ui.lockndam_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::pipeline_l){
            ui.pipeline_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::submcable_l){
            ui.submcable_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::roadnrailw_l){
            ui.roadnrailw_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::bridge_l){
            ui.bridge_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::transmline_l){
            ui.transmline_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::river_l){
            ui.river_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::shlnecon_l){
            ui.shlnecon_lCB->setChecked(checked);
        }
        else if(layers[i] == Layer::navaid_p){
            ui.navaid_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::limit_p){
            ui.limit_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::towernmast_p){
            ui.towernmast_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::signsound_p){
            ui.signsound_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::ctext_p){
            ui.ctext_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::vegetation_p){
            ui.vegetation_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::moorfacil_p){
            ui.moorfacil_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::natseabed_p){
            ui.natseabed_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::obstruct_p){
            ui.obstruct_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::harbour_p){
            ui.harbour_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::marservice_p){
            ui.marservice_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::wreck_p){
            ui.wreck_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::building_p){
            ui.building_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::rock_p){
            ui.rock_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::checkpoint_p){
            ui.checkpoint_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::fortstruct_p){
            ui.fortstruct_pCB->setChecked(checked);
        }
        else if(layers[i] == Layer::lights){
            ui.lightsCB->setChecked(checked);
        }
    }
}*/

void ToolBox::brightnessChanged(int bright){
    emit brightnessChange(bright);
}

void ToolBox::setAngleChecked(bool checked){
    emit angleCheckChange(checked);
}

void ToolBox::setCompassChecked(bool checked){
    emit compassCheckChange(checked);
}

void ToolBox::setgpsChecked(bool checked){
    emit gpsCheckChange(checked);
}

void ToolBox::setWindChecked(bool checked){
    emit windCheckChange(checked);
}

void ToolBox::on_routeComboBox_currentIndexChanged(int index)
{
    /*  if(index==0)
        status->setRouteOption(false);
    else
        status->setRouteOption(true);*/
}

void ToolBox::on_routeButton_clicked()
{

    /* if(status->simulationMode())
        dataSimulator->clearSimRoute();
    Route * r;
    r=Route::getInstance();
    r->addStaticWeather(dataSimulator->getSimTWD(),dataSimulator->getSimWindSpeed());

    QVector < QPointF> path=(UwMath::toConformalInverted(r->path(UwMath::fromConformalInverted( (const QPointF)status->boatPos()),status->getGeoDestiny(),status->getRouteOption())));
    //Minimum route 2 points
    if(path.size()>=2){
        if(status->simulationMode())
            dataSimulator->setSimRoute(path);
        status->setRoute(path);
    }*/
}


void ToolBox::on_destinyButton_clicked()
{
    /*status->setSettingDestiny(true);

    emit changeCursor();*/
}
