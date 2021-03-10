#ifndef ANNEALER_H
#define ANNEALER_H

#include <array>
#include <iostream>
#include <fstream>
#include <random>

#include "globals.h"
#include "room.h"

class Annealer {
    private:
    std::array<int, SIZE_OF_COMPATIBILITIES> compatibilities;
    std::array<Room, NUM_ROOMS> rooms;
    int temperature, acceptedChanges, attemptedChanges;

    std::random_device rd;
    std::mt19937 rng;
    std::uniform_int_distribution<int> whichSwap;
    std::uniform_int_distribution<int> whichRoom;
    std::uniform_int_distribution<int> whichRoommate;

    public:
    Annealer();

    bool LoadCompatibilities(std::string filename);
    void PreloadRooms();
    void ReduceTemperature();
    void Solver();
    void RandomSwap();
    void SmallSwap(int room1, int room2);
    void LargeSwap();
    bool SaveResultsToFile(std::string filename);
};

#endif /* ANNEALER_H */