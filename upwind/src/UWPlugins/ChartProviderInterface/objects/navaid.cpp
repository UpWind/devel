
#define QT_NO_DEBUG_OUTPUT
#include "navaid.h"
QString spar(int mark)
{
    QString type;
    switch (mark) {
    case 1: // left
            type="left";
   //            resource=":/symbols/40_spar_left.png";
           qDebug() << "Left side spar buoy strcnav: " << mark;
    break;
    case 2: // right
               qDebug() << "Right side SPAR buoy strcnav: " << mark;
            type="right";

    break;
    case 3:
            type="north";
             qDebug() << "Northside SPAR buoy strcnav: " << mark;
    break;
    case 4:
            type="south";
            qDebug() << "Southside SPAR buoy strcnav: " << mark;
            break;
    case 5:
        type="west";
        qDebug() << "Westside SPAR buoy strcnav: " << mark;

        break;

    case 6:
         type="east";
         qDebug() << "Eastside SPAR buoy strcnav: " << mark;

         break;

    case 8:
        type="safewatermark";
        qDebug() << "Safewatermark SPAR buoy strcnav: " << mark;

        break;
    case 9:
        type="specialmark";
        qDebug() << "specialmark SPAR buoy strcnav: " << mark;
        break;
   default:
           break;
           }
    return QString(":/symbols/40_spar_%1.png").arg(type);
}

QString buoy(int mark,QString rootName)
{
//    QString resource;
    QString type;
    qDebug()<<"int mark: "<<mark;

    switch(mark)
    {
    case 1:
        //left side mark

        type = "left";
    break;
    case 2:
        //right side mark

        type = "right";
    break;
    case 3:
        type="north";
        break;
    case 4:
        type="south";
        break;
    case 5:
        type="west";
        break;
    case 6:
        type="east";
        break;
    case 8:
        type="safewatermark";
        qDebug() << "Safewatermark buoy strcnav: " << mark;

        break;
    case 9:
        type="special";
        break;
    default:
    break;
    }

    return QString(":/symbols/40_%1_buoy_%2.png").arg(rootName).arg(type);
}

NavaId::NavaId(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
     ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

NavaId::NavaId(QList<ChartObjectInterface::ChartElement> rocks, OGRLayer *feat, QString tabName)
    : ChartObjectInterface(QVector<QPolygonF>(), QVector<QPolygonF>(), feat, tabName) {
    tableName = tabName;
    featureData = feat;
    mElements = rocks;
}

NavaId::~NavaId(){}

void NavaId::setupGraphicProperties() {
    pen.setColor(Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor(Qt::black);
}

ChartObjectInterface::ChartObjectType NavaId::getType() const {
    return ChartObjectInterface::Point;
}

QVector<QPolygonF> NavaId::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> NavaId::getCoordinateGeometry() const {
    return cGeometry;
}
QPen NavaId::getPen() const {
    return pen;
}

QBrush NavaId::getBrush() const {
    return brush;
}

QString NavaId::getTableName() const {
    return tableName;
}

OGRLayer* NavaId::getFeatureData() const {
    return featureData;
}

const QString NavaId::resourceName(ChartObjectInterface::ChartElement::Attributes attributes) const {
    QString resource;

    qDebug()<<"fi_typenav: "<<attributes.value("fi_typenav");
qDebug() << "buoy ntypnavaid: " << attributes.value("ntypnavaid").toInt();
    switch (attributes.value("fi_typenav").toInt()) {
    case 9: // BUOY
        // buoy
        qDebug()<<"attributes.value(fi_strcnav):"<<attributes.value("fi_strcnav").toInt();
        switch (attributes.value("fi_strcnav").toInt()) {
        case 1:

                resource=buoy(attributes.value("ntypnavaid").toInt(),"mark");
                break;
        case 2:
                resource=buoy(attributes.value("ntypnavaid").toInt(),"ice");
                break;
        default:
        break;


        }


    break;

    //Perimeter mark
//    case 7:

    case 10: // spar buoy
//
        resource=spar(attributes.value("ntypnavaid").toInt());
        break;



    }
    qDebug()<<"return QString resource: "<<resource;
    return resource;
}
