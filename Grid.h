//
// Created by enzoc on 13/11/23.
//

#ifndef D_GRID_GRID_H
#define D_GRID_GRID_H


#include <iostream>
#include "Point.h"
#include <vector>
#include <algorithm>
#include "params.h"
#include "Bucket.h"



class Grid {
private:

    int gridSize;
    int cellSize;
    int bucketSize;

    vector<vector<vector<Bucket*>>> grid;

public:
    int getgridSize() const{return gridSize;}
    int getcellSize() const {return cellSize;}

    auto getGrid(){return grid;}

    Grid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        int numCells = gridSize / cellSize;
        grid.resize(numCells, vector<vector<Bucket*>>(numCells, vector<Bucket*>(bucketSize, nullptr)));
    }

    vector<Bucket*> getCell(int x, int y) const {
        int cellX = x / cellSize;
        int cellY = y / cellSize;
        return grid[cellX][cellY];
    }

    void localUpdate(int oldX, int oldY, int newX, int newY, int data) {
        if (oldX == newX && oldY == newY) {
            vector<Bucket*> currentBucket = getCell(oldX,oldY);
            for(auto &o : currentBucket) {
                for (auto &entry: o->objectData) {
                    if (entry.id == data) {
                        Point newPoint(newX, newY);
                        entry.p = newPoint;
                        break;
                    }
                }
            }
        } else {
            deleteFromCell(oldX, oldY, data);
            insertIntoCell(newX, newY, data);
        }
    }

    void deleteFromCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;

        vector<Bucket*> entries = grid[cellX][cellY];
        for(auto &oo : entries) {
            oo->objectData.erase(
                    remove_if(oo->objectData.begin(), oo->objectData.end(),
                              [data](const Entry &entry) { return entry.id == data; }),
                    oo->objectData.end()
            );
        }
    }

    void insertIntoCell(int x, int y, int data) {
        int cellX = x / cellSize;
        int cellY = y / cellSize;
        auto ins = grid[cellX][cellY].back()->insert(data);
        if(!ins){
            Bucket* b = new Bucket(bucketSize);
            b->insert(data);
            grid[cellX][cellY].push_back(b);
        }
    }

    void printGrid() const {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                std::cout << "Cell (" << i * cellSize << ", " << j * cellSize << "): ";
                vector<Bucket*> currentBucket = grid[i][j];
                for(auto &oo : currentBucket) {
                    if (oo != nullptr) {
                        for (auto &entry: oo->objectData) {
                            std::cout << "ID : " << entry.id << " - ";
                        }
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
};


#endif //D_GRID_GRID_H
