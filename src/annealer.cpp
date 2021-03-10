#include "annealer.h"

Annealer::Annealer() {
    temperature = INITIAL_TEMPERATURE;
    acceptedChanges = 0;
    attemptedChanges = 0;

    /* Initialize random number generator. */
    rng = std::mt19937(rd());
    whichSwap = std::uniform_int_distribution<int>(0,1);
    whichRoom = std::uniform_int_distribution<int>(0, NUM_ROOMS - 1);
    whichRoommate = std::uniform_int_distribution<int>(0, NUM_STUDENTS_PER_ROOM - 1);
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

void Annealer::ReduceTemperature() {
    if(acceptedChanges >= CHANGES_BEFORE_REDUCTION || attemptedChanges >= ATTEMPTS_BEFORE_REDUCTION) {
        temperature *= GEOMETRIC_TEMP_REDUCTION;
        acceptedChanges = 0;
        attemptedChanges = 0;
    }
}

void Annealer::Solver() {
    ReduceTemperature();

    int i = 0;
    while(i++ < 100) {
        RandomSwap();
    }
}

void Annealer::RandomSwap() {
    int room1 = whichRoom(rng);
    int room2 = whichRoom(rng);
    while(room1 == room2) {
        room2 = whichRoom(rng);
    }

    SmallSwap(room1, room2);
}

void Annealer::SmallSwap(int room1, int room2) {
    Room changedRoom1 = Room(rooms[room1]);
    Room changedRoom2 = Room(rooms[room2]);
    int roommate1 = whichRoommate(rng);
    int roommate2 = whichRoommate(rng);

    int temp = changedRoom1.roommate[roommate1];
    changedRoom1.roommate[roommate1] = changedRoom2.roommate[roommate2];
    changedRoom2.roommate[roommate2] = temp;
    changedRoom1.CalculateFitness(compatibilities);
    changedRoom2.CalculateFitness(compatibilities);

    int deltaDifference = changedRoom1.fitnessScore + changedRoom2.fitnessScore - rooms[room1].fitnessScore - rooms[room2].fitnessScore;
    if(deltaDifference < 0) {
        std::cout << "BETTER! ";
        rooms[room1] = changedRoom1;
        rooms[room2] = changedRoom2;
    }
    else {
        std::cout << "WORSE! DON'T ";
    }

    if(DEBUG_ACTIVE) {
        std::cout << "Swapping roommate " << roommate1 + 1 << " of room " << room1 + 1
    << " with roommate " << roommate2 + 1 << " of room " << room2 + 1 << 
    " for a score of " << deltaDifference << "\n";
    }
}

void Annealer::LargeSwap() {
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

    for(int room = 0; room < NUM_ROOMS; room++) {
        saveFile << "\nRoom #" << room + 1 << ": " << rooms[room].fitnessScore;
    }

    saveFile.close();
    return true;
}