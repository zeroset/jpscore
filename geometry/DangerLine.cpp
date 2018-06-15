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

#include <time.h>
#include "../math/Mathematics.h"
#include "Point.h"
#include "../general/Macros.h"
#include "DangerLine.h"
#include "../pedestrian/Pedestrian.h"
#include "Line.h"
#include "Wall.h"


using namespace std;

#define DEBUG 0

/************************************************************
  Konstruktoren
 ************************************************************/
DangerLine::DangerLine() : Line()
{
    _fatalDistance = 0;
    _startDistance = 0;
    _alpha = 1;
    _x_speed = 0;
    _y_speed = 0;
    _lastTimeUpdate = 0;

}

DangerLine::~DangerLine()
{
}


void DangerLine::setParameters(double fatalDistance, double startDistance, double alpha, double x_speed,
                               double y_speed) {
    _fatalDistance = fatalDistance;
    _startDistance = startDistance;
    _alpha = alpha;
    _x_speed = x_speed;
    _y_speed = y_speed;

}


void DangerLine::expose(Pedestrian* ped) const
{
    Point p = ped->GetPos();
    double fatality = abs(getFatalProbability(&p));
    double fate = rand() % 100;
    if( fate < fatality ){
        ped->SetV0Norm(0,0,0,0,0,0,0);
    }
}

double DangerLine::getFatalProbability(const Point *p) const
{
    double distance = this->DistTo(*p);
    if(distance < _fatalDistance) //certain death
    {
        return 100;
    }else if (distance < _startDistance){
        return pow((1-(distance - _fatalDistance)/(_startDistance - _fatalDistance)),_alpha);
    }else{
        return 0;
    }
}

void DangerLine::update(double time)
{
    if(_lastTimeUpdate == 0)
        _lastTimeUpdate = time;

    double new_x1 = this->GetPoint1()._x + (time-_lastTimeUpdate)*_x_speed;
    double new_y1 = this->GetPoint1()._y + (time-_lastTimeUpdate)*_y_speed;
    double new_x2 = this->GetPoint2()._x + (time-_lastTimeUpdate)*_x_speed;
    double new_y2 = this->GetPoint2()._y + (time-_lastTimeUpdate)*_y_speed;

    this->SetPoint1(Point(new_x1,new_y1));
    this->SetPoint2(Point(new_x2,new_y2));
    this->_lastTimeUpdate = time;

}
