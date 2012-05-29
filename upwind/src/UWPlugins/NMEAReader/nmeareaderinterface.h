#ifndef NMEAREADERINTERFACE_H
#define NMEAREADERINTERFACE_H

#include "../UWPluginInterface/uwplugininterface.h"


class CoreNMEAReader;

class NMEAReaderInterface : public UWPluginInterface
{
public:
    virtual ~NMEAReaderInterface() {}
    virtual QString getName() = 0;
    static QString generateChecksum(QString & nString);
};

Q_DECLARE_INTERFACE(NMEAReaderInterface,"NMEAReaderInterface")

#endif // NMEAREADERINTERFACE_H
