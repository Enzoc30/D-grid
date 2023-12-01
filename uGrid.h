#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <list>

#include "Entry.hpp"

using namespace std;

class uGrid {
private:
    using entry_id = uint64_t;
    using Bucket = std::vector<Entry>;
    using grid_element = std::list<Bucket>;

    struct SecondaryEntry {
      grid_element* p_grid_cell;
      Bucket* p_bucket;
      uint32_t index_in_bucket;
    };

private:
    Point const m_lower_limit;
    Point const m_upper_limit;

    double const m_cell_size;
    uint32_t const m_n_cells;
    uint32_t const m_bucket_size;

    double max_pos_x;
    double max_neg_x;
    double max_pos_y;
    double max_neg_y;

    vector<vector<grid_element>> m_grid;
    unordered_map<entry_id, SecondaryEntry> m_secondary_index;

public:
    auto getGrid(){return m_grid;}

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
    uGrid(double cell_size, uint32_t bucket_size, Point lower_limit, Point upper_limit)
        : m_lower_limit(lower_limit),
          m_upper_limit(upper_limit),
          m_cell_size(cell_size),
          m_n_cells(calculate_size_number()),
          m_bucket_size(bucket_size),
          max_pos_x(0.0),
          max_neg_x(0.0),
          max_pos_y(0.0),
          max_neg_y(0.0)
    {
        m_grid.resize(m_n_cells, vector<grid_element>(m_n_cells));
    }

    void insert_update(Entry const& entry) {
        auto it = m_secondary_index.find(entry.id);

        auto [x_offset, y_offset] = get_offset(entry.p);

        if (x_offset < 0 || y_offset < 0) {
            return;
        }

        auto [cell_x, cell_y] = cell_coords_for_offset(x_offset, y_offset);

        if (cell_x >= m_n_cells || cell_y >= m_n_cells) {
            return;
        }

        grid_element& new_cell = m_grid.at(cell_x).at(cell_y);

        if (it == m_secondary_index.end()) {
            auto [p_bucket, index_in_bucket] = insert_no_sindex(entry, new_cell);
            m_secondary_index[entry.id] = {&new_cell, p_bucket, index_in_bucket};
            return;
        }

        SecondaryEntry& se = it->second;

        if (se.p_grid_cell == std::addressof(new_cell)) {
            localUpdate(se.p_bucket->at(se.index_in_bucket), entry);
        }
        else {
            // Remove the element from its current cell
            grid_element& old_cell = *se.p_grid_cell;

            assert(!old_cell.empty());

            Bucket& first_bucket = old_cell.front();

            assert(!first_bucket.empty());

            Entry& last_in_first_bucket = first_bucket.back();
            Entry& removing_entry = se.p_bucket->at(se.index_in_bucket);

            if (&last_in_first_bucket != &removing_entry) {
                removing_entry.id = last_in_first_bucket.id;
                removing_entry.p = last_in_first_bucket.p;
                removing_entry.v = last_in_first_bucket.p;

                SecondaryEntry& se_swapping = m_secondary_index.at(last_in_first_bucket.id);
                assert(se_swapping.p_grid_cell == se.p_grid_cell);
                se_swapping.p_bucket = se.p_bucket;
                se_swapping.index_in_bucket = se.index_in_bucket;
            }

            first_bucket.pop_back();

            // Insert it in new location
            auto [p_bucket, index_in_bucket] = insert_no_sindex(entry, new_cell);

            se.p_grid_cell = &new_cell;
            se.index_in_bucket = index_in_bucket;
            se.p_bucket = p_bucket;
        }
    }

    void localUpdate(Entry& updating_entry, Entry const& new_entry) {
        assert(updating_entry.deleted_p == false);

        updating_entry.p = new_entry.p;
        updating_entry.v = new_entry.v;
    }

    double maxVelocity(){
        return max({max_neg_x, max_pos_x, max_neg_y, max_neg_x});
    }

    auto get_offset(Point const& p) -> std::pair<double, double> {
        double x_offset = (p.getX() - m_lower_limit.getX());
        double y_offset = (p.getY() - m_lower_limit.getY());

        return {x_offset, y_offset};
    }

    auto cell_coords_for_offset(double dx, double dy) -> std::pair<uint32_t, uint32_t> {
        uint32_t cell_x = dx / m_cell_size;
        uint32_t cell_y = dy / m_cell_size;

        return {cell_x, cell_y};
    }

    auto insert_no_sindex(Entry const& e, grid_element& cell)
        -> std::pair<Bucket*, uint32_t> {
        Bucket& first_bucket = cell.front();
        if (first_bucket.size() == m_bucket_size) {
            cell.push_front({});
            Bucket& new_first_bucket = cell.front();
            new_first_bucket.push_back(e);
            return {&new_first_bucket, new_first_bucket.size() - 1};
        }
        else {
            first_bucket.push_back(e);
            return {&first_bucket, first_bucket.size() - 1};
        }
    }

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
        double minX = q1.getX();
        double minY = q1.getY();
        double maxX = q2.getX();
        double maxY = q2.getY();

        vector<Entry> result;

        //Definimos ST -> minimum rectangle with grid cell boundaries containing S
        // for (auto x = minX; x <= maxX; x += cellSize) { //iterando en ST
        //     for (auto y = minY; y<= maxY; y += cellSize) {
        //         auto cell = m_grid.at(x).at(y);  // están en S' ahora
        //         for (auto bucket : cell) {
        //             for (auto entry : bucket) {
        //                 result.push_back(entry);
        //             }
        //         }
        //     }
        // }

        return result;
    }

    vector<Entry> predictiveRangeQuery(Point q1, Point q2, double tq){
        pair<Point,Point> Sp = enlargeS(q1,q2,tq);
        vector<Entry> candidates = rangeQuery(Sp.first, Sp.second);
        //to-do -> evaluar candidate set

        return {};
    }

    void printIndexSecondary(){
        for(auto &i : m_secondary_index){
            cout << "*******************" << endl;
            cout << " Index : " << i.second.index_in_bucket << endl;
            cout << " size :  "<< i.second.p_bucket->size() << endl;
            for(auto &iem : *i.second.p_bucket ){
                cout << "ID " << iem.id << endl;
            }
            cout << "*******************" << endl;
        }
    }

    void printGrid() const {
        for (int i = 0; i < m_grid.size(); ++i) {
            for (int j = 0; j < m_grid[i].size(); ++j) {
                std::cout << "Cell (" << i * m_cell_size << ", " << j * m_cell_size << "): ";
                grid_element const& grid_e = m_grid[i][j];
                cout << endl <<"---------------------------- " << endl;

                for(Bucket const& b : grid_e) {
                    for (Entry const& entry: b) {
                        std::cout << "ID : " << entry.id << " - ";
                    }
                    std::cout << std::endl;
                }

                cout << "---------------------------- " << endl;
                std::cout << std::endl;
            }
        }
    }

    void clear() {
        for (auto& row : m_grid) {
            for (grid_element ge : row) {
                ge.clear();
            }
        }

        m_secondary_index.clear();
    }
};
