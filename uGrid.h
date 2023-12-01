//
// Created by enzoc on 30/11/23.
//

#ifndef D_GRID_UGRID_H
#define D_GRID_UGRID_H

#include <iostream>
#include "Bucket.h"
#include <vector>
#include <algorithm>

using namespace std;

struct SecondaryEntry{
    Bucket* ptr1;
    vector<Bucket*>* ptr2;
    int index;
    int oid;
    SecondaryEntry(){
        ptr1 = nullptr;
        ptr2 = nullptr;
        index = 0;
        oid = 0;
    }
    SecondaryEntry(vector<Bucket*>* gridcell, Bucket* bucket, int id){
        ptr1 = bucket;
        ptr2 = gridcell;
        for (auto i = 0; i < bucket->objectData.size(); i++){
            if(bucket->objectData[i].id == id){
                index = i;
                break;
            }
        }
        oid = id;
    }
};

class uGrid {
private:

    int gridSize;
    int cellSize;
    int bucketSize;

    vector<vector<vector<Bucket*>>> grid;
    vector<SecondaryEntry> secondaryIndex;

public:
    int getgridSize() const{return gridSize;}
    int getcellSize() const {return cellSize;}

    auto getGrid(){return grid;}

    uGrid(int area, int cell, int bucket)
            : gridSize(area), cellSize(cell), bucketSize(bucket) {
        int numCells = gridSize / cellSize;
        grid.resize(numCells, vector<vector<Bucket*>>(numCells));

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
        if(grid[cellX][cellY].empty()){
            Bucket* b = new Bucket(bucketSize);
            grid[cellX][cellY].push_back(b);
        }

        auto ins = grid[cellX][cellY].back()->insert(data);
        if(!ins){
            Bucket* b = new Bucket(bucketSize);
            b->insert(data);
            grid[cellX][cellY].push_back(b);
        }
        secondaryIndex.emplace_back(&grid[cellX][cellY], grid[cellX][cellY].back(), data);
    }

    void printGrid() const {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                std::cout << "Cell (" << i * cellSize << ", " << j * cellSize << "): ";
                vector<Bucket*> currentBucket = grid[i][j];
                cout << endl <<"---------------------------- " << endl;
                for(auto &oo : currentBucket) {
                    if (oo != nullptr) {
                        for (auto &entry: oo->objectData) {
                            std::cout << "ID : " << entry.id << " - ";
                        }
                    }
                    std::cout << std::endl;
                }
                cout << "---------------------------- " << endl;
                std::cout << std::endl;
            }
        }
    }
};



#endif //D_GRID_UGRID_H