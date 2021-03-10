#include "room.h"

/* TODO: Change this to work with constants instead of magic numbers. */
void Room::CalculateFitness(const std::array<int, 40000>& compatibilities) {
    fitnessScore = 0;
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[1]];
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[2]];
    fitnessScore += compatibilities[roommate[0] * 200 + roommate[3]];
    fitnessScore += compatibilities[roommate[1] * 200 + roommate[2]];
    fitnessScore += compatibilities[roommate[1] * 200 + roommate[3]];
    fitnessScore += compatibilities[roommate[2] * 200 + roommate[3]];
}

void Room::SortRoommates() {
    int temp;
    for(int roommate1 = 0; roommate1 < NUM_STUDENTS_PER_ROOM - 1; roommate1++) {
        for(int roommate2 = roommate1 + 1; roommate2 < NUM_STUDENTS_PER_ROOM; roommate2++) {
            if(roommate[roommate1] > roommate[roommate2]) {
                temp = roommate[roommate1];
                roommate[roommate1] = roommate[roommate2];
                roommate[roommate2] = temp;
            }
        }
    }
}

std::string Room::PrintRoom() {
    std::string output = "\tFitness Score [";
    output += std::to_string(fitnessScore);
    output += "]\tStudents: [";
    for(int student = 0; student < NUM_STUDENTS_PER_ROOM - 1; student++) {
        output += std::to_string(roommate[student]);
        output += ", ";
    }
    output += std::to_string(roommate[NUM_STUDENTS_PER_ROOM - 1]);
    output += "]";

    return output;
}