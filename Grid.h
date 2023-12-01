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

    void localUpdate(Entry oldEntry, Entry newEntry) {
        if (oldEntry.p.getX() == newEntry.p.getX() && oldEntry.p.getY() == newEntry.p.getY()) {
            vector<Bucket*> currentBucket = getCell(oldEntry.p.getX(),oldEntry.p.getY());
            for(auto &o : currentBucket) {
                for (auto &entry: o->objectData) {
                    if (entry.id == newEntry.id) {
                        Point newPoint(newEntry.p.getX(), newEntry.p.getY());
                        entry.p = newPoint;
                        break;
                    }
                }
            }
        } else {
            deleteFromCell(oldEntry);
            insertIntoCell(newEntry);
        }
    }

    void deleteFromCell(Entry &data) {
        int cellX = data.p.getX() / cellSize;
        int cellY = data.p.getY() / cellSize;

        vector<Bucket*> entries = grid[cellX][cellY];
        for(auto &oo : entries) {
            oo->objectData.erase(
                    remove_if(oo->objectData.begin(), oo->objectData.end(),
                              [data](const Entry &entry) { return entry.id == data.id; }),
                    oo->objectData.end()
            );
        }
    }

    void insertIntoCell(Entry &data) {
        int cellX = data.p.getX() / cellSize;
        int cellY = data.p.getY() / cellSize;
        auto ins = grid[cellX][cellY].back()->insert(data.id);
        if(!ins){
            Bucket* b = new Bucket(bucketSize);
            b->insert(data.id);
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
