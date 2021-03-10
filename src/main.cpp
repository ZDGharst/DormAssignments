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
    for(int room = 0, student = 0; room < NUM_ROOMS; room++) {
        for(int roommate = 0; roommate < NUM_STUDENTS_PER_ROOM; roommate++) {
            annealer.rooms[room].roommate[roommate] = student++;
        }
        annealer.rooms[room].CalculateFitness(annealer.compatibilities);

        std::cout << "Room #" << room + 1 << ": " << annealer.rooms[room].fitnessScore << "\n";
    }

    return 0;
}