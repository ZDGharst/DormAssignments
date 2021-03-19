#ifndef ROOM_H
#define ROOM_H

#include <array>

#include "globals.h"

class Room {
    public:
    
    std::array<int, NUM_STUDENTS_PER_ROOM> roommate;
    int fitnessScore;

    void CalculateFitness(const std::array<int, SIZE_OF_COMPATIBILITIES>& compatibilities);
    void SortRoommates();
    const std::string PrintRoom();
};

#endif /* ROOM_H */