#include "savingtype.h"

SavingType::SavingType(const QString &linestr, const QString &timestr):
        name("default")
{
    line = linestr;
    time = timestr;
}

QString SavingType::getLine(){
    return line;
}
QString SavingType::getTime(){
    return time;
}

void SavingType::setName(const QString &newname){
    name = newname;
}

QString SavingType::getName(){
    return name;
}
