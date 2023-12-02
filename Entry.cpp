#include <ostream>
#include "Entry.hpp"

std::ostream& operator<<(std::ostream& os, Entry const& e) {
    os << "<Entry: " << e.id << ", (" << e.p.getX() << ", " << e.p.getY() << "), " << "(" << e.v.getX() << ", " << e.v.getY() << ")" << ">";

    return os;
}
