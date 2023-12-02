#pragma once

#include <ostream>

#include "Point.h"

struct Entry {
    int id;
    Point p;
    Point v;
    bool deleted_p = false;

};

std::ostream& operator<<(std::ostream& os, Entry const& e);
