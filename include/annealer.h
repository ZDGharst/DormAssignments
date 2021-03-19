#ifndef ANNEALER_H
#define ANNEALER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <chrono>
#include <random>
#include <sstream>

#include "globals.h"
#include "room.h"

class Annealer {
    private:
    std::array<int, SIZE_OF_COMPATIBILITIES> m_compatibilities;
    std::array<Room, NUM_ROOMS> m_rooms;
    int m_acceptedChanges = 0, m_attemptedChanges = 0,
        m_totalChanges = 0, m_totalAttempts = 0,
        m_startingTemperature;
    double m_temperature;
    bool m_solved = false;
    
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::time_point<std::chrono::steady_clock> m_end;

    std::random_device m_rd;
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_whichSwap;
    std::uniform_int_distribution<int> m_whichRoom;
    std::uniform_int_distribution<int> m_whichRoommate;
    std::uniform_real_distribution<double> m_acceptanceProbability;

    public:
    Annealer(std::string filename, int temperature);

    bool LoadCompatibilities(std::string filename);
    void PreloadRooms();
    void ReduceTemperature();
    void Solver();
    void RandomSwap();
    void SmallSwap(int room1, int room2);
    void LargeSwap(int room1, int room2);
    bool AcceptChange(int oldFitness, int newFitness);
    bool SaveResultsToFile();
};

#endif /* ANNEALER_H */