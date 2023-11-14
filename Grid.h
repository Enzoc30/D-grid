//
// Created by enzoc on 13/11/23.
//

#ifndef D_GRID_GRID_H
#define D_GRID_GRID_H


#include <iostream>
#include "Point.h"
#include <vector>
#include "Bucket.h"

template<typename Type=double>
class Grid {
private:
    // Is a Matrix
    std::vector<std::vector<Bucket*>> matrix;
    Type gridArea;
    Type gridSize;
    size_t bucketSize;

    void initializeGrid() {
        int n = static_cast<int>(gridArea / gridSize);
        matrix.resize(n, std::vector<Bucket*>(n, nullptr));
    }

public:


    Grid(Type gridArea, Type gridSize, size_t bucketSize)
            : gridArea(gridArea), gridSize(gridSize), bucketSize(bucketSize) {
        initializeGrid();
    }


    // TODO ADD
    // TODO Query - Search


    ~Grid() {
        for (auto& row : matrix) {
            for (Bucket* bucket : row) {
                while (bucket != nullptr) {
                    Bucket* nextBucket = bucket->nextBucket;
                    delete bucket;
                    bucket = nextBucket;
                }
            }
        }
    }
};


#endif //D_GRID_GRID_H
