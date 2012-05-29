#include"randomnavigate.h"

randomNavigate::randomNavigate()
{
}
randomNavigate::randomNavigate(QPointF a)
{


         actual.setX(a.x());
         actual.setY(a.y());
}
void randomNavigate::setCoordinates(QPointF a) {
actual.setX(a.x());
actual.setY(a.y());
}
QPointF randomNavigate::getCoordinates(){
    return actual;

}
void randomNavigate::move(){
    if(!actual.isNull()){
randomValue = randInt(0,2);
switch (randomValue){
case 0: actual.setX(actual.x()+0.0008); break;
case 1: break;
case 2: actual.setX(actual.x()-0.0008); break;
}
randomValue = randInt(0,2);
switch (randomValue){
case 0: actual.setY(actual.y()+0.0008); break;
case 1: break;
case 2: actual.setY(actual.y()-0.0008); break;
}

    }
}

int randomNavigate::randInt(int low, int high)
    {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

