#include <iostream>
#include <fstream>
#include <array>

#include "room.h"

int main(int argc, char* argv[]) {
    std::array<int, 40000> compatibilities;
    std::array<Room, 50> rooms;

    std::ifstream compatibilityFile;
    compatibilityFile.open("data/roommates.txt");

    for(int i = 0; i < 40000; i++) {
        compatibilityFile >> compatibilities[i];
    }

    compatibilityFile.close();

    int j = 0;
    for(int i = 0; i < 50; i++) {
        rooms[i].roommate[0] = j++;
        rooms[i].roommate[1] = j++;
        rooms[i].roommate[2] = j++;
        rooms[i].roommate[3] = j++;
        rooms[i].CalculateFitness(compatibilities);

        std::cout << "Room #" << i + 1 << ": " << rooms[i].fitnessScore << "\n";
    }

    return 0;
}