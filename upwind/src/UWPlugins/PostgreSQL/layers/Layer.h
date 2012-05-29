
#ifndef LAYER_H_
#define LAYER_H_

#include <QString>
#include <QGraphicsItemGroup>


class Layer : public QGraphicsItemGroup
{
public:
    enum LayerType {
		obstacles,
		generarea_r,
    	deptharea_r,
        lake_r,
    	spoilgrnd_r,
    	limit_r,
    	builtupare_r,
    	fwayarea_r,
    	survqual_r,
    	building_r,
    	dock_r,
    	depthcont_l,
    	coastline_l,
    	closinglne_l,
    	navigline_l,
    	ferry_l,
    	limit_l,
    	heightcont_l,
    	leadinglne_l,
    	lockndam_l,
    	pipeline_l,
    	submcable_l,
    	roadnrailw_l,
    	bridge_l,
    	transmline_l,
    	river_l,
    	shlnecon_l,
    	navaid_p,
    	limit_p,
    	towernmast_p,
    	signsound_p,
    	ctext_p,
    	vegetation_p,
    	moorfacil_p,
    	natseabed_p,
    	obstruct_p,
    	harbour_p,
    	marservice_p,
    	wreck_p,
    	building_p,
    	rock_p,
    	checkpoint_p,
    	fortstruct_p,
    	lights,
    	unknown
    };
    static QString toString(LayerType type);
    static LayerType fromString(QString name);
};

#endif /* LAYER_H_ */
