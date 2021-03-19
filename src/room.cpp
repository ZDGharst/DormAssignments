#include "room.h"

using namespace DormAssignments::Globals;

void Room::CalculateFitness(const std::array<int, 40000>& compatibilities) {
    fitnessScore = 0;
    for(int student1 = 0; student1 < NUM_STUDENTS_PER_ROOM - 1; student1++) {
        for(int student2 = student1 + 1; student2 < NUM_STUDENTS_PER_ROOM; student2++) {
            fitnessScore += compatibilities[roommate[student1] * NUM_STUDENTS + roommate[student2]];
        }
    }
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

const std::string Room::PrintRoom() {
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