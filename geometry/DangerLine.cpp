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
#include "../IO/OutputHandler.cpp"
#include <chrono>

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
    _lastTimeExposure = 0;
    _expositionReady = true;
    _fdangerlines = new FileHandler("dangerlines.txt");

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

double DangerLine::GetRandomNumber()
{
     std::mt19937_64 rng;
     // initialize the random number generator with time-dependent seed
     uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
     rng.seed(timeSeed);
     // initialize a uniform distribution between 0 and 1
     std::uniform_real_distribution<double> unif(0, 1);
     return unif(rng);
}

void DangerLine::expose(Pedestrian* ped, double seed)
{
    //We don't want to expose every step, but every second

    if(_expositionReady) {
        // std::cout<<"OK++ EXPOSE: "<<_lastTimeUpdate<<" "<<_lastTimeExposure<<std::endl;
        _lastTimeExposure = _lastTimeUpdate;
        Point p = ped->GetPos();
        double fatality = getFatalProbability(&p);
        double fate = GetRandomNumber();
        // std::cerr << "  " << fate << "   " << fatality << "\n";
        if (fate < fatality) {

             //we "kill" the pedestrian by making it not moving
             //TODO improve this part by making the pedestrian able to be walked over
             ped->SetV0Norm(0, 0, 0, 0, 0, 0, 0);
             ped->Kill();
        }
    }else{
        //std::cout<<"KO---- EXPOSE: "<<_lastTimeUpdate<<" "<<_lastTimeExposure<<std::endl;
    }
}

double DangerLine::getFatalProbability(const Point *p) const
{
    double distance = this->DistTo(*p);
    if(distance <= _fatalDistance) //certain death
    {
        return 1;
    }else if (distance < _startDistance){
        // mortality chance is (d/dmax)^alpha where d is how close the ped is
        // and dmax the minimum distance allowed before certain death

         double p = pow((1-(distance - _fatalDistance)/(_startDistance - _fatalDistance)),_alpha);
         // std::cout << "----\n distance " << distance << " start " << " "  << _startDistance << " fataldistance "  <<  _fatalDistance << "\n";
         // std::cout << ">> p= " << p << " alpha " << _alpha << "\n";
                                                              return p;

    }else{
        return 0;
    }
}

void DangerLine::update(double time)
{
    if(_lastTimeUpdate == 0)    //first time calling
        _lastTimeUpdate = time;

    if(_lastTimeExposure == 0)
        _lastTimeExposure = time;

    if(_expositionReady){
        _expositionReady = false;
    }else if(time - _lastTimeExposure > 1){
        _expositionReady = true;
    }


    double new_x1 = this->GetPoint1()._x + (time-_lastTimeUpdate)*_x_speed;
    double new_y1 = this->GetPoint1()._y + (time-_lastTimeUpdate)*_y_speed;
    double new_x2 = this->GetPoint2()._x + (time-_lastTimeUpdate)*_x_speed;
    double new_y2 = this->GetPoint2()._y + (time-_lastTimeUpdate)*_y_speed;

    this->SetPoint1(Point(new_x1,new_y1));
    this->SetPoint2(Point(new_x2,new_y2));
    this->_lastTimeUpdate = time;
    //For the moment, danger line trajectories are written in the console
    this->outputDangerLine();

}

void DangerLine::outputDangerLine()
{
     _fdangerlines->Write("%.2f,%.2f,%.2f,%.2f,%.2f",
                          _lastTimeUpdate,
                          this->GetPoint1()._x,
                          this->GetPoint1()._y,
                          this->GetPoint2()._x,
                          this->GetPoint2()._y);
}
