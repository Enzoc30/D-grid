//
// Created by enzoc on 14/11/23.
//

#ifndef D_GRID_BUCKET_H
#define D_GRID_BUCKET_H

#include <iostream>
#include <list>
#include "Point.h"
#include "params.h"


class Bucket{
private:
    std::vector<Point> data;
public:
    Bucket* nextBucket;
    Bucket* prevBucket;

    Bucket() : nextBucket(nullptr), prevBucket(nullptr) { };

    void push(const Point& point){
        data.push_back(point);
    }

    void pop() {
        if(!data.empty()){
            data.pop_back();
        }
    }

    bool pop(const Point& point){
        for(auto it = data.begin(); it != data.end(); it ++ ){
            if(*it == point){
                data.erase(it);
                return true ;
            }
        }
        return false;
    }

    void printBucket() const {
        for(const Point&point : data){
            std::cout << point ;
        }std::cout << std::endl;
    }

    ~Bucket() = default;

};

#endif //D_GRID_BUCKET_H
