#include "annealer.h"

Annealer::Annealer(std::string filename) {
    temperature = INITIAL_TEMPERATURE;
    acceptedChanges = 0;
    attemptedChanges = 0;
    solved = false;

    /* Initialize random number generator. */
    rng = std::mt19937(rd());
    whichSwap = std::uniform_int_distribution<int>(0,1);
    whichRoom = std::uniform_int_distribution<int>(0, NUM_ROOMS - 1);
    whichRoommate = std::uniform_int_distribution<int>(0, NUM_STUDENTS_PER_ROOM - 1);

    if(!LoadCompatibilities(filename)) {
        exit(-1);
    };

    PreloadRooms();
}

/* Load roommate compatibility scores from file. Return boolean indicates success. */
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

/* Generate rooms at the start by arbitrary assignment. */
void Annealer::PreloadRooms() {
    for(int room = 0, student = 0; room < NUM_ROOMS; room++) {
        for(int roommate = 0; roommate < NUM_STUDENTS_PER_ROOM; roommate++) {
            rooms[room].roommate[roommate] = student++;
        }

        rooms[room].CalculateFitness(compatibilities);
    }
}

/* Check if there has been enough attempts/changes for a reduction, and if there have been
 * no changes after enough attempts, consider the assignments solved. */
void Annealer::ReduceTemperature() {
    if(attemptedChanges >= ATTEMPTS_BEFORE_REDUCTION
    || acceptedChanges >= CHANGES_BEFORE_REDUCTION) {
        if(acceptedChanges == 0) {
            solved = true;
            return;
        }

        temperature *= GEOMETRIC_TEMP_REDUCTION;
        acceptedChanges = 0;
        attemptedChanges = 0;
    }
}

/* Swap randomly, adjust the temperature as needed, then save the results once the
 * assignments are considered to be solved. */
void Annealer::Solver() {
    while(!solved) {
        RandomSwap();
        ReduceTemperature();
    }

    SaveResultsToFile("output.txt");
}

/* Choose two different rooms, then randomly pick between 1 of 2 swap methods. */
void Annealer::RandomSwap() {
    int room1 = whichRoom(rng);
    int room2 = whichRoom(rng);
    while(room1 == room2) {
        room2 = whichRoom(rng);
    }

    SmallSwap(room1, room2);

    attemptedChanges++;
    totalAttempts++;
}

/* Switch a roommate at random from the first room with another roommate at random
 * from the second room. */
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

    int deltaDifference = changedRoom1.fitnessScore + changedRoom2.fitnessScore
    - rooms[room1].fitnessScore - rooms[room2].fitnessScore;
    if(deltaDifference < 0) {
        std::cout << "BETTER! ";
        rooms[room1] = changedRoom1;
        rooms[room2] = changedRoom2;
        acceptedChanges++;
        totalChanges++;
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

/* Switch the first two roommates in the first room with the second two roommates
 * in the second room. */
void Annealer::LargeSwap() {
}

bool Annealer::SaveResultsToFile(std::string filename) {
    std::ofstream saveFile;
    saveFile.open(filename);

    if(!saveFile) {
        std::cout << "Couldn't create output file! Exiting...\n";
        return false;
    }

    int averageScore = 0, bestScore = rooms[0].fitnessScore, worstScore = rooms[0].fitnessScore;
    
    for(int room = 0; room < NUM_ROOMS; room++) {
        averageScore += rooms[room].fitnessScore;
        if(bestScore > rooms[room].fitnessScore) {
            bestScore = rooms[room].fitnessScore;
        }
        else if(worstScore < rooms[room].fitnessScore) {
            worstScore = rooms[room].fitnessScore;
        }
    }

    averageScore /= NUM_ROOMS;

    saveFile << "Initial temperature:   " << INITIAL_TEMPERATURE
             << "\nGeometric reduction:   " << GEOMETRIC_TEMP_REDUCTION
             << "\nBest fitness score:    " << bestScore
             << "\nWorst fitness score:   " << worstScore
             << "\nAverage fitness score: " << averageScore
             << "\nTotal swaps:           " << totalChanges
             << "\nTotal attempts:        " << totalAttempts
             << "\n";

    for(int room = 0; room < NUM_ROOMS; room++) {
        saveFile << "\nRoom #" << room + 1 << ": " << rooms[room].fitnessScore;
    }

    saveFile.close();
    return true;
}