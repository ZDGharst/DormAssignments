#ifndef ROOM_H
#define ROOM_H

#include <array>

#include "globals.h"

class Room {
    public:

    int roommate[NUM_STUDENTS_PER_ROOM];
    int fitnessScore;

    void CalculateFitness(const std::array<int, SIZE_OF_COMPATIBILITIES>& compatibilities);
    void SortRoommates();
    std::string PrintRoom();
};

#endif /* ROOM_H */