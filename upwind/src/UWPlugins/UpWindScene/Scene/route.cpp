#include "route.h"
#include "../../Settings/settings.h"
#include "../shared/uwmath.h"
#include "../UpWindScene/Scene/projection.h"

#include <math.h>
#include "ogrsf_frmts.h"

//Default Chart Error (in metres)
#define CHARTERROR 15
//Point tolerance (in metres)
#define POINTTOLERANCE 10
//Minimum point tolerance (in metres)
#define NOPOINTTOLERANCE 0.001
//Max height value (in metres)
#define MAXHEIGHT 999.99
//Minimum clearance of an air way (bride/transmission line) (in metres)
#define AIRWAYCLEARANCE 0.2
//Error generating the airway/navigation line intersection (in metres)
#define AIRWAYERROR 0.1



Route* Route::instance = NULL;

Route* Route::getInstance(){
        if(instance == NULL) {
                instance = new Route();
                return instance;
        } else
                return instance;
}

Route::Route(){
    debug = true;


    adMatrix.clear();
    nodes.clear();
    edges.clear();
    airway.clear();


}

Route::~Route(){
    if ( instance != NULL ){
        clear();
        instance=NULL;
    }
}

void Route::clear(){

    adMatrix.clear();
    nodes.clear();
    edges.clear();
    airway.clear();
}

void Route::initiate(){
QTime time1;
time1.start();
    this->getBoatInfo();
    this->setNavigLinesCorrectedFromDB();
    //this->settings = Settings::getSettings();
    edges.clear(); //why clear now?
    qDebug() << QString("Route:: database: %1 ms ").arg( QString::number(time1.elapsed(), 10));
}
void Route::getBoatInfo() { //these are test values, need to implement boat.class

    this->boatDraught =1;
    this->boatHeight = 1;
}
QVector<QPolygonF>  Route::getNavigLinesCorrected() {
    return this->navlinegeom;
}


void Route::loadChartObjects(QVector<ChartObjectInterface*> cObjects) {
    //this->clear();
    this->chartObjects = cObjects;
    this->initiate();

}

void Route::FilterAirWay(){
    setBridgesFromDB();
    setTransmissionLinesFromDB();

    airway.clear();
}

void Route::setTransmissionLinesFromDB(){ //

    OGRLayer* layer;
    QVector<float> ver;
    QVector<QPolygonF> wkb;

    for (int i = 0 ; i < this->chartObjects.size(); i++) {

        if(this->chartObjects.at(i)->getTableName() == "transmline_l") {

            layer = this->chartObjects.at(i)->getFeatureData();
            OGRFeatureDefn *poFDefn;
            OGRFeature *poFeature;
            wkb = this->chartObjects.at(i)->getCoordinateGeometry();
            layer->ResetReading();

            for(int m= 0; m < layer->GetFeatureCount();m++ ) {

               poFDefn = layer->GetLayerDefn();
               poFeature = layer->GetNextFeature();

               for(int i = 0; i < poFDefn->GetFieldCount(); i++ ){
                   OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( i );
                    QString str = poFieldDefn->GetNameRef() ;
                    ver.append(1.00); // fake value
                    if (str.contains("vercsa") == true) {
                        ver.append(poFeature->GetFieldAsDouble(i)); //This column no longer in table, need to fix this somehow
                    }

                }

            }
        }
    }

    for ( int l = 0; l < wkb.size(); l++ ) {

        QPolygonF line = wkb.at(l);

        float verccl = ver.at(l);

        for(int k = 0 ; k < line.size()-1 ; k++){
            addAirWay(verccl,0,QLineF(QPointF(Projection::merc_x(QPointF(line.at(k)).x()),Projection::merc_y(QPointF(line.at(k)).y())),QPointF(Projection::merc_x(QPointF(line.at(k+1)).x()),Projection::merc_y(QPointF(line.at(k+1)).y()))));
        }

    }



}


void Route::setBridgesFromDB(){

    OGRLayer* layer;
    QVector<double> hor;
    QVector<double> ver;
    QVector<QPolygonF> wkb;

    for (int i = 0 ; i < this->chartObjects.size(); i++) {

        if(this->chartObjects.at(i)->getTableName() == "bridge_l") {

            layer = this->chartObjects.at(i)->getFeatureData();
            OGRFeatureDefn *poFDefn;
            OGRFeature *poFeature;
            wkb = this->chartObjects.at(i)->getCoordinateGeometry();

            for(int m= 0; m < layer->GetFeatureCount();m++ ) {

               poFDefn = layer->GetLayerDefn();
               poFeature = layer->GetNextFeature();
               layer->ResetReading();

               for(int i = 0; i < poFDefn->GetFieldCount(); i++ ){
                   OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( i );
                    QString str = poFieldDefn->GetNameRef() ;

                    if(str.contains("horclr") == true) {

                        hor.append(poFeature->GetFieldAsDouble(i));


                    }
                    else if (str.contains("verccl") == true) {
                        ver.append(poFeature->GetFieldAsDouble(i));
                    }

                }

            }
        }
    }

    for ( int l = 0; l < wkb.size(); l++ ) { //TODO: Currently does not take in to account "lifting" bridges

        QPolygonF line = wkb.at(l);
        float horclr = hor.at(l);
        float verccl = ver.at(l);

        for(int k = 0 ; k < line.size()-1 ; k++){
            addAirWay(verccl,horclr,QLineF(QPointF(Projection::merc_x(QPointF(line.at(k)).x()),Projection::merc_y(QPointF(line.at(k)).y())),QPointF(Projection::merc_x(QPointF(line.at(k+1)).x()),Projection::merc_y(QPointF(line.at(k+1)).y()))));
        }

    }
}

void Route::setNavigLinesCorrectedFromDB(){

    if (!this->navlinegeom.isEmpty()) {
        for ( int l = 0; l < this->navlinegeom.size(); l++ ) { //TODO: Currently does not take in to account "lifting" bridges

            QPolygonF line = this->navlinegeom.at(l);
            float navlinedep = this->navlinedep.at(l);
            float navlineheight = this->navlineheight.at(l);


            if(navlinedep>=this->boatDraught){
                for(int k=0;k<line.size()-1;k++){
                    addEdge(navlinedep,navlineheight,QPointF(Projection::merc_x(QPointF(line.at(k)).x()),Projection::merc_y(QPointF(line.at(k)).y())),QPointF(Projection::merc_x(QPointF(line.at(k+1)).x()),Projection::merc_y(QPointF(line.at(k+1)).y())),false);
                }
            }
        }
    } else {
       createNavigLinesCorrected();
    }
}

bool Route::createNavigLinesCorrected(){

    bool end = false;
    OGRLayer* layer;
    QVector<float> ndep;
    QVector<QPolygonF> ngeom;


    for (int i = 0 ; i < this->chartObjects.size(); i++) {
        // QString tableName = this->chartObjects.at(i)->getTableName();
        if(this->chartObjects.at(i)->getTableName().compare("navigline_l") == 0) {

            layer = this->chartObjects.at(i)->getFeatureData();
            OGRFeatureDefn *poFDefn;
            OGRFeature *poFeature;
            ngeom = this->chartObjects.at(i)->getCoordinateGeometry();
            layer->ResetReading();


            for(int m= 0; m < layer->GetFeatureCount();m++ ) {

               poFDefn = layer->GetLayerDefn();
               poFeature = layer->GetNextFeature();


               for(int i = 0; i < poFDefn->GetFieldCount(); i++ ){
                   OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( i );
                   QString str = poFieldDefn->GetNameRef() ;
                   ndep.append(1.00); //fake value to get something in
                   if(str.contains("navlnedep") == true) {
                        ndep.append(poFeature->GetFieldAsDouble(i)); //problem here is now that DB navigline_l_layer5 does not contain navlnedep field. It is in the original navligline table but not in the reconstructured table.
                   }

                }
            }
        }
    }
    for ( int l = 0; l < ngeom.size(); l++ ) {

        QPolygonF line = ngeom.at(l);
        float navlinedep = ndep.at(l);

        for(int k=0;k<line.size()-1;k++){
            addEdge(navlinedep,QPointF(Projection::merc_x(QPointF(line.at(k)).x()),Projection::merc_y(QPointF(line.at(k)).y())),QPointF(Projection::merc_x(QPointF(line.at(k+1)).x()),Projection::merc_y(QPointF(line.at(k+1)).y())),true);
        }
    }

    if (intersections(CHARTERROR)){

        FilterAirWay();

        for(int i=0;i<adMatrix.size();i++){
            for(int j=i+1;j<adMatrix.size();j++){
                if(!adMatrix[i][j].empty()){

                    //sql.append("INSERT INTO navigline_l_corrected (wkb_geometry,navlinedep,navlineheight) VALUES (");

                    QVector<QPointF> vectorLine;
                    vectorLine.append(QPointF(Projection::merc_lon(nodes[i].x()),Projection::merc_lat(nodes[i].y())));
                    vectorLine.append(QPointF(Projection::merc_lon(nodes[j].x()),Projection::merc_lat(nodes[j].y())));
                    QPolygonF line(vectorLine);
                    this->navlinegeom.append(line);
                    //sql.append(QString::number(adMatrix[i][j].getDepth(),'f',2));
                    this->navlinedep.append(adMatrix[i][j].getDepth());
                    //sql.append(QString::number(adMatrix[i][j].getHeight(),'f',2));
                    this->navlineheight.append(adMatrix[i][j].getHeight());
                }
            }
        }
        end =true;
    }


    //adMatrix.resize(0);//testing, this was not commented out in original code
   //nodes.resize(0); //this clears nodes which are needed for path calculation. need to check the order at which calculations happens.
    //edges.resize(0); //same as above
    //airway.resize(0); // same as above

    if (end==true){
        if(debug)qDebug() << QString("Route::navigline_l_corrected table created");
    }else{
        if(debug)qDebug() << QString("ERROR Route::impossible to create navigline_l_corrected table: too many edges");
    }
    return end;
}

void Route::addAirWay(double height,double width, QLineF line){

    addAirWay(height, buildRectangle(line,width+AIRWAYCLEARANCE));

}


void Route::addAirWay(double height, QPainterPath area){

    airway.append(area);

    int N = edges.size();
    int i = 0;

    while(N > 0){
        //Transform the edge into an area. Only for the calculations
        double m = getSlope(edges[i]);
        double b = edges[i].p1().y()-m*edges[i].p1().x();
        QVector <QPointF> pp = getPerpendicularPoints(m,b,AIRWAYERROR,edges[i].p1());

        QPainterPath edge(edges[i].p1());
        edge.lineTo(QPointF(edges[i].p2()));
        edge.lineTo(pp[0]);
        edge.closeSubpath();

        QPainterPath intersection= edge.intersected(area);
        if(!intersection.isEmpty()){
            if (intersection.elementAt(0)==edges[i].p1() && intersection.elementAt(1)==edges[i].p2()){
                //The edge is inside the area
                int starti,endi=-1;
                isPoint(edges[i].p1(),starti,false);
                isPoint(edges[i].p2(),endi,false);
                adMatrix[starti][endi].setHeight(height);
                adMatrix[endi][endi].setHeight(height);

            }else if(intersection.elementAt(0)==edges[i].p1()){
                //p1 is inside, and p2 is outside the area
                int starti,endi=-1;
                isPoint(edges[i].p1(),starti,false);
                isPoint(edges[i].p2(),endi,false);
                NavigLineInfo aux=adMatrix[starti][endi];
                QPointF p1=edges[i].p1();
                QPointF p2=edges[i].p2();

                deleteEdge(edges[i].p1(),edges[i].p2(),false);
                i--;
                addEdge(aux.getDepth(),height,p1,intersection.elementAt(1),false);
                addEdge(aux.getDepth(),aux.getHeight(),intersection.elementAt(1),p2,false);

            }else if(intersection.elementAt(1)==edges[i].p2()){
                //p2 is inside, and p1 is outside the area
                int starti,endi=-1;
                isPoint(edges[i].p1(),starti,false);
                isPoint(edges[i].p2(),endi,false);
                NavigLineInfo aux=adMatrix[starti][endi];
                QPointF p1=edges[i].p1();
                QPointF p2=edges[i].p2();

                deleteEdge(edges[i].p1(),edges[i].p2(),false);
                i--;
                addEdge(aux.getDepth(),aux.getHeight(),p1,intersection.elementAt(0),false);
                addEdge(aux.getDepth(),height,intersection.elementAt(0),p2,false);
            }else{
                //p1 and p2 are outside the area
                QPointF auxpoint;
                if(edges[i].intersect(QLineF(QPointF(area.elementAt(0)),QPointF(area.elementAt(1))),&auxpoint)==QLineF::BoundedIntersection ||
                   edges[i].intersect(QLineF(QPointF(area.elementAt(1)),QPointF(area.elementAt(2))),&auxpoint)==QLineF::BoundedIntersection ||
                   edges[i].intersect(QLineF(QPointF(area.elementAt(2)),QPointF(area.elementAt(3))),&auxpoint)==QLineF::BoundedIntersection ||
                   edges[i].intersect(QLineF(QPointF(area.elementAt(3)),QPointF(area.elementAt(0))),&auxpoint)==QLineF::BoundedIntersection){
                    int starti,endi=-1;
                    isPoint(edges[i].p1(),starti,false);
                    isPoint(edges[i].p2(),endi,false);
                    NavigLineInfo aux=adMatrix[starti][endi];
                    QPointF p1=edges[i].p1();
                    QPointF p2=edges[i].p2();

                    deleteEdge(edges[i].p1(),edges[i].p2(),false);
                    i--;
                    addEdge(aux.getDepth(),aux.getHeight(),p1,intersection.elementAt(0),false);
                    addEdge(aux.getDepth(),height,intersection.elementAt(0),intersection.elementAt(1),false);
                    addEdge(aux.getDepth(),aux.getHeight(),intersection.elementAt(1),p2,false);
                }

            }

        }
        N--;
        i++;
    }
}
bool Route::isPoint(QPointF a,int &position,bool tolerance ){

    position=-1;
    for(int i=0;i<nodes.size();i++){
        if (samePoint(nodes[i],a,tolerance)){
            position=i;
            return true;
        }
    }
    return false;
}

QPainterPath Route::buildRectangle(QLineF line,double distance){

    QVector <QPointF> p1;
    QVector <QPointF> p2;

    double m = getSlope(line);

    double b = line.p1().y() - m*line.p1().x();

    p1 = getPerpendicularPoints(m,b,distance/2,line.p1());
    p2 = getPerpendicularPoints(m,b,distance/2,line.p2());

    QPainterPath area(p1[0]);
    area.lineTo(p2[0]);
    area.lineTo(p2[1]);
    area.lineTo(p1[1]);
    area.closeSubpath();

    return area;
}

double Route::getSlope(QLineF line){
    double m;
    if((line.p1().x()-line.p2().x()) == 0 )
        m = 999999999;
    else
        m = (line.p1().y()-line.p2().y()) / (line.p1().x()-line.p2().x());
    return m;
}


QVector <QPointF> Route::getPerpendicularPoints(double m,double b,double distance,QPointF p){
    //Based on http://es.wikipedia.org/wiki/Distancia_de_un_punto_a_una_recta
    QVector <QPointF> points;
    points.resize(2);

    if(m>=999999999){//Vertical
        points[0].setX(p.x()+distance);
        points[0].setY(p.y());
        points[1].setX(p.x()-distance);
        points[1].setY(p.y());
        return points;
    }

    if(m==0){//Horizontal
        points[0].setX(p.x());
        points[0].setY(p.y()+distance);
        points[1].setX(p.x());
        points[1].setY(p.y()-distance);
        return points;
    }

    double m2=-1/m;
    double b2=p.y() - m2*p.x();

    points[0].setX(( distance*sqrt(pow(m,2)+1) + b2 -b )/(m-m2));
    points[0].setY(m2*points[0].x() + b2);
    distance=-distance;
    points[1].setX(( distance*sqrt(pow(m,2)+1) + b2 -b )/(m-m2));
    points[1].setY(m2*points[1].x() + b2);
    return points;
}

bool Route::addEdge(double depth, QPointF startNode, QPointF endNode,bool tolerance){
    return addEdge(depth,MAXHEIGHT,startNode,endNode,tolerance);
}

bool Route::addEdge(double depth,double height, QPointF startNode, QPointF endNode,bool tolerance){
    // startNode = this->pixelToGeopoint(startNode);
     //endNode = this->pixelToGeopoint(endNode);

     int start,end=-1;
    if (!isEdge(startNode,endNode,tolerance) ){
        if(!isPoint(startNode,start,tolerance)){
            nodes.append(startNode);
            start=nodes.size()-1;
            for (int i = 0; i < adMatrix.size(); i++){
               adMatrix[i].append(NavigLineInfo());
            }
            adMatrix.append(QVector<NavigLineInfo>(adMatrix.size()+1,NavigLineInfo()));
            adMatrix[start][start].setDistance(0);
        }
        if (!isPoint(endNode,end,tolerance)){
            nodes.append(endNode);
            end=nodes.size()-1;
            for (int i = 0; i < adMatrix.size(); i++){
                 adMatrix[i].append(NavigLineInfo());
            }
             adMatrix.append(QVector<NavigLineInfo>(adMatrix.size()+1,NavigLineInfo()));
             adMatrix[end][end].setDistance(0);
        }
        if(start==end)return false;
        NavigLineInfo edge(distanceHaversine(nodes[start],nodes[end]),depth,height);
        adMatrix[start][end]=edge;
        adMatrix[end][start]=edge;
        edges.append(QLineF(nodes[start],nodes[end]));
        return true;
    }
    return false;
}
bool Route::isEdge(QPointF startNode, QPointF endNode,bool tolerance){

    int start,end=-1;
    if (isPoint(startNode,start,tolerance) && isPoint(endNode,end,tolerance)){
        if(start==end){
            return true;
        }
        for(int i=0;i<edges.size();i++){
            if(sameSegment(edges[i],QLineF(nodes[start],nodes[end])))
                return true;
        }
    }
    return false;
}


double Route::distanceHaversine(QPointF start, QPointF end){
    start.setX(Projection::merc_lon(start.x()));
    start.setY(Projection::merc_lat(start.y()));
    end.setX(Projection::merc_lon(end.x()));
    end.setY(Projection::merc_lat(end.y()));
    double dec_rad=M_PI/180;
    double radius=RADIUSAVERAGE;

    double lon1=start.x()*dec_rad;
    double lat1=start.y()*dec_rad;
    double lon2=end.x()*dec_rad;
    double lat2=end.y()*dec_rad;

    double dLat=lat2-lat1;
    double dLon=lon2-lon1;

    double a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * +sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    double d = radius * c;
    return d;
}

void Route::deleteEdge(QPointF startNode, QPointF endNode,bool tolerance){
    int start,end=-1;
    int position=-1;

    if (isPoint(startNode,start,tolerance) && isPoint(endNode,end,tolerance)){
         adMatrix[start][end]=NavigLineInfo();
         adMatrix[end][start]=NavigLineInfo();
         position=edges.indexOf(QLineF(nodes[start],nodes[end]));
         if(position==-1)
             position=edges.indexOf(QLineF(nodes[end],nodes[start]));
         edges.remove(position);
    }
}
bool Route::samePoint(QPointF a,QPointF b,bool tolerance ){
    double error=POINTTOLERANCE;
    if(tolerance==false){
        error=NOPOINTTOLERANCE;
        if(distance(a,b)<=error){
            return true;
        }
        return false;
    }

    if(distanceHaversine(a,b)<=error){
        return true;
    }
    return false;
}

bool Route::sameSegment(QLineF ab, QLineF cd){
    if(ab.p1()==cd.p1() &&ab.p2()==cd.p2())
        return true;
    if(ab.p1()==cd.p2() &&ab.p2()==cd.p1())
        return true;
    return false;
}

double Route::distance(QPointF start, QPointF end){
    return sqrt(pow(end.x()-start.x(),2)+pow(end.y()-start.y(),2));
}



bool Route::intersections(double error){

    //Only for debug: the new points generated
    QVector <QPointF> points(0);

    QLineF::IntersectType intersectionType;
    int N=edges.size();
    int NOriginal=N;
    int count=0;
    int i=0;
    int j=0;
    bool deci=false;
    bool decj=false;
    bool erri=false;
    bool errj=false;

    while(i<N){
        j=i+1;
        deci=false;
        while(j<N && deci==false){
            QPointF point;

            decj=false;

            intersectionType=edges[i].intersect(edges[j],&point);
            if ((intersectionType==(QLineF::BoundedIntersection)) && (newcross(edges[i],edges[j],point))){
                int starti,endi,startj,endj=-1;
                isPoint(edges[i].p1(),starti,true);
                isPoint(edges[i].p2(),endi,true);
                isPoint(edges[j].p1(),startj,true);
                isPoint(edges[j].p2(),endj,true);
                double depthi=adMatrix[starti][endi].getDepth();
                double depthj=adMatrix[startj][endj].getDepth();
                double heighti=adMatrix[starti][endi].getHeight();
                double heightj=adMatrix[startj][endj].getHeight();

                if(points.indexOf(point)==-1){
                    points.append(point);
                }
                if(!samePoint(edges[i].p1(),point,true) && !samePoint(edges[i].p2(),point,true)){
                    addEdge(depthi,heighti,edges[i].p1(),point,true);
                    addEdge(depthi,heighti,point,edges[i].p2(),true);
                         deci=true;
                }
                if(!samePoint(edges[j].p1(),point,true) && !samePoint(edges[j].p2(),point,true)){
                    addEdge(depthj,heightj,edges[j].p1(),point,true);
                    addEdge(depthj,heightj,point,edges[j].p2(),true);
                     decj=true;
                }

                if(deci){
                    deleteEdge(edges[i].p1(),edges[i].p2(),true);
                    i--;
                    j--;
                }
                if(decj){
                     deleteEdge(edges[j].p1(),edges[j].p2(),true);
                    j--;
                }
                N=edges.size();

                count++;

            }else if((error>0)&&(intersectionType==(QLineF::UnboundedIntersection))){
                QLineF newline;
                QLineF newline2;
                if(newcross(edges[i],edges[j],point)){//newcross(newline,newline2,point)){

                    if(belongtonewsegment(point,edges[i],error,&newline) && belongtonewsegment(point,edges[j],error,&newline2)){

                        int starti,endi,startj,endj=-1;
                        isPoint(edges[i].p1(),starti,true);
                        isPoint(edges[i].p2(),endi,true);
                        isPoint(edges[j].p1(),startj,true);
                        isPoint(edges[j].p2(),endj,true);
                        double depthi=adMatrix[starti][endi].getDepth();
                        double depthj=adMatrix[startj][endj].getDepth();
                        double heighti=adMatrix[starti][endi].getHeight();
                        double heightj=adMatrix[startj][endj].getHeight();

                        erri=false;
                        errj=false;

                        if(!sameSegment(edges[i],newline)){
                            if(samePoint(newline.p1(),newline.p2(),true)){//The new segment is shorter than the error of the program
                                erri=true;
                            }else{
                                erri=false;

                                if (addEdge(depthi,heighti,newline.p1(),newline.p2(),true)==true)
                                    if(points.indexOf(point)==-1){
                                      points.append(point);
                                    }
                            }
                        }else{//Intersection within the segment
                            if(!samePoint(edges[i].p1(),point,true)&& !samePoint(point,edges[i].p2(),true)){
                                addEdge(depthi,heighti,edges[i].p1(),point,true);
                                addEdge(depthi,heighti,point,edges[i].p2(),true);
                                deci=true;
                            }else erri=true;
                        }
                        if(!sameSegment(edges[j],newline2)){
                            if(samePoint(newline2.p1(),newline2.p2(),true)){//The new segment is shorter than the error of the program
                                errj=true;
                            }else{
                                errj=false;

                                if(addEdge(depthj,heightj,newline2.p1(),newline2.p2(),true)==true)
                                    if(points.indexOf(point)==-1){
                                      points.append(point);
                                    }
                            }

                        }else{
                            if(!samePoint(edges[j].p1(),point,true)&& !samePoint(point,edges[j].p2(),true)){
                                addEdge(depthj,heightj,edges[j].p1(),point,true);
                                addEdge(depthj,heightj,point,edges[j].p2(),true);
                                decj=true;
                            }else errj=true;
                        }

                        if(erri && errj){
                            if(sameSegment(newline,edges[i])){
                              //  if(newline2.p2()==edges[j].p1()){
                               //     addEdge(max(depthi,depthj),min(heighti,heightj),newline.p1(),edges[j].p1(),true);
                              //  }else if(newline2.p2()==edges[j].p2()){
                              //      addEdge(max(depthi,depthj),min(heighti,heightj),newline.p1(),edges[j].p2(),true);
                              //  }
                                addEdge(max(depthi,depthj),min(heighti,heightj),newline.p1(),newline2.p2(),true);
                            }else if(sameSegment(newline2,edges[j])){
                               // if(newline.p2()==edges[i].p1()){
                                //    addEdge(max(depthi,depthj),min(heighti,heightj),newline2.p1(),edges[i].p1(),true);
                               // }else if(newline.p2()==edges[i].p2()){
                               //     addEdge(max(depthi,depthj),min(heighti,heightj),newline2.p1(),edges[i].p2(),true);
                               // }
                                addEdge(max(depthi,depthj),min(heighti,heightj),newline2.p1(),newline.p2(),true);
                            }else{
                                //if(newline.p2()==edges[i].p1()){
                                  //  if(newline2.p2()==edges[j].p1()){
                                    //    addEdge(max(depthi,depthj),min(heighti,heightj),edges[i].p1(),edges[j].p1(),true);
                                   // }else if(newline2.p2()==edges[j].p2()){
                                    //    addEdge(max(depthi,depthj),min(heighti,heightj),edges[i].p1(),edges[j].p2(),true);
                                   // }
                               // }else if(newline.p2()==edges[i].p2()){
                                //    if(newline2.p2()==edges[j].p1()){
                                 //       addEdge(max(depthi,depthj),min(heighti,heightj),edges[i].p2(),edges[j].p1(),true);
                                  //  }else if(newline2.p2()==edges[j].p2()){
                                   //     addEdge(max(depthi,depthj),min(heighti,heightj),edges[i].p2(),edges[j].p2(),true);
                                   // }
                                //}
                                addEdge(min(depthi,depthj),min(heighti,heightj),newline.p2(),newline2.p2(),true);
                            }
                        }

                        if(deci){
                            deleteEdge(edges[i].p1(),edges[i].p2(),true);
                            i--;
                            j--;
                        }
                        if(decj){
                             deleteEdge(edges[j].p1(),edges[j].p2(),true);
                            j--;
                        }

                        N=edges.size();
                        count++;
                    }
                }
            }
            //Error: charterror very big in relation with the pointtolerance
            if(NOriginal*2<=N){points.resize(-1);return false;}
            j++;
        }
        i++;
    }


    return true;
}


bool Route::newcross(QLineF ab,QLineF cd, QPointF point){
    QPointF a=ab.p1();
    QPointF b=ab.p2();
    QPointF c=cd.p1();
    QPointF d=cd.p2();
   //check the error float:
   if(a==b || a==c || a==d || b==c || b==d || c==d)
       return false;
   if((a== point || b== point)&&(c!=point&&d!=point))
        return true;
   if((c== point || d== point)&&(a!=point&&b!=point))
        return true;

   if((a!= point && b!= point)&&(c!=point&&d!=point))
        return true;
   return false;
}

//point= intersection point
//We assume that "point" belongs to the line formed by the segment
//Output:   If "point" belongs to the segment, newline= (end of the segment closest to "point", end of the segment farthest to "point")
//          Else, newline=(point,end of the segment)
bool Route::belongtonewsegment(QPointF point,QLineF segment, double error,QLineF *newline){
    QPointF a=segment.p1();
    QPointF b=segment.p2();
    double epsilon=error;
    double toA=distanceHaversine(point,a);
    double toB=distanceHaversine(point,b);

    if(belong(point,segment)){ //Aqu da fallo debido al bug de qt
        if(toA<toB)
            newline->setPoints(a,b);
        else
            newline->setPoints(b,a);
        return true;
    }else{
        if (toA< epsilon && toB>=epsilon){
            newline->setPoints(point,a);
            return true;
        }else if(toB< epsilon && toA>=epsilon){
            newline->setPoints(point,b);
            return true;
        }else if(toA< epsilon && toB< epsilon){
            if (toA<toB){
                newline->setPoints(point,a);
            }else{
                newline->setPoints(point,b);
            }
            return true;
        }else{
            return false;
        }
    }
}

//A point belongs to a segment?
bool Route::belong(QPointF point, QLineF segment){
    QPointF a=segment.p1();
    QPointF b=segment.p2();

    double m=(b.y()-a.y())/(b.x()-a.x());
    double k=a.y()-m*a.x();
    double error=NOPOINTTOLERANCE;

    double check=point.y()-m*point.x()-k;

    if(check<=error && check >=-error)
        if((min(a.x()-error,b.x()-error)<= point.x()) && (point.x()<= max(a.x()+error,b.x()+error)))
            if((min(a.y()-error,b.y()-error)<=point.y()) &&(point.y()<=max(a.y()+error,b.y()+error)))
                return true;
    return false;
}

qreal Route::min(qreal a, qreal b){
    if(a<b)
        return a;
    else
        return b;
}

qreal Route::max(qreal a, qreal b){
    if(a>b)
        return a;
    else
        return b;
}

QVector < QPointF> Route::path( QPointF startPoint,QPointF endPoint,bool type){

 //   QTime time1;
 //   QTime time2;
 //   QTime time3;
//Dijkstra: Single Source Shortest path modified to convert it in Single Pair Shortest Path
//Source http://compprog.wordpress.com/2007/12/01/one-source-shortest-path-dijkstras-algorithm/
    QVector <QPointF> path;
 //   time1=time1.currentTime();
 //   time2=time2.currentTime();

    int start=-1;
    int end=-1;
    start=nearestNode(QPointF(Projection::merc_x(startPoint.x()),Projection::merc_y(startPoint.y()))); //checks which is nearest node for startpoint (current boat location)
    end=nearestNode(QPointF(Projection::merc_x(endPoint.x()),Projection::merc_y(endPoint.y()))); // checks which is nearest node for endpoint (destination), returns array pointer index

    if(start!=-1 && end!=-1)
    {
        int N=adMatrix.size();
        double INF=99999999;
        QVector<bool> visited(N,false);
        QVector<double> d(N,INF);
        QVector<int> prev(N,-1);
        int i, k, mini;

        d[start] = 0;

        for (k = 0; k < N; ++k)
        {
            mini = -1;
            for (i = 0; i < N; ++i)
                if (!visited[i] && ((mini == -1) || (d[i] < d[mini])))
                    mini = i;

            visited[mini] = true;
            if(mini==end)
                break; //che
            for (i = 0; i < N; ++i)
                if (adMatrix[mini][i].getDistance()>0 && adMatrix[mini][i].getDepth()>=this->boatDraught && adMatrix[mini][i].getHeight()>=this->boatHeight )
                    if (d[mini] + adMatrix[mini][i].getMeasure(type) < d[i])
                    {
                        d[i] = d[mini] + adMatrix[mini][i].getMeasure(type);
                        prev[i]=mini;
                    }
        }

       // time2=time2.currentTime();

        int j=end;
        //double distance=0;
        //double time=0;
        while(prev[j]!=-1)
        {
            path.prepend(QPointF(Projection::merc_lon(nodes[j].x()),Projection::merc_lat(nodes[j].y())));
            //distance+=adMatrix[prev[j]][j].getDistance();
            //time+=adMatrix[prev[j]][j].getTime();
            j=prev[j];
        }
        path.prepend(QPointF(Projection::merc_lon(nodes[j].x()),Projection::merc_lat(nodes[j].y())));
   }

  // time3=time3.currentTime();
  // qDebug() << QString("Route:: Dijkstra: %1 ms ").arg( QString::number(time1.msecsTo(time2),10));
 //  qDebug() << QString("Route:: Generate path: %1 ms ").arg( QString::number(time2.msecsTo(time3),10));
   this->routePoints = path;

   return path;
}

QPolygonF Route::getRoute(){

    return this->routePoints;
}

int Route::nearestNode( QPointF point){
    int position=-1;
    double min=9999999;
    double distance;

    for(int i=0;i<nodes.size();i++){
        distance=distanceHaversine(point,nodes[i]);
        if(distance<min){
            min=distance;
            position=i;
        }
    }
    return position;
}

//NO function call to this in this piece of software!
//void Route::addStaticWeather(double angleWind,double speedWind){ //this method is currently commeted out since there is a new version of weather simulation coming.
//    (void)angleWind;
//    (void)speedWind;
//}
void Route::addStaticWeather(double angleWind,double speedWind)
{ //this method is currently commeted out since there is a new version of weather simulation coming.

    QTime time1;
    QTime time2;
    if(debug)time1=time1.currentTime();

    PolarDiagram pDiagram;
    pDiagram.populateWithFinngulf36();
    qDebug()<<"Add static weather called";

    for(int i=0;i<adMatrix.size();i++){
        for(int j=0;j<adMatrix.size();j++){
            if(i!=j && !adMatrix[i][j].empty()){

                double twa=UwMath::getTWA(QPointF(Projection::merc_lon(nodes[i].x()),Projection::merc_lat(nodes[i].y())),QPointF(Projection::merc_lon(nodes[j].x()),Projection::merc_lat(nodes[j].y())),angleWind);

                if(twa<=pDiagram.getBeatAngle(speedWind)){
                    //Beating
                    QPointF tack=getTackPoint(nodes[i],nodes[j],angleWind,pDiagram.getBeatAngle(speedWind));
                    double auxtemp=distanceHaversine(nodes[i],tack)/(pDiagram.getVelocity(speedWind,pDiagram.getBeatAngle(speedWind))*0.5144);
                    auxtemp+=distanceHaversine(tack,nodes[j])/(pDiagram.getVelocity(speedWind,pDiagram.getBeatAngle(speedWind))*0.5144);
                    adMatrix[i][j].setTime(auxtemp);

                }else if(twa>=pDiagram.getGybeAngle(speedWind)){
                    //Gybing
                     QPointF tack=getTackPoint(nodes[i],nodes[j],angleWind,pDiagram.getGybeAngle(speedWind));
                    double auxtemp=distanceHaversine(nodes[i],tack)/(pDiagram.getVelocity(speedWind,pDiagram.getGybeAngle(speedWind))*0.5144);
                    auxtemp+=distanceHaversine(tack,nodes[j])/(pDiagram.getVelocity(speedWind,pDiagram.getGybeAngle(speedWind))*0.5144);
                    adMatrix[i][j].setTime(auxtemp);

                }else if(twa>=pDiagram.getGybeAngle(speedWind)){
                    //Gybing
                     QPointF tack=getTackPoint(nodes[i],nodes[j],angleWind,pDiagram.getGybeAngle(speedWind));
                    double auxtemp=distanceHaversine(nodes[i],tack)/(pDiagram.getVelocity(speedWind,pDiagram.getGybeAngle(speedWind))*0.5144);
                    auxtemp+=distanceHaversine(tack,nodes[j])/(pDiagram.getVelocity(speedWind,pDiagram.getGybeAngle(speedWind))*0.5144);
                    adMatrix[i][j].setTime(auxtemp);
                }else{
                    //Reaching
                    adMatrix[i][j].setTime(distanceHaversine(nodes[i],nodes[j])/(pDiagram.getVelocity(speedWind,twa)*0.5144));
                }
            }
        }
    }
//    if(debug)time2=time2.currentTime();
//    if(debug)qDebug() << QString("Route::addStaticWeather:: Time to add Static Weather %2, %3 knots: %1 ms ").arg( QString::number(time1.msecsTo(time2),10),QString::number(angleWind,'f',2),QString::number(speedWind,'f',2));
}

QPointF Route::getTackPoint(QPointF p1,QPointF p2,double twd,double angle)
{

    double qtchange=fmod(270-twd,360);

    QLineF line1(p1,p2);
    line1.setAngle(fmod(qtchange+angle,360));

    QLineF line2(p2,p1);
    line2.setAngle(fmod(qtchange-angle,360));

    QPointF pointaux;
    line1.intersect(line2,&pointaux);
    return pointaux;
}
