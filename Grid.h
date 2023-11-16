//
// Created by enzoc on 13/11/23.
//

#ifndef D_GRID_GRID_H
#define D_GRID_GRID_H


#include <iostream>
#include "Point.h"
#include <vector>
#include "params.h"
#include "Bucket.h"

class Grid {
private:
    // Is a Matrix
    std::vector<std::vector<Bucket*>> matrix;
    int gridArea;
    int gridSize;
    size_t bucketSize;

    void initializeGrid() {
        auto n = (gridArea / gridSize);
        matrix.resize(n, std::vector<Bucket*>(n, nullptr));
    }

public:


    Grid(int gridArea, int gridSize, size_t bucketSize)
            : gridArea(gridArea), gridSize(gridSize), bucketSize(bucketSize) {
        initializeGrid();
    }


    // TODO ADD
    // TODO Query - Search


    ~Grid() {
        for(int i = 0 ; i < gridSize; i++){
            for(int j = 0 ; j < gridSize ; j ++ ){
                delete matrix[i][j];
            }
        }
    }
};


#endif //D_GRID_GRID_H
