//
// Created by enzoc on 30/11/23.
//

#ifndef D_GRID_UGRID_H
#define D_GRID_UGRID_H
#include <cassert>
#include <cstdint>
#include <iostream>
#include "Bucket.h"
#include <vector>
#include "unordered_map"
#include <algorithm>

using namespace std;

class uGrid {
private:
    using entry_id = uint64_t;
    using grid_element = std::vector<Bucket>;

    struct SecondaryEntry {
      Bucket* p_bucket;
      grid_element* p_grid_cell;
      int index;
      int oid;
    };

private:
    Point const m_lower_limit;
    Point const m_upper_limit;

    uint32_t const m_cell_size;
    uint32_t const m_n_cells;
    uint32_t const m_bucket_size;

    double max_pos_x;
    double max_neg_x;
    double max_pos_y;
    double max_neg_y;

    vector<vector<grid_element>> grid;
    unordered_map<entry_id, SecondaryEntry> secondaryIndex;

public:
    auto getGrid(){return grid;}

    uint32_t calculate_size_number() {
        double x_amplitude = m_upper_limit.getX() - m_lower_limit.getX();
        double y_amplitude = m_upper_limit.getY() - m_lower_limit.getY();

        assert(x_amplitude > 0);
        assert(y_amplitude > 0);

        double max_amplitude = max(x_amplitude, y_amplitude);
        return ceil(max_amplitude / m_cell_size);
    }

    /**
     * lower_limit: The leftmost|lowermost point in the area
     * upper_limit: The rightmost|uppermost point in the area
     * */
    uGrid(uint32_t cell_size, uint32_t bucket_size, Point lower_limit, Point upper_limit)
        : m_lower_limit(lower_limit),
          m_upper_limit(upper_limit),
          m_cell_size(cell_size),
          m_n_cells(calculate_size_number()),
          m_bucket_size(bucket_size),
          max_pos_x(0),
          max_neg_x(0),
          max_pos_y(0),
          max_neg_y(0)
    {
        grid.resize(m_n_cells, vector<grid_element>(m_n_cells));
    }

    void localUpdate(Entry newEntry) {
        int cellX = newEntry.p.getX() / cellSize;
        int cellY = newEntry.p.getY() / cellSize;
        auto newgridcell = grid[cellX][cellY];
        SecondaryEntry oldEntry = *SearchbyIS(newEntry.id);
        auto oldgridcell = *oldEntry.ptr2;
        if (oldgridcell == newgridcell) {
            Bucket* currentBucket = oldEntry.ptr1;
            Point newPoint(newEntry.p.getX(), newEntry.p.getY());
            currentBucket->objectData[oldEntry.index].p = newPoint;
        } else {
            deleteFromCell(oldEntry);
            insertIntoCell(newEntry);
        }
    }

    double maxVelocity(){
        double maxi = -1e9;
        for(auto &i : grid ){
            for(auto &j : i){
                for(auto &k : j){
                    double z = k->getMaxVelocity();
                    if(maxi < z){
                        maxi = z;
                    }
                }
            }
        }
        return maxi;
    }


    void deleteFromCell(SecondaryEntry &e) {
        e.ptr1->deleteEntry(e.oid);
        secondaryIndex.erase(e.oid);
//        maxVelocity();
    }

    Point getMaxVel() const{
        return maxiVel;
    }
    Point getMinVel() const{
        return minVel;
    }

    void insertOrUpdate(Entry & e){
        if (SearchbyIS(e.id) == nullptr)
            insertIntoCell(e);
        else
            localUpdate(e);
    }

    void insertIntoCell(Entry &data) {
        int cellX = data.p.getX() / cellSize;
        int cellY = data.p.getY() / cellSize;

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
        SecondaryEntry minieentrie(&grid[cellX][cellY], grid[cellX][cellY].back(), data.id);
        secondaryIndex[data.id] = minieentrie;
    }

    SecondaryEntry* SearchbyIS(int idx){
        auto it = secondaryIndex.find(idx);
        return (it != secondaryIndex.end()) ? &it->second : nullptr;
    }

    //return elements of grids

    //

    pair<Point,Point> enlargeS(Point q1, Point q2, double tq){
        //Definimos los bordes de S
        double minX = min(q1.getX(),q2.getX());
        double minY = min(q1.getY(),q2.getY());
        double maxX = max(q1.getX(),q2.getX());
        double maxY = max(q1.getY(),q2.getY());

        //Definimos las bordes de S'
        //to-do -> ARREGLAR POR VELOCIDADES INDIVIDUALES POR LADO (NSEW)
        minX += tq * maxVelocity();
        minY += tq * maxVelocity();
        maxX += tq * maxVelocity();
        maxY += tq * maxVelocity();
        return {Point(minX,minY),Point(maxX,maxY)};
    }

    vector<Entry> rangeQuery(Point q1, Point q2){
        vector<Entry> result;
        double minX = q1.getX();
        double minY = q1.getY();
        double maxX = q2.getX();
        double maxY = q2.getY();

        //Definimos ST -> minimum rectangle with grid cell boundaries containing S
        for (auto x=minX; x<=maxX; x+=cellSize){ //iterando en ST
            for (auto y=minY; y<=maxY; y+=cellSize){
                auto cell = getCell(x,y); //están en S' ahora
                for (auto bucket : cell){
                    for (auto entry : bucket->objectData) {
                        result.push_back(entry); 
                    }
                }
            }
        }
        return result;
    }


    vector<Entry> predictiveRangeQuery(Point q1, Point q2, double tq){
        pair<Point,Point> Sp = enlargeS(q1,q2,tq);
        vector<Entry> candidates = rangeQuery(Sp.first, Sp.second);
        //to-do -> evaluar candidate set
    }

    void printIndexSecondary(){
        for(auto &i : secondaryIndex){
            cout << "*******************" << endl;
            cout << " oid : " << i.second.oid << endl;
            cout << " Index : " << i.second.index << endl;
            cout << " size :  "<< i.second.ptr1->entries << endl;
            for(auto &iem : i.second.ptr1->objectData ){
                cout << "ID " << iem.id << endl;
            }
            cout << "*******************" << endl;
        }
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

    void clear() {
        for (auto& row : grid) {
            for (grid_element ge : row) {
                ge.clear();
            }
        }

        secondaryIndex.clear();
    }
};

#endif //D_GRID_UGRID_H
