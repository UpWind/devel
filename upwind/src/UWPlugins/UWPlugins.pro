TEMPLATE = subdirs

SUBDIRS = \
    UWPluginInterface \
    Logger \
    NMEADataSimulator \
    NMEAInstruments \
    NMEALogger \
    NMEAReader \
    UpWindScene \
    NMEASerialPortReader \
    ChartProviderInterface \
    PostgreChartProvider \
    QtRenderer \
    ViewRenderer
#    RouteManager \
#    GoogleMapRenderer \
#   PostgreSQL \


QT       += svg webkit
