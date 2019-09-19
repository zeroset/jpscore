/**
 * \file        Point.h
 * \date        Sep 30, 2010
 * \version     v0.8
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
 * # Description
 * Basic 2-d vectorial structure to deal with 2-D points. This class provides interaction for two points with each other
 * and fullfills [boost/geometry object model](http://www.boost.org/doc/libs/1_60_0/libs/geometry/doc/html/geometry/examples/example__adapting_a_legacy_geometry_object_model.html)
 * for a 2-d point.
 *
 *
 **/
#pragma once

#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry.hpp>

#include <string>
#include <cmath>

class Point {
public:
     double _x = 0.; //*<x-coordinate of a 2-d point*/
     double _y = 0.; //*<y-coordinate of a 2-d point*/

public:

     /**
      * **Ctor**
      * Constructs a new point with given x and y.
      * x and y are 0 if not given.
      * @param [in] x: x-coordinate as double
      * @param [in] y: y-coordinate as double
      */
     Point(double x = 0, double y = 0) : _x(x), _y(y) {};

     /// Norm
     double Norm() const;

     /// Norm square
     inline double NormSquare() const
     {
          return ScalarProduct(*this);
     }
     /// normalized vector
     Point Normalized() const;

    /// dot product
     inline double ScalarProduct(const Point &v) const
     {
          return _x * v._x + _y * v._y;
     }

     /**
      * since we have only 2D vectors (may be changed in the future), this function returns a scalar
      * basically the third component of the vector (0,0,z) )
      *
      */
     inline double CrossProduct(const Point &p) const
     {
          return Determinant(p);
     }



     /// determinant of the square matrix formed by the vectors [ this, v]
     inline double Determinant(const Point &v) const
     {
          return _x*v._y - _y*v._x;
     }


     Point TransformToEllipseCoordinates(const Point &center, double cphi, double sphi) const;
     /// translation and rotation in cartesian system
     Point TransformToCartesianCoordinates(const Point &center, double cphi, double sphi) const;
     /// rotate the vector by theta
     Point Rotate(double ctheta, double stheta) const;


     // operators
     /// addition
     Point operator+(const Point& p) const;
     /// substraction
     Point operator-(const Point& p) const;
     /// equal
     bool operator==(const Point& p) const;
     /// not equal
     bool operator!=(const Point& p) const;
     /// Assignement
     Point& operator+=(const Point& p);
     /// nice formating of the point
     std::string toString() const;
};

// Register Point for usage with boost::geometry
BOOST_GEOMETRY_REGISTER_POINT_2D(Point, double, cs::cartesian, _x, _y);


/// multiplication
Point operator*(const Point& p, const double f);
/// division
Point operator/(const Point& p, const double f);
