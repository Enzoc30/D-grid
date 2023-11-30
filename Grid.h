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

    int gridSize;
    int cellSize;
    int bucketSize;

    std::vector<std::vector<Bucket*>> grid;

public:
    int getgridSize() const{return gridSize;}
    int getcellSize() const {return cellSize;}

    std::vector<std::vector<Bucket*>> getGrid(){return grid;}
    Grid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        int numCells = gridSize / cellSize;

        grid.resize(numCells, std::vector<Bucket*>(numCells, nullptr));
    }
    Bucket* getCell(int x, int y) const {
        int cellX = x / cellSize;
        int cellY = y / cellSize;
        return grid[cellX][cellY];
    }

    void localUpdate(int oldX, int oldY, int newX, int newY, int data) {
        if (oldX == newX && oldY == newY) {
            Bucket* currentBucket = grid[oldX / cellSize][oldY / cellSize];
            while (currentBucket != nullptr) {
                if (currentBucket->objectData == data) {
                    break;
                }
                currentBucket = currentBucket->nextBucket;
            }
        } else {
            deleteFromCell(oldX, oldY, data);
            insertIntoCell(newX, newY, data);
        }
    }

    void deleteFromCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        Bucket* currentBucket = grid[cellX][cellY];
        Bucket* prevBucket = nullptr;

        while (currentBucket != nullptr) {
            if (currentBucket->objectData == data) {
                if (prevBucket != nullptr) {
                    prevBucket->nextBucket = currentBucket->nextBucket;
                } else {
                    grid[cellX][cellY] = currentBucket->nextBucket;
                }

                delete currentBucket;
                break;
            }

            prevBucket = currentBucket;
            currentBucket = currentBucket->nextBucket;
        }
    }

    void insertIntoCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        Bucket* newBucket = new Bucket(data);

        if (grid[cellX][cellY] == nullptr) {
            grid[cellX][cellY] = newBucket;
        } else {
            Bucket* currentBucket = grid[cellX][cellY];
            while (currentBucket->nextBucket != nullptr) {
                currentBucket = currentBucket->nextBucket;
            }

            currentBucket->nextBucket = newBucket;
        }
    }

    void printGrid() const {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                std::cout << "Cell (" << i * cellSize << ", " << j * cellSize << "): ";

                Bucket* currentBucket = grid[i][j];
                while (currentBucket != nullptr) {
                    std::cout << currentBucket->objectData << " ";
                    currentBucket = currentBucket->nextBucket;
                }

                std::cout << std::endl;
            }
        }
    }

    void cleanup() {
        for (auto& row : grid) {
            for (auto& bucket : row) {
                while (bucket != nullptr) {
                    Bucket* temp = bucket;
                    bucket = bucket->nextBucket;
                    delete temp;
                }
            }
        }
    }
};


#endif //D_GRID_GRID_H
