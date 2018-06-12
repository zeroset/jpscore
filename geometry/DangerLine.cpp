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

#include "../math/Mathematics.h"
#include "Point.h"
#include "../general/Macros.h"
#include "Line.h"
#include "Wall.h"
#include <time.h>

int Line::_static_UID = 0;

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
    std::srand(time(NULL));
}

DangerLine::DangerLine(const Point& p1, const Point& p2, int count, float fatalDistance, float startDistance, float lambda):
      Line(p1,p2,count), _fatalDistance(fatalDistance), _startDistance(startDistance), _lamda(lambda)
{
}

DangerLine::DangerLine(const Point& p1, const Point& p2):
            Line(p1,p2), _fatalDistance(fatalDistance), _startDistance(startDistance), _lamda(lambda)
{
}

DangerLine::DangerLine(const Line& orig):
      _point1(orig.GetPoint1()), _point2(orig.GetPoint2()), _centre(orig.GetCentre()), _length(orig.GetLength()), _uid(orig.GetUniqueID())
{
}

DangerLine::~DangerLine()
{
}






void DangerLine::expose(Pedestrian* ped)
{
    Point p = ped->GetPos();
    float fatality = abs(getFatalProba(p));
    float fate = rand() % 100;
    if( fate < fatality ){
        ped->SetV0Norm(0,0,0,0,0,0,0);
    }
}

float DangerLine::getFatalProbability(Point *p)
{
    float distance = this->DistTo(p);
    if(distance < _fatalDistance) //certain death
    {
        return 100;
    }else if (distance < _startDistance){
        return pow((1-(distance - _fatalDistance)/(_startDistance - _fatalDistance)),_alpha);
    }else{
        return 0;
    }
}
