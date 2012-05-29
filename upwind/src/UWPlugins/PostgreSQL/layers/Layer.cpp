
#include "Layer.h"

Layer::LayerType Layer::fromString(QString name)
{
	LayerType type;
	if (name == "obstacles")
		type= obstacles;
	else if (name == "generarea_r")
		type= generarea_r;
	else if (name == "deptharea_r")
		type= deptharea_r;
	else if (name == "lake_r")
		type= lake_r;
	else if (name == "spoilgrnd_r")
		type= spoilgrnd_r;
	else if (name == "limit_r")
		type= limit_r;
	else if (name == "builtupare_r")
		type= builtupare_r;
	else if (name == "fwayarea_r")
		type= fwayarea_r;
	else if (name == "survqual_r")
		type= survqual_r;
	else if (name == "building_r")
		type= building_r;
	else if (name == "dock_r")
		type= dock_r;
	else if (name == "depthcont_l")
		type= depthcont_l;
	else if (name == "coastline_l")
		type= coastline_l;
	else if (name == "closinglne_l")
		type= closinglne_l;
	else if (name == "navigline_l")
		type= navigline_l;
	else if (name == "ferry_l")
		type= ferry_l;
	else if (name == "limit_l")
		type= limit_l;
	else if (name == "heightcont_l")
		type= heightcont_l;
	else if (name == "leadinglne_l")
		type= leadinglne_l;
	else if (name == "lockndam_l")
		type= lockndam_l;
	else if (name == "pipeline_l")
		type= pipeline_l;
	else if (name == "submcable_l")
		type= submcable_l;
	else if (name == "roadnrailw_l")
		type= roadnrailw_l;
	else if (name == "bridge_l")
		type= bridge_l;
	else if (name == "transmline_l")
		type= transmline_l;
	else if (name == "river_l")
		type= river_l;
	else if (name == "shlnecon_l")
		type= shlnecon_l;
	else if (name == "navaid_p")
		type= navaid_p;
	else if (name == "limit_p")
		type= limit_p;
	else if (name == "towernmast_p")
		type= towernmast_p;
	else if (name == "signsound_p")
		type= signsound_p;
	else if (name == "ctext_p")
		type= ctext_p;
	else if (name == "vegetation_p")
		type= vegetation_p;
	else if (name == "moorfacil_p")
		type= moorfacil_p;
	else if (name == "natseabed_p")
		type= natseabed_p;
	else if (name == "obstruct_p")
		type= obstruct_p;
	else if (name == "harbour_p")
		type= harbour_p;
	else if (name == "marservice_p")
		type= marservice_p;
	else if (name == "wreck_p")
		type= wreck_p;
	else if (name == "building_p")
		type= building_p;
	else if (name == "rock_p")
		type= rock_p;
	else if (name == "checkpoint_p")
		type= checkpoint_p;
	else if (name == "fortstruct_p")
		type= fortstruct_p;
	else if (name == "lights")
		type= lights;
	else
		type= unknown;

	return type;
}


QString Layer::toString(LayerType type)
{
	QString name;
	switch (type)
	{
	case obstacles:
		name= "obstacles";
		break;
	case generarea_r:
		name= "generarea_r";
		break;
	case deptharea_r:
		name= "deptharea_r";
		break;
	case lake_r:
		name= "lake_r";
		break;
	case spoilgrnd_r:
		name= "spoilgrnd_r";
		break;
	case limit_r:
		name= "limit_r";
		break;
	case builtupare_r:
		name= "builtupare_r";
		break;
	case fwayarea_r:
		name= "fwayarea_r";
		break;
	case survqual_r:
		name= "survqual_r";
		break;
	case building_r:
		name= "building_r";
		break;
	case dock_r:
		name= "dock_r";
		break;
	case depthcont_l:
		name= "depthcont_l";
		break;
	case coastline_l:
		name= "coastline_l";
		break;
	case closinglne_l:
		name= "closinglne_l";
		break;
	case navigline_l:
		name= "navigline_l";
		break;
	case ferry_l:
		name= "ferry_l";
		break;
	case limit_l:
		name= "limit_l";
		break;
	case heightcont_l:
		name= "heightcont_l";
		break;
	case leadinglne_l:
		name= "leadinglne_l";
		break;
	case lockndam_l:
		name= "lockndam_l";
		break;
	case pipeline_l:
		name= "pipeline_l";
		break;
	case submcable_l:
		name= "submcable_l";
		break;
	case roadnrailw_l:
		name= "roadnrailw_l";
		break;
	case bridge_l:
		name= "bridge_l";
		break;
	case transmline_l:
		name= "transmline_l";
		break;
	case river_l:
		name= "river_l";
		break;
	case shlnecon_l:
		name= "shlnecon_l";
		break;
	case navaid_p:
		name= "navaid_p";
		break;
	case limit_p:
		name= "limit_p";
		break;
	case towernmast_p:
		name= "towernmast_p";
		break;
	case signsound_p:
		name= "signsound_p";
		break;
	case ctext_p:
		name= "ctext_p";
		break;
	case vegetation_p:
		name= "vegetation_p";
		break;
	case moorfacil_p:
		name= "moorfacil_p";
		break;
	case natseabed_p:
		name= "natseabed_p";
		break;
	case obstruct_p:
		name= "obstruct_p";
		break;
	case harbour_p:
		name= "harbour_p";
		break;
	case marservice_p:
		name= "marservice_p";
		break;
	case wreck_p:
		name= "wreck_p";
		break;
	case building_p:
		name= "building_p";
		break;
	case rock_p:
		name= "rock_p";
		break;
	case checkpoint_p:
		name= "checkpoint_p";
		break;
	case fortstruct_p:
		name= "fortstruct_p";
		break;
	case lights:
		name= "lights";
		break;
	default:
		name="unknown";
	}
	return name;

}
