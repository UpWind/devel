#include "nmeaentry.h"

NMEAEntry::NMEAEntry(){}

QString NMEAEntry::getDate(){
    return date;
}

QString NMEAEntry::getNMEA(){
    return nmeastring;
}

void NMEAEntry::setDate(QString d){
    date = d;
}

void NMEAEntry::setNMEA(QString string){
    nmeastring = string;
}
