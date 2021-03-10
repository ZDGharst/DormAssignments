#ifndef ANNEALER_H
#define ANNEALER_H

#include <array>

class Annealer {
    public:
    std::array<int, 40000> compatibilities;
    std::array<Room, 50> rooms;
};

#endif