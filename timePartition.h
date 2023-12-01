//
// Created by valen on 1/12/2023.
//

#ifndef D_GRID_TIMEPARTITION_H
#define D_GRID_TIMEPARTITION_H

#include "uGrid.h"

class timePartition{
private:
    uGrid u1;
    uGrid u2;
    double dt;
    double actualtime;
public:
    timePartition(double _dt, int area, int cell, int bucket)
        : dt(_dt), actualtime(0), u1(uGrid(area,cell,bucket)), u2(uGrid(area,cell,bucket)) {};

    void update(Entry e){
        if(int(e.time/dt) % 2){
            if(u1.SearchbyIS(e.id) == nullptr) u1.insert(e);
            else u1.localUpdate(e);
        } else {
            if(u2.SearchbyIS(e.id) == nullptr) u2.insert(e);
            else u2.localUpdate(e);
        }
        if(int(e.time/dt) % 2 != int(actualtime/dt) % 2){
            if(int(actualtime/dt) % 2) u1.cleanup();
            else u2.cleanup();
        }
        actualtime = e.time;
    }

    void print(){
        if(int(actualtime/dt) % 2){
            u1.printGrid();
        } else {
            u2.printGrid();
        }
    }

};

#endif //D_GRID_TIMEPARTITION_H
