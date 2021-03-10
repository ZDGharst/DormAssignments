#ifndef ANNEALER_H
#define ANNEALER_H

#include <array>
#include <iostream>
#include <fstream>

#include "globals.h"
#include "room.h"

class Annealer {
    public:
    std::array<int, SIZE_OF_COMPATIBILITIES> compatibilities;
    std::array<Room, NUM_ROOMS> rooms;
    int temperature, acceptedChanges, attemptedChanges;

    Annealer();

    bool LoadCompatibilities(std::string filename);
    void PreloadRooms();
    void RandomSwap();
    void SmallSwap();
    void LargeSwap();
    bool SaveResultsToFile(std::string filename);
};

#endif /* ANNEALER_H */