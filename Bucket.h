//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <vector>
#include "Point.h"
#include "params.h"

using namespace std;



//struct Meta {
//    Bucket* nextBucket;
//    Bucket* prevBucket;
//    int entries;
//    Meta():entries(0),nextBucket(nullptr), prevBucket(nullptr) {}
//};
//

struct Entry {
    int id;
    Point p;
    Point v;
    bool flag;
    Entry(int id) : id(id), p(0,0), v(0,0), flag(false) {};
};

struct Bucket {
    size_t size;
    size_t entries;
    vector<Entry> objectData;
    Bucket(size_t _size){
        size = _size;
        entries = 0;

    }
    bool insert(int data){
        if(objectData.size() != size) {
            objectData.emplace_back(data);
            entries++;
            return true;
        }
        else
            return false;
    }
};

#endif //D_GRID_BUCKET_H
