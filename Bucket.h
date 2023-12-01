//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
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
    double velocity;
    bool flag;
    double time;
    Entry(int data) : id(data), p(0,0), v(0,0), flag(false) {};
    Entry(int x, int y, int data) : id(data), p(x,y), v(0,0), flag(false) {};

    Entry(int id_, double t, int lat, int lon, int sx, int sy)
            : id(id_), time(t), p(lat, lon), v(sx,sy) {};

    Entry(int id_, double t, int lat, int lon, int sx, int sy, double velo)
            : id(id_), time(t), p(lat, lon), v(sx, sy), velocity(velo), flag(false) {};

};

struct Bucket {
    size_t size;
    size_t entries;
    vector<Entry> objectData;
    Bucket(size_t _size){
        size = _size;
        entries = 0;

    }

    double searchmaxvelo(){
        double u = -1e9;
        for(auto &i : objectData){
            if(u < i.velocity){
                u = i.velocity;
            }
        }
        return u;
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
    bool deleteEntry(int id){
        auto it = find_if(objectData.begin(), objectData.end(),
                          [id](const Entry& entry) { return entry.id == id; });
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
