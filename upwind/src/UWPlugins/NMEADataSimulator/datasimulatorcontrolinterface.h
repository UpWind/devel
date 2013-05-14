#ifndef DATASIMULATORCONTROLINTERFACE_H
#define DATASIMULATORCONTROLINTERFACE_H

class DataSimulatorControlInterface
{
public:
    virtual ~DataSimulatorControlInterface() {};

    virtual void setTurningSpeed(int degreesPerSecond) = 0;

    virtual void setVelocityMultiplier(float Velocitymultiplier) = 0;

    virtual void setAnchor(bool anchor) = 0;

    virtual void setSail(bool sailing) = 0;

    virtual operator QObject*() = 0;
};

Q_DECLARE_INTERFACE(DataSimulatorControlInterface,"DataSimulatorControlInterface")

#endif // DATASIMULATORCONTROLINTERFACE_H
