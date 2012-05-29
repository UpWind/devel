/**
 * Utilies.h
 *
 *  Created on: Oct 24, 2008
 *      Author: markus
 */

#ifndef UTILIES_H_
#define UTILIES_H_

#include <QColor>

//static const float PI = 3.14159f;
#define PI 3.1415926535

inline double degreesToRadians(double degrees) {
	return degrees * (PI / 180);
}

inline double radiansToDegrees(double radians) {
	return radians * (180 / PI);
}

/*
0= tuntematon
1= merimajakka
2= sektoriloisto
3= linjamerkki
4= suuntaloisto
5= apuloisto
6= muu merkki
7= reunamerkki
8= tutkamerkki
9= poiju
10= viitta
11= tunnusmajakka
13= kummeli
 */
enum LightType {
	UNKNOWN = 0,
	LIGHTHOUSE = 1,
	SECTORBEACON = 2,
	FAYWAREMARK = 3,
	NAVLIGHT = 4,
	HELPBEACON = 5, // apuloisto
	OTHERMARK = 6,
        EDGEMARK = 7,
        RADARTARGET = 8, // RACON?
	BUOY = 9,
        SPAR = 10,
	SYMBOLIGHTHOUSE = 11,
        CAIRN = 13
};


/*0= tuntematon
   1= valkoinen
   2= punainen
   3= vihre�
   4= oranssi
   5= sininen
   6= keltainen
   7= valaisematon
 */
enum LightColor
{
	UNKOWN = 0,
	WHITE,
	RED,
	GREEN,
	ORANGE,
	BLUE,
	YELLOW,
	NONILLUMINATING // TODO check the correctness of the word
};

enum Type
{
	DAY = 1,
	NIGHT,
	NOT_KNOWN
};

enum Quality
{
	MAIN = 1,
	EXTRA
};

/*
1= kalastusloisto
2= lentomajakka
3= varoitusvalo
4= lentoestevalo
5= tehovalo
 */
enum SpecialFunction
{
	FISHING = 1,
	FLIGHT,
	WARNING,
	FLIGHTBLOCK,
	POWERLIGHT
};
/*
1= peittynyt sektori
2= vahvistettu valosektor
 */
enum Visibility
{
	DEFERRED = 1,
	ENHANCED
};


class QPen;
class QBrush;
class QPixmap;

QString ColorToLetter(LightColor color);
void ColorToBrushAndPen(LightColor color, QPen &pen, QBrush &brush);

#endif
