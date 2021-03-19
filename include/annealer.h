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
    std::array<int, DormAssignments::Globals::SIZE_OF_COMPATIBILITIES> m_compatibilities;
    std::array<Room, DormAssignments::Globals::NUM_ROOMS> m_rooms;
    int m_acceptedChanges = 0, m_attemptedChanges = 0,
        m_totalChanges = 0, m_totalAttempts = 0,
        m_startingTemperature;
    double m_temperature, m_reduction;
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
    Annealer(const std::string filename, const int temperature, const double reduction);

    bool LoadCompatibilities(std::string filename);
    void PreloadRooms();
    void ReduceTemperature();
    void Solver();
    void RandomSwap();
    void SmallSwap(const int room1, const int room2);
    void LargeSwap(const int room1, const int room2);
    bool AcceptChange(const int oldFitness, const int newFitness);
    bool SaveResultsToFile();
};

#endif /* ANNEALER_H */