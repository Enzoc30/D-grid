//
// Created by enzoc on 13/11/23.
//

#ifndef D_GRID_POINT_H
#define D_GRID_POINT_H

#include <iostream>
#include "params.h"
#include <utility>


// Point is a pair
// There are only two coordinates

class Point {
private:
    int objectID;
    std::pair<double, double> coordinates;

public:
    Point(double x, double y) : coordinates(x, y), objectID(-1) {};
    Point() = default;

    void setx(double x) { coordinates.first = x; }
    void sety(double y) { coordinates.second = y; }
    void setPoint(const std::pair<double, double> &other) { coordinates = other; }

    double getX() const { return coordinates.first; }
    double getY() const { return coordinates.second; }
    std::pair<double, double> getPoint() { return coordinates ; }


    Point& operator=(const Point& other) {
        coordinates.first = other.getX();
        coordinates.second = other.getY();
        return *this;
    }


    bool operator==(const Point& other) const {
        return this->coordinates.first == other.getX() and this->coordinates.second == other.getY();
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point operator+(const Point& other) const {
        Point result( other.getX()+this->coordinates.first , other.getY()+this->coordinates.second);
        return result;
    }

    Point operator-(const Point& other) const {
        Point result(coordinates.first - other.getX(), coordinates.second - other.getY());
        return result;
    }

    Point operator*(double scalar) const {
        Point result(coordinates.first * scalar, coordinates.second * scalar);
        return result;
    }

<<<<<<< HEAD
=======

>>>>>>> 68a66b0 (init)
    Point operator/(double scalar) const {
        Point result(coordinates.first / scalar, coordinates.second / scalar);
        return result;
    }

    Point& operator+=(const Point& other) {
        coordinates.first += other.getX();
        coordinates.second += other.getY();
        return *this;
    }

    Point& operator-=(const Point& other) {
        coordinates.first -= other.getX();
        coordinates.second -= other.getY();
        return *this;
    }

    Point& operator*=(double scalar) {
        coordinates.first *= scalar;
        coordinates.second *= scalar;
        return *this;
    }

    Point& operator/=(double scalar) {
        coordinates.first /= scalar;
        coordinates.second /= scalar;
        return *this;
    }


    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "("; os << p.coordinates.first ; os << ", ";  os << p.coordinates.second; os << ")";
        return os;
    }

};


#endif //D_GRID_POINT_H