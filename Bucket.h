//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <utility>
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
    string time;
    Entry(int data) : id(data), p(0,0), v(0,0), flag(false) {};
    Entry(int id_, std::string& t, int lat, int lon, int sx, int sy)
            : id(id_), time(t), p(lat, lon), v(sx,sy) {}

};

struct Bucket {
    size_t size;
    size_t entries;
    vector<Entry> objectData;
    Bucket(size_t _size){
        size = _size;
        entries = 0;

    }
    bool insert(Entry e){
        if(objectData.size() != size) {
            objectData.push_back(e);
            entries++;
            return true;
        }
        else
            return false;
    }
    bool deleteEntry(Entry e){
        auto it = find_if(objectData.begin(), objectData.end(),
                          [e](Entry& entry) { return entry.id == e.id; });
        if (it != objectData.end()) {
            objectData.erase(it);
            entries--;
            return true;
        } else {
            return false;
        }
    }
};

#endif //D_GRID_BUCKET_H
