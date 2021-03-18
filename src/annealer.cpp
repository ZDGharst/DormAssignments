#include "annealer.h"

Annealer::Annealer(std::string filename) {
    temperature = INITIAL_TEMPERATURE;
    acceptedChanges = 0;
    totalChanges = 0;
    attemptedChanges = 0;
    totalAttempts = 0;
    solved = false;

    /* Initialize random number generator. */
    rng = std::mt19937(rd());
    whichSwap = std::uniform_int_distribution<int>(0,1);
    whichRoom = std::uniform_int_distribution<int>(0, NUM_ROOMS - 1);
    whichRoommate = std::uniform_int_distribution<int>(0, NUM_STUDENTS_PER_ROOM - 1);
    acceptanceProbability = std::uniform_real_distribution<double>(0.0, 1.0);

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

        totalChanges += acceptedChanges;
        acceptedChanges = 0;

        totalAttempts += attemptedChanges;
        attemptedChanges = 0;
    }
}

/* Swap randomly, adjust the temperature as needed, then save the results once the
 * assignments are considered to be solved. */
void Annealer::Solver() {
    start = std::chrono::steady_clock::now();

    while(!solved) {
        RandomSwap();
        ReduceTemperature();
    }

    end = std::chrono::steady_clock::now();
    SaveResultsToFile();
}

/* Choose two different rooms, then randomly pick between 1 of 2 swap methods. */
void Annealer::RandomSwap() {
    int room1 = whichRoom(rng);
    int room2 = whichRoom(rng);
    while(room1 == room2) {
        room2 = whichRoom(rng);
    }

    if(whichSwap(rng) == 0) {
        SmallSwap(room1, room2);
    }
    
    else {
        LargeSwap(room1, room2);
    }

    attemptedChanges++;
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

    if(AcceptChange(rooms[room1].fitnessScore + rooms[room2].fitnessScore,
                    changedRoom1.fitnessScore + changedRoom2.fitnessScore)) {
        rooms[room1] = changedRoom1;
        rooms[room2] = changedRoom2;
        acceptedChanges++;
    }
}

/* Switch the first two roommates in the first room with the second two roommates
 * in the second room. */
void Annealer::LargeSwap(int room1, int room2) {
    Room changedRoom1 = Room(rooms[room1]);
    Room changedRoom2 = Room(rooms[room2]);

    int temp;
    for(int studentPair = 0; studentPair < NUM_STUDENTS_PER_ROOM / 2; studentPair++) {
        temp = changedRoom1.roommate[studentPair];
        changedRoom1.roommate[studentPair] = changedRoom2.roommate[NUM_STUDENTS_PER_ROOM - studentPair - 1];
        changedRoom2.roommate[NUM_STUDENTS_PER_ROOM - studentPair - 1] = temp;
    }

    changedRoom1.CalculateFitness(compatibilities);
    changedRoom2.CalculateFitness(compatibilities);

    if(AcceptChange(rooms[room1].fitnessScore + rooms[room2].fitnessScore,
                    changedRoom1.fitnessScore + changedRoom2.fitnessScore)) {
        rooms[room1] = changedRoom1;
        rooms[room2] = changedRoom2;
        acceptedChanges++;
    }
}

/* Always accept a chance if it's better. Only accept a worse change if it falls within
 * the probability chance. */
bool Annealer::AcceptChange(int oldFitness, int newFitness) {
    if(newFitness <= oldFitness) {
        return true;
    }
    
    double acceptanceChance = std::exp((oldFitness - newFitness)/temperature);
    if(acceptanceChance >= acceptanceProbability(rng)) {
        return true;
    }
    
    return false;
}

bool Annealer::SaveResultsToFile() {
    auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&timeT), "%Y-%m-%d_%X");

    std::ofstream saveFile;
    saveFile.open("results/" + datetime.str() + ".txt");

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

        rooms[room].SortRoommates();
    }

    averageScore /= NUM_ROOMS;
    auto elapsedTime = end - start;

    saveFile <<   "Initial temperature:   " << std::setw(8) << INITIAL_TEMPERATURE
             << "\nGeometric reduction:   " << std::setw(8) << GEOMETRIC_TEMP_REDUCTION
             << "\nBest fitness score:    " << std::setw(8) << bestScore
             << "\nWorst fitness score:   " << std::setw(8) << worstScore
             << "\nAverage fitness score: " << std::setw(8) << averageScore
             << "\nTotal swaps:           " << std::setw(8) << totalChanges
             << "\nTotal attempts:        " << std::setw(8) << totalAttempts
             << "\nTime to solve (ms):    " << std::setw(8)
             << std::chrono::duration <double, std::milli> (elapsedTime).count() 
             << "\n";

    for(int room = 0; room < NUM_ROOMS; room++) {
        saveFile << "\nRoom #" << room + 1 << ": " << rooms[room].PrintRoom();
    }

    saveFile.close();
    return true;
}