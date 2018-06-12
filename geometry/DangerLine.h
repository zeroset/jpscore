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
#include "../IO/OutputHandler.h"

#include <string>

//forward declarations
class OutputHandler;

class Wall;

// external variables
extern OutputHandler* Log;

class DangerLine : public Line {
private:
    float _fatalDistance;
    float _startDistance;
    float _alpha;

    float getFatalProbability(Point *p);

public:

    DangerLine();

    DangerLine(const Point &p1, const Point &p2,
               int count); // count = 0 --> don't count the line. Useful for temporary lines.

    DangerLine(const Point &p1, const Point &p2);

    DangerLine(const Line &orig);

    virtual ~DangerLine();

    void expose(Pedestrian *ped);

    void setParameters(float fatalDistance, float startDistance, float lambda);

}

#endif  /* _LINE_H */











