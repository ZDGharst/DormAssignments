#ifndef ANNEALER_H
#define ANNEALER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <chrono>
#include <random>

#include "globals.h"
#include "room.h"

class Annealer {
    private:
    std::array<int, SIZE_OF_COMPATIBILITIES> compatibilities;
    std::array<Room, NUM_ROOMS> rooms;
    int acceptedChanges, attemptedChanges, totalChanges, totalAttempts;
    double temperature;
    bool solved;
    
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;

    std::random_device rd;
    std::mt19937 rng;
    std::uniform_int_distribution<int> whichSwap;
    std::uniform_int_distribution<int> whichRoom;
    std::uniform_int_distribution<int> whichRoommate;
    std::uniform_real_distribution<double> acceptanceProbability;

    public:
    Annealer(std::string filename);

    bool LoadCompatibilities(std::string filename);
    void PreloadRooms();
    void ReduceTemperature();
    void Solver();
    void RandomSwap();
    void SmallSwap(int room1, int room2);
    void LargeSwap(int room1, int room2);
    bool AcceptChange(int oldFitness, int newFitness);
    bool SaveResultsToFile(std::string filename);
};

#endif /* ANNEALER_H */