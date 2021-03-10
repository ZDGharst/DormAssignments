#ifndef ROOM_H
#define ROOM_H

#include <array>

class Room {
    public:

    int roommate[4];
    int fitnessScore;

    void CalculateFitness(const std::array<int, 40000>& compatibilities);
};

#endif /* ROOM_H */