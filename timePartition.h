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
        : dt(_dt), actualTime(0),
          u1(uGrid(cell, bucket, limiS, limitI)),
          u2(uGrid(cell, bucket, limiS, limitI)) {}

    void update(Entry e, double time) {
        if (static_cast<int>(time / dt) % 2) {
            u1.insert_update(e);
        } else {
            u2.insert_update(e);
        }

        if (static_cast<int>(time / dt) % 2 != static_cast<int>(actualTime / dt) % 2) {
            if (static_cast<int>(actualTime / dt) % 2)
                u1.clear();
            else
                u2.clear();
        }

        actualTime = time;
    }

    void print() {
        if (static_cast<int>(actualTime / dt) % 2) {
            u1.printGrid();
            // cout << "Max Velocity : " << u1.getMaxVel( ) << endl;
        } else {
            u2.printGrid();
            // cout << "Max Velocity : " << u2.getMaxVel( ) << endl;
        }
    }
};

#endif //D_GRID_TIMEPARTITION_H
