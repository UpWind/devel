#ifndef NAVIGLINEINFO_H
#define NAVIGLINEINFO_H

class NavigLineInfo
{
public:
    NavigLineInfo();
    NavigLineInfo(double depth, double height);
    NavigLineInfo(double distance, double depth, double height);
    bool empty();


    void setDistance(double distance);
    void setDepth(double depth);
    void setHeight(double height);
    void setTime(double time);

    double getMeasure(bool type);
    double getDistance();
    double getDepth();
    double getHeight();
    double getTime();

private:
    double distance;
    double depth;
    double height;
    double time;
};


#endif // NAVIGLINEINFO_H
