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
#include <list>
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
    double time;
    Entry(int data) : id(data), p(0,0), v(0,0), flag(false) {};
    Entry(int x, int y, int data) : id(data), p(x,y), v(0,0), flag(false) {};

    Entry(int id_, double t, double lat, double lon, int sx, int sy)
            : id(id_), time(t), p(lat, lon), v(sx,sy) {};


};

struct Bucket {
    size_t size;
    size_t entries;
    vector<Entry> objectData;
//    Point minPoint;
//    Point maxPoint;

    Bucket(size_t _size) : size(_size), entries(0) {}

    Point seachminvelo(){
        double vv = 1e9;
        double uu = 1e9;
        for(auto& i : objectData){
//            if(vv > i.p.getY()){
//                vv = i.v.getY(minPoint);
//            }
//            if(vv > i.p.getY()){
//                vv = i.v.getY();
//            }
        }
        return {uu,vv};
    }

    Point searchmaxvelo(){
        double uu = -1e9;
        double vv = -1e9;
        for(auto &i : objectData){
//            if(uu < i.p.getX()){
//                uu = i.v.getX();
//            }
//            if(vv < i.p.getY()){
//                vv = i.v.getY();
//            }
        }
        return {uu, vv };
    }

    bool insert(Entry e){
        if(objectData.size() != size) {
            objectData.push_back(e);
            entries++;
//            minPoint.setx(std::min(minPoint.getX(), e.p.getX()));
//            minPoint.sety(std::min(minPoint.getY(), e.p.getY()));
//            maxPoint.setx( std::max(maxPoint.getX(), e.p.getX()));
//            maxPoint.sety(std::max(maxPoint.getY(), e.p.getY()));
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
