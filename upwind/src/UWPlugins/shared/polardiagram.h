/**
* \file polardiagram.h
* \author Carles Escrig i Royo
*
* Copyright (C) 2010 by University of Oulu (Finland)
*
* This file is part of UpWind Project.
*
* PolarDiagram class is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* PolarDiagram class is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with PolarDiagram class.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef POLARDIAGRAM_H
#define POLARDIAGRAM_H

#include <limits>
#include <QString>
#include <QMap>
#include <QDebug>

#include "uwmath.h"

/** Errors codes.
  * Error codes created within the UpWind::ShortTermPlanning project.
  */
enum UP_ERROR { ERROR_NONE, /**< no errors. */
                ERROR_PD_NAME, /**< Polar diagram: name is empty */
                ERROR_PD_SIZE, /**< Polar diagram: size is 0 */
                ERROR_PD_TA, /**< Polar diagram: time allowance out of range */
                ERROR_PD_ANG, /**< Polar diagram: gybe or beat angle missing */
                ERROR_BT_TWA, /**< Boat: TWA is not valid */
                ERROR_BT_TWD, /**< Boat: TWD is not valid */
                ERROR_BT_WSP, /**< Boat: wind velocity is not valid */
                ERROR_BT_HED, /**< Boat: boat angle is not valid */
                ERROR_BT_POS, /**< Boat: geographical position is not valid */
                ERROR_BT_QSP, /**< Boat: conformal inverted (QtGraphicsScene) position is not valid */
                ERROR_BT_LLA, /**< Boat: lay lines angle is not valid */
                ERROR_BT_LLP, /**< Boat: a point in the short path is not valid */
                ERROR_BT_ROT, /**< Boat: a point in the route is not valid */
                ERROR_BT_ROO, /**< Boat: a point in the route is inside an obstacle region */
                ERROR_BT_CKP /**< Boat: the destiny checkpoint is not valid */
              };

/// for debugging the class
#define DEBUG 1

/// minimum value allowed for a time allowance
#define MIN_TA_ALLOWED 1
/// maximum value allowed for a time allowance
#define MAX_TA_ALLOWED 10000

/// beat angle is from 0.0 to this value
#define MAX_ANGLE_CONSIDERED_BEAT   60.0
/// gybe angle is from this value to 180.0
#define MIN_ANGLE_CONSIDERED_GYBE   130.0

/// Angles is actually a QMap<True Wind Angle, Time Allowance>
typedef QMap<float, float> Angles;

/// Diagram is actually a QMap<wind velocity, QMap<True Wind Angle, Time Allowance>>
typedef QMap<float, Angles> Diagram;

class PolarDiagram
{
public:
    PolarDiagram();
    PolarDiagram( const PolarDiagram &);

    /** Fills a PolarDiagram with example valid data.
      */
    PolarDiagram& populate();

    /** Clears a a polar diagram, by clearing the name and the Diagram
      */
    void clear();

    /** Returns the name of the polar diagram
      */
    QString getName() const;

    /** Sets the name of the polar diagram
      */
    void setName(const QString &);

    /** Tells if the polar diagram is in TWA or AWA, same as isTWA()
      */
    bool getTWA() const;

    /** Tells if the polar diagram is in TWA or AWA
      */
    bool isTWA() const;

    /** Sets if the polar diagram is in TWA or AWA
      */
    void setTWA(bool);

    /** Returns the structure Diagram of a polar diagram
      */
    Diagram getDiagram();

    /** Sets the structure Diagram of a polar diagram by copying it
      */
    void setDiagram(const Diagram&);

    /** Returns the structure Angles-TA of the specified wind velocity
      */
    Angles getAngles(const float & speed);

    /** Sets the Angles-TA of the specified wind velocity
      * If the wind velocity is already present in the polar diagram
      * it merges the data.
      * \sa addLine()
      * @param speed The True Wind Velocity
      * @param angles An structure of Angles-TA
      */
    void setAngles(const float & speed, const Angles & angles);

    /** Adds data to the polar diagram.
      * \sa setAngles()
      * @param speed The True Wind Velocity
      * @param angle The True Wind Angle
      * @param ta The time allowance
      */
    void addLine(const float & speed, const float & angle, const float & ta);

    /** Returns a list of the wind velocitys present in the polar diagram
      */
    QList<float> getSpeeds();

    /** Checks a polar diagram
      * @return UP_ERROR error code
      */
    UP_ERROR check();

    /** Checks if the polar diagram is valid by using check()
      */
    bool isValid();

    /** Checks if the polar diagram contains the given wind velocity
      */
    bool contains(const float & speed);

    /** \brief Search the Gybe Angle for the given wind velocity.
      *
      * If the speed is not contained in the polar diagram, returns the nearest value.
      * If the input speed is smaller than any contained, returns the gybe angle
      * of the smallest wind velocity present in the polar diagram.
      * If the input speed is bigger than any contained, returns the gybe angle
      * of the biggest wind velocity present in the polar diagram.
      * Otherwise returns an average of the two closest gybe angles.
      */
    float getGybeAngle(const float &);

    /** \brief Search the Beat Angle for the given wind velocity.
      * If the speed is not contained in the polar diagram, returns the nearest value.
      * If the input speed is smaller than any contained, returns the beat angle
      * of the smallest wind velocity present in the polar diagram.
      * If the input speed is bigger than any contained, returns the beat angle
      * of the biggest wind velocity present in the polar diagram.
      * Otherwise returns an average of the two closest beat angles.
      */
    float getBeatAngle(const float &);

    /** \brief Search the time allowance for a given wind velocity and angle.
      * If the speed is not contained in the polar diagram, returns the closest value.
      * If the input speed is smaller than any contained, uses the first speed as closest.
      * If the input speed is bigger than any contained, uses the last speed as closest.
      * Otherwise uses an average of the two closer speeds.
      * It uses the same mechanism if the given angle is not present for the selected speed.
      * @param speed The wind velocity
      * @param angle The True Wind Angle
      * @return The time allowance
      */
    float getTA(const float & speed, const float & angle);

    /** \brief Search the optimum angle to use when sailing.
      * If the optimum angle is the angle of reaching returns 0.
      * \sa getGybeAngle()
      * \sa getBeatAngle()
      * @param speed The wind velocity
      * @param angle The True Wind Angle
      * @return The angle use with the wind direction.
      */
    float getAngle(const float & speed, const float & angle);

    int operator==(PolarDiagram &);
    int operator!=(PolarDiagram &);
    PolarDiagram & operator=(PolarDiagram);

private:
    // name: of this polar diagram
    // TWA: if true data is in TWA, else is AWA
    // diagram: QMap< float, QMap< float, float>>
    //! The name of the polar diagram
    QString name;
    //! Its data is TWA or AWA, by now all should be TWA
    bool TWA;
    //! The actual polar diagram
    Diagram diagram;
    //! Debug the polar diagram class
    bool debug;
};

#endif // POLARDIAGRAM_H
