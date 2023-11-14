//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <list>
#include "Point.h"
#include "params.h"

using namespace std;

struct Meta {
    Bucket* nextBucket;
    Bucket* prevBucket;
    int entries;
    Meta():entries(0),nextBucket(nullptr), prevBucket(nullptr) {}
};

struct Entry {
    int id;
    Point* p; //position (x,y)
    Point* v; //velocity (vx,vy)
    bool flag;
    Entry(int id) : id(id), p(nullptr), v(nullptr), flag(false) {};
};

class Bucket{
private:
    Meta* meta; //meta
    list<Entry*> entries; //será así????

public:
    Bucket(): meta(nullptr) { };
    
    //insert

    ~Bucket() = default;

};

#endif //D_GRID_BUCKET_H
