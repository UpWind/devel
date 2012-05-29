#include "naviglineinfo.h"

NavigLineInfo::NavigLineInfo(){
    this->distance=-1;
    this->depth=0;
    this->height=0;
    this->time=0;
}


NavigLineInfo::NavigLineInfo(double depth, double height){
    this->depth=depth;
    this->height=height;
    this->distance=-1;
    this->time=0;
}


NavigLineInfo::NavigLineInfo(double distance, double depth, double height){
    this->distance=distance;
    this->depth=depth;
    this->height=height;
}

bool NavigLineInfo::empty(){
    if(distance>0) return false;
    else return true;
}

void NavigLineInfo::setDistance(double distance){
    this->distance=distance;
}

void NavigLineInfo::setDepth(double depth){
    this->depth=depth;
}

void NavigLineInfo::setHeight(double height){
    this->height=height;
}

void NavigLineInfo::setTime(double time){
    this->time=time;
}

//type: 0=>distance
//      1=>time
double NavigLineInfo::getMeasure(bool type){
    if(type==false)
        return this->distance;
    else
        return this->time;
}

double NavigLineInfo::getDistance(){
    return this->distance;
}

double NavigLineInfo::getDepth(){
    return this->depth;
}

double NavigLineInfo::getHeight(){
    return this->height;
}

double NavigLineInfo::getTime(){
    return this->time;
}
