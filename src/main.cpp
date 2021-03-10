#include <iostream>
#include <fstream>

#include "room.h"
#include "annealer.h"

int main(int argc, char* argv[]) {
    Annealer annealer;

    /* Handle input from file. */
    std::ifstream compatibilityFile;
    compatibilityFile.open("data/roommates.txt");

    if(!compatibilityFile) {
        std::cout << "Couldn't find input file! Exiting...\n";
        return -1;
    }

    for(int i = 0; i < 40000; i++) {
        compatibilityFile >> annealer.compatibilities[i];
    }

    compatibilityFile.close();

    /* Pre-load rooms. */
    for(int i = 0, j = 0; i < 50; i++) {
        annealer.rooms[i].roommate[0] = j++;
        annealer.rooms[i].roommate[1] = j++;
        annealer.rooms[i].roommate[2] = j++;
        annealer.rooms[i].roommate[3] = j++;
        annealer.rooms[i].CalculateFitness(annealer.compatibilities);

        std::cout << "Room #" << i + 1 << ": " << annealer.rooms[i].fitnessScore << "\n";
    }

    return 0;
}