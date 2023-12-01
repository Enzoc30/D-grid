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
    double maxVeloX;
    double maxVeloY;
    double minVeloX;
    double minVeloY;
public:
    timePartition(double _dt, Point limiS, Point limitI ,int cell, int bucket)
            : dt(_dt), actualTime(0), u1(uGrid(limiS, limitI , cell, bucket)), u2(uGrid(limiS, limitI, cell, bucket)) {}


    void update(Entry e) {
        if (static_cast<int>(e.time / dt) % 2) {
            u1.insertOrUpdate(e);
        } else {
            u2.insertOrUpdate(e);
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
            cout << "Max Velocity : " << u1.getMaxVel( ) << endl;
        } else {
            u2.printGrid();
            cout << "Max Velocity : " << u2.getMaxVel( ) << endl;
        }
    }
};

#endif //D_GRID_TIMEPARTITION_H
