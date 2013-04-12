#ifndef SIMULATORWHEELINTERFACE_H
#define SIMULATORWHEELINTERFACE_H

class DataSimulatorControlInterface;
class QObject;

class SimulatorWheelInterface
{
public:
    ~SimulatorWheelInterface() {};

    virtual void connectToSimulator(DataSimulatorControlInterface* simulator, QObject* simulatorObject) = 0;
};

Q_DECLARE_INTERFACE(SimulatorWheelInterface,"SimulatorWheelInterface")

#endif // SIMULATORWHEELINTERFACE_H
