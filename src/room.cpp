#include <array>

#include "room.h"

Room::Room() {
}

void Room::CalculateFitness(const std::array<int, 40000>& compatibilities) {
    fitnessScore = 0;
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[1]];
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[2]];
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[3]];
    fitnessScore += compatibilities[roommate[1] * 200 + roommate[2]];
    fitnessScore += compatibilities[roommate[1] * 200 + roommate[3]];
    fitnessScore += compatibilities[roommate[2] * 200 + roommate[3]];
}