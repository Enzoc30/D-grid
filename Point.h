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
    std::pair<int, int> coordinates;

public:
    Point(int x, int y) : coordinates(x, y), objectID(-1) {};
    Point() = default;

    void setx(int x) { coordinates.first = x; }
    void sety(int y) { coordinates.second = y; }
    void setPoint(const std::pair<int, int> &other) { coordinates = other; }

    int getX() const { return coordinates.first; }
    int getY() const { return coordinates.second; }
    std::pair<int, int> getPoint() { return coordinates ; }


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

    Point operator*(int scalar) const {
        Point result(coordinates.first * scalar, coordinates.second * scalar);
        return result;
    }

    Point operator/(int scalar) const {
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

    Point& operator*=(int scalar) {
        coordinates.first *= scalar;
        coordinates.second *= scalar;
        return *this;
    }

    Point& operator/=(int scalar) {
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
