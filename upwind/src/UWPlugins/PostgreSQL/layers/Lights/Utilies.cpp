/*
 * Utilies.cpp
 *
 *  Created on: Oct 28, 2008
 *      Author: markus
 */

#include "Utilies.h"

#include <QPen>
#include <QBrush>

QString ColorToLetter(LightColor color) {
	switch(color) {
	case WHITE :
		return QString("W");
		break;
	case RED :
		return QString("R");
		break;
	case GREEN :
		return QString("G");
		break;
	case ORANGE :
		return QString("O");
		break;
	case BLUE :
		return QString("B");
		break;
	case YELLOW :
		return QString("Y");
		break;
		// TODO: Fix unknown & nonilluminating, they most likely aren't black for real
	case UNKOWN :
		return QString("Unknown");
		break;
	case NONILLUMINATING :
		return QString("NonIlluminating");
		break;
	default:
		qWarning("Unknown light color given");
		return QString("Unknown color");
	}
}

void ColorToBrushAndPen(LightColor color, QPen &pen, QBrush &brush) {
	// default style for the pen
	pen.setWidthF(0);
	pen.setCosmetic(true);
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(Qt::black);
	pen.setStyle(Qt::NoPen);

	// default style for the brushs
	brush.setStyle(Qt::SolidPattern);

	switch(color) {
	case WHITE :
		pen.setStyle(Qt::SolidLine);

		brush.setColor(Qt::yellow);
		break;
	case RED :
		brush.setColor(Qt::red);
		break;
	case GREEN :
		brush.setColor(Qt::green);
		break;
	case ORANGE :
		brush.setColor(QColor(0xFF7900));
		break;
	case BLUE :
		brush.setColor(Qt::blue);
		break;
	case YELLOW :
		brush.setColor(Qt::yellow);
		break;
		// TODO: Fix unknown & nonilluminating, they most likely aren't black for real
	case UNKOWN :
		brush.setColor(Qt::black);
		break;
	case NONILLUMINATING :
		brush.setColor(Qt::black);
		break;
	default:
		qWarning("Unknown light color given");
	}
}


