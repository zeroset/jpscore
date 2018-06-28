/**
 * \file        DangerLine.cpp
 * \date        Jun 11, 2018
 * \version     v0.0
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 *
 *
 **/


#ifndef _DANGERLINE_H
#define _DANGERLINE_H

#include "Line.h"
#include "../pedestrian/Pedestrian.h"
#include "../IO/OutputHandler.h"

#include <string>

//forward declarations
class OutputHandler;

//class Pedestrian;

class Wall;

// external variables
extern OutputHandler* Log;

class DangerLine : public Line {
private:
    double _fatalDistance;
    double _startDistance;
    double _alpha;
    double _x_speed;
    double _y_speed;
    double _lastTimeUpdate;
    double _lastTimeExposure;
    bool _expositionReady;

    /**
     * @param p a point close to the line
     * @return double between 0 and 100, 100 = certain death
     */
    double getFatalProbability(const Point *p) const;

public:

    /**
     * The default constructor
     * DONT FORGET TO CALL DangerLine::setParameters(...)
     */
    DangerLine();

    virtual ~DangerLine();

    /**
     * Set V0 norm (speed) of the pedestrian at 0
     * with a random chance bigger the closer to the line
     * @param ped the pedestrian to expose
     */
    void expose(Pedestrian *ped);

    /**
     * set the characteristics of the line
     * @param fatalDistance the distance from the line where there is a 100% chance of dying
     * @param startDistance the distance from the line after which there is no danger
     * @param alpha must be > 0, default 1, the exponent of the exposition formula
     * formula = 100*pow((1-(distance - _fatalDistance)/(_startDistance - _fatalDistance)),_alpha)
     * @param x_speed the horizontal speed of the line, in meter/sec
     * @param y_speed the vertical speed of the line, in meter/sec
     */
    void setParameters(double fatalDistance, double startDistance, double alpha, double x_speed, double y_speed);

    /**
     * move the line according to its own speed
     * @param time simulation time
     */
    void update(double time);

    void print();

};

#endif  /* _DANGERLINE_H */











