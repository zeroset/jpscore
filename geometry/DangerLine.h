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

    double getFatalProbability(const Point *p);

public:

    DangerLine();

    virtual ~DangerLine();

    void expose(Pedestrian *ped);

    void setParameters(double fatalDistance, double startDistance, double alpha, double x_speed, double y_speed);

    void update();
};

#endif  /* _LINE_H */











