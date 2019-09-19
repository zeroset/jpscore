/*
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
 **/

#include "geometry/Point.h"

#include <catch2/catch.hpp>
#include <cmath>
#include <string>

const double PI = std::acos(-1);

TEST_CASE("geometry/Point", "[geometry][Point]")
{
    SECTION("ctor")
    {
        Point p;
        REQUIRE(p._x == 0);
        REQUIRE(p._y == 0);

        Point p2(1, 1);
        REQUIRE(p2._x == 1);
        REQUIRE(p2._y == 1);

        Point p3(p2);
        REQUIRE(p3._x == 1);
        REQUIRE(p3._y == 1);
    }

    SECTION("Norm")
    {
        REQUIRE(Point(0, 0).Norm() == Approx(0));
        REQUIRE(Point(1, 0).Norm() == Approx(1));
        REQUIRE(Point(-1, 0).Norm() == Approx(1));
        REQUIRE(Point(-3, 0).Norm() == Approx(3));
        REQUIRE(Point(0, 1).Norm() == Approx(1));
        REQUIRE(Point(0, -1).Norm() == Approx(1));
        REQUIRE(Point(0, 3).Norm() == Approx(3));
        REQUIRE(Point(3, 4).Norm() == Approx(5));
    }

    SECTION("toString")
    {
        REQUIRE(Point(0, 1.116).toString() == "( 0.00 : 1.12 )");
        REQUIRE(Point(0, 1.111).toString() == "( 0.00 : 1.11 )");
        REQUIRE(Point(10000, 1).toString() == "( 10000.00 : 1.00 )");
        REQUIRE(Point(-2, -0.5).toString() == "( -2.00 : -0.50 )");
    }

    SECTION("NormSquare")
    {
        for(double i = -5, j = 0.6; i < 5; ++i, ++j) {
            Point p{i, j};
            REQUIRE(p.NormSquare() == Approx(std::pow(i, 2) + std::pow(j, 2)));
        }
    }

    SECTION("Normalized")
    {
        Point p1(0.0001, 0.0001);
        Point p2 = p1.Normalized();
        REQUIRE(p2._x == Approx(0.0));
        REQUIRE(p2._y == Approx(0.0));

        for(double i = 0, j = -10; i < 5; ++i, ++j) {
            p1._x = (i);
            p1._y = (j);
            p2    = p1.Normalized();
            REQUIRE(p2._x == Approx(i / p1.Norm()));
            REQUIRE(p2._y == Approx(j / p1.Norm()));
        }
    }

    SECTION("Determinant")
    {
        Point p1(10, 5);
        Point p2;
        for(int i = 1; i < 5; ++i) {
            p2._x = (std::cos(PI / -i));
            p2._y = (std::sin(PI / i));
            REQUIRE(p1.Determinant(p2) == Approx(10 * std::sin(PI / i) - 5 * std::cos(PI / i)));
        }
    }

    SECTION("ScalarProduct")
    {
        Point p1(10, 5);
        Point p2;
        for(int i = 1; i < 5; ++i) {
            p2._x = (std::cos(PI / i));
            p2._y = (std::sin(PI / -i));
            REQUIRE(p1.ScalarProduct(p2) == Approx(10 * p2._x + 5 * p2._y));
        }
    }

    SECTION("Operators")
    {
        for(int i = 1; i < 10; ++i) {
            Point p1(i, -i * 10);
            Point p2(cos(PI / i), sin(PI / i));

            Point sum = p1 + p2;
            REQUIRE(sum._x == Approx(i + std::cos(PI / i)));
            REQUIRE(sum._y == Approx(-i * 10 + std::sin(PI / i)));
            Point sub = p1 - p2;
            REQUIRE(sub._x == Approx(i - std::cos(PI / i)));
            REQUIRE(sub._y == Approx(-i * 10 - std::sin(PI / i)));
            Point mul = p2 * i;
            REQUIRE(mul._x == Approx(i * std::cos(PI / i)));
            REQUIRE(mul._y == Approx(i * std::sin(PI / i)));
            Point pluseq(i, i);
            pluseq += p1;
            REQUIRE(pluseq._x == Approx(i + i));
            REQUIRE(pluseq._y == Approx(-i * 10 + i));
            Point div = p2 / 1E-7;
            REQUIRE(div._x == Approx(p2._x));
            REQUIRE(div._y == Approx(p2._y));
            div = p2 / 2;
            REQUIRE(div._x == Approx(p2._x / 2));
            REQUIRE(div._y == Approx(p2._y / 2));

            REQUIRE(p1 != p2);
            p1._x = (p2._x);
            p1._y = (p2._y);
            REQUIRE(p1 == p2);
        }
    }

    SECTION("TransformToEllipseCoordinates")
    {
        for(int i = 1; i < 5; ++i) {
            Point p1(i * 10, i / 10);
            Point center(i, -i);
            Point check = (p1 - center).Rotate(std::cos(PI / i), -std::sin(PI / i));
            Point transform =
                p1.TransformToEllipseCoordinates(center, std::cos(PI / i), std::sin(PI / i));
            REQUIRE(transform == check);
        }
    }

    SECTION("TransformToCartesianCoordinates")
    {
        for(int i = 1; i < 5; ++i) {
            Point p1(i * 10, i / 10);
            Point center(-i, i);
            Point transform =
                p1.TransformToCartesianCoordinates(center, std::cos(PI / i), std::sin(PI / i));
            Point check = p1.Rotate(std::cos(PI / i), std::sin(PI / i));
            REQUIRE(transform._x == Approx(check._x - i));
            REQUIRE(transform._y == Approx(check._y + i));
        }
    }
}
