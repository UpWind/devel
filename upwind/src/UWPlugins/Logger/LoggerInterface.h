#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include "../UWPluginInterface/uwplugininterface.h"
#include <QObject>

/** Interface declaration of LoggerInterface, inherits UWPluginInterface
  */
class LoggerInterface: public UWPluginInterface
{
public:
    virtual ~LoggerInterface() {}

    /** Virtual function used to store a NMEA-string
    */
    virtual void appendNMEAString(QString) = 0;

    /** Virtual funtion used to do some finalizing before shutting down the program or plugin.
      */
    virtual void saveBeforeQuit() = 0;
};

Q_DECLARE_INTERFACE(LoggerInterface,"LoggerInterface")

#endif // LOGGERINTERFACE_H

