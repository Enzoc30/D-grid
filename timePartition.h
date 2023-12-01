//
// Created by valen on 1/12/2023.
//

#ifndef D_GRID_TIMEPARTITION_H
#define D_GRID_TIMEPARTITION_H

#include "uGrid.h"

class timePartition {
private:
    uGrid u1;
    uGrid u2;
    double dt;
    double actualTime;

public:
    timePartition(double _dt, int area, int cell, int bucket)
            : dt(_dt), actualTime(0), u1(uGrid(area, cell, bucket)), u2(uGrid(area, cell, bucket)) {}

    void update(Entry e) {
        if (static_cast<int>(e.time / dt) % 2) {
            if (u1.SearchbyIS(e.id) == nullptr)
                u1.insertIntoCell(e);
            else
                u1.localUpdate(e);
        } else {
            if (u2.SearchbyIS(e.id) == nullptr)
                u2.insertIntoCell(e);
            else
                u2.localUpdate(e);
        }

        if (static_cast<int>(e.time / dt) % 2 != static_cast<int>(actualTime / dt) % 2) {
            if (static_cast<int>(actualTime / dt) % 2)
                u1.cleanup();
            else
                u2.cleanup();
        }
        actualTime = e.time;
    }

    void print() {
        if (static_cast<int>(actualTime / dt) % 2) {
            u1.printGrid();
        } else {
            u2.printGrid();
        }
    }
};

#endif //D_GRID_TIMEPARTITION_H
