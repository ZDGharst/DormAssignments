#include "annealer.h"

Annealer::Annealer() {
    temperature = INITIAL_TEMPERATURE;
    acceptedChanges = 0;
    attemptedChanges = 0;
}

bool Annealer::LoadCompatibilities(std::string filename) {
    std::ifstream compatibilityFile;
    compatibilityFile.open(filename);

    if(!compatibilityFile) {
        std::cout << "Couldn't find input file! Exiting...\n";
        return false;
    }

    for(int i = 0; i < SIZE_OF_COMPATIBILITIES; i++) {
        compatibilityFile >> compatibilities[i];
    }

    compatibilityFile.close();
    return true;
}

void Annealer::PreloadRooms() {
    for(int room = 0, student = 0; room < NUM_ROOMS; room++) {
        for(int roommate = 0; roommate < NUM_STUDENTS_PER_ROOM; roommate++) {
            rooms[room].roommate[roommate] = student++;
        }

        rooms[room].CalculateFitness(compatibilities);
    }
}

void Annealer::RandomSwap() {

}

bool Annealer::SaveResultsToFile(std::string filename) {
    std::ofstream saveFile;
    saveFile.open(filename);

    if(!saveFile) {
        std::cout << "Couldn't create output file! Exiting...\n";
        return false;
    }

    saveFile << "Initial temperature:  " << INITIAL_TEMPERATURE
             << "\nCooling schedule: " << GEOMETRIC_TEMP_REDUCTION
             << "\nBest room score:    "
             << "\nWorst room score:   "
             << "\nAverage room score: ";

    saveFile.close();
    return true;
}