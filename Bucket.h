//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <list>
#include "Point.h"
#include "params.h"

// TODO Double linked-list
// ****************************************
// But for the moment is a **linked-list**
// ****************************************
class Bucket{
private:
    // TODO Double Linked-list
    std::list<Point> data;
public:
    Bucket* nextBucket;
    // Bucket* prevBucket;
    Bucket() : nextBucket(nullptr) { };
};

#endif //D_GRID_BUCKET_H
