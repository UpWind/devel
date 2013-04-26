#ifndef VELOCITYHANDLEINTERFACE_H
#define VELOCITYHANDLEINTERFACE_H

class DataSimulatorControlInterface;
class QObject;

class VelocityHandleInterface
{
public:
    ~VelocityHandleInterface() {};

    virtual void connectToSimulator(DataSimulatorControlInterface* simulator, QObject* simulatorObject) = 0;
};

Q_DECLARE_INTERFACE(VelocityHandleInterface,"VelocityHandleInterface")

#endif // VELOCITYHANDLEINTERFACE_H
