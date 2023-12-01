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

    vector<Entry> obejectData ;

    Bucket(int data) {
        Entry entry(data);
        obejectData.emplace_back(entry);
    }

    Bucket(const Entry& entry){
        obejectData.emplace_back(entry);
    }

};

#endif //D_GRID_BUCKET_H
