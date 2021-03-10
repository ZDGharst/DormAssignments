#include <iostream>
#include <fstream>

#include "globals.h"
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

    for(int i = 0; i < SIZE_OF_COMPATIBILITIES; i++) {
        compatibilityFile >> annealer.compatibilities[i];
    }

    compatibilityFile.close();

    /* Pre-load rooms. */
    for(int i = 0, j = 0; i < NUM_ROOMS; i++) {
        for(int r = 0; r < NUM_STUDENTS_PER_ROOM; r++) {
            annealer.rooms[i].roommate[r] = j++;
        }
        annealer.rooms[i].CalculateFitness(annealer.compatibilities);

        std::cout << "Room #" << i + 1 << ": " << annealer.rooms[i].fitnessScore << "\n";
    }

    return 0;
}