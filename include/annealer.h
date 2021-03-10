#ifndef ANNEALER_H
#define ANNEALER_H

#include <array>

#include "globals.h"
#include "room.h"

class Annealer {
    public:
    std::array<int, SIZE_OF_COMPATIBILITIES> compatibilities;
    std::array<Room, NUM_ROOMS> rooms;
    int temperature, acceptedChanges, attemptedChanges;

    Annealer();
};

#endif /* ANNEALER_H */