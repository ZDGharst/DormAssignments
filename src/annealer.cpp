#include "annealer.h"

using namespace DormAssignments::Globals;

Annealer::Annealer(const std::string filename, const int temperature, const double reduction)
    : m_rng(m_rd())
    , m_whichSwap(0, 1)
    , m_whichRoom(0, NUM_ROOMS - 1)
    , m_whichRoommate(0, NUM_STUDENTS_PER_ROOM - 1)
    , m_acceptanceProbability(0.0, 1.0)
    , m_startingTemperature(temperature)
    , m_temperature(static_cast<double>(temperature))
    , m_reduction(reduction) {

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
        std::cout << "DormAssignments: cannot stat '" << filename << "': No such file or directory\n";
        return false;
    }

    for(int i = 0; i < SIZE_OF_COMPATIBILITIES; i++) {
        compatibilityFile >> m_compatibilities[i];
    }

    compatibilityFile.close();
    return true;
}

/* Generate rooms at the start by arbitrary assignment. */
void Annealer::PreloadRooms() {
    for(int room = 0, student = 0; room < NUM_ROOMS; room++) {
        for(int roommate = 0; roommate < NUM_STUDENTS_PER_ROOM; roommate++) {
            m_rooms[room].roommate[roommate] = student++;
        }

        m_rooms[room].CalculateFitness(m_compatibilities);
    }
}

/* Check if there has been enough attempts/changes for a reduction, and if there have been
 * no changes after enough attempts, consider the assignments solved. */
void Annealer::ReduceTemperature() {
    if(m_attemptedChanges >= ATTEMPTS_BEFORE_REDUCTION
    || m_acceptedChanges >= CHANGES_BEFORE_REDUCTION) {
        if(m_acceptedChanges == 0) {
            m_solved = true;
            return;
        }

        m_temperature *= m_reduction;

        m_totalChanges += m_acceptedChanges;
        m_acceptedChanges = 0;

        m_totalAttempts += m_attemptedChanges;
        m_attemptedChanges = 0;
    }
}

/* Swap randomly, adjust the temperature as needed, then save the results once the
 * assignments are considered to be solved. */
void Annealer::Solver() {
    m_start = std::chrono::steady_clock::now();

    while(!m_solved) {
        RandomSwap();
        ReduceTemperature();
    }

    m_end = std::chrono::steady_clock::now();
    SaveResultsToFile();
}

/* Choose two different rooms, then randomly pick between 1 of 2 swap methods. */
void Annealer::RandomSwap() {
    m_attemptedChanges++;

    int room1 = m_whichRoom(m_rng);
    int room2 = m_whichRoom(m_rng);

    while(room1 == room2) {
        room2 = m_whichRoom(m_rng);
    }

    if(m_whichSwap(m_rng) == 0) {
        SmallSwap(room1, room2);
    }
    
    else {
        LargeSwap(room1, room2);
    }
}

/* Switch a roommate at random from the first room with another roommate at random
 * from the second room. */
void Annealer::SmallSwap(const int room1, const int room2) {
    Room changedRoom1 = Room(m_rooms[room1]);
    Room changedRoom2 = Room(m_rooms[room2]);
    int roommate1 = m_whichRoommate(m_rng);
    int roommate2 = m_whichRoommate(m_rng);

    int temp = changedRoom1.roommate[roommate1];
    changedRoom1.roommate[roommate1] = changedRoom2.roommate[roommate2];
    changedRoom2.roommate[roommate2] = temp;

    changedRoom1.CalculateFitness(m_compatibilities);
    changedRoom2.CalculateFitness(m_compatibilities);

    if(AcceptChange(m_rooms[room1].fitnessScore + m_rooms[room2].fitnessScore,
                    changedRoom1.fitnessScore + changedRoom2.fitnessScore)) {
        m_rooms[room1] = changedRoom1;
        m_rooms[room2] = changedRoom2;
        m_acceptedChanges++;
    }
}

/* Switch the first two roommates in the first room with the second two roommates
 * in the second room. */
void Annealer::LargeSwap(const int room1, const int room2) {
    Room changedRoom1 = Room(m_rooms[room1]);
    Room changedRoom2 = Room(m_rooms[room2]);

    int temp;
    for(int studentPair = 0; studentPair < NUM_STUDENTS_PER_ROOM / 2; studentPair++) {
        temp = changedRoom1.roommate[studentPair];
        changedRoom1.roommate[studentPair] = changedRoom2.roommate[NUM_STUDENTS_PER_ROOM - studentPair - 1];
        changedRoom2.roommate[NUM_STUDENTS_PER_ROOM - studentPair - 1] = temp;
    }

    changedRoom1.CalculateFitness(m_compatibilities);
    changedRoom2.CalculateFitness(m_compatibilities);

    if(AcceptChange(m_rooms[room1].fitnessScore + m_rooms[room2].fitnessScore,
                    changedRoom1.fitnessScore + changedRoom2.fitnessScore)) {
        m_rooms[room1] = changedRoom1;
        m_rooms[room2] = changedRoom2;
        m_acceptedChanges++;
    }
}

/* Always accept a chance if it's better. Only accept a worse change if it falls within
 * the probability chance. */
bool Annealer::AcceptChange(const int oldFitness, const int newFitness) {
    if(newFitness <= oldFitness) {
        return true;
    }
    
    double acceptanceChance = std::exp((oldFitness - newFitness)/m_temperature);
    if(acceptanceChance >= m_acceptanceProbability(m_rng)) {
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

    int averageScore = 0, bestScore = m_rooms[0].fitnessScore, worstScore = m_rooms[0].fitnessScore;
    
    for(int room = 0; room < NUM_ROOMS; room++) {
        averageScore += m_rooms[room].fitnessScore;

        if(bestScore > m_rooms[room].fitnessScore) {
            bestScore = m_rooms[room].fitnessScore;
        }

        else if(worstScore < m_rooms[room].fitnessScore) {
            worstScore = m_rooms[room].fitnessScore;
        }

        m_rooms[room].SortRoommates();
    }

    averageScore /= NUM_ROOMS;
    auto elapsedTime = m_end - m_start;

    saveFile <<   "Initial temperature:   " << std::setw(8) << m_startingTemperature
             << "\nGeometric reduction:   " << std::setw(8) << m_reduction
             << "\nBest fitness score:    " << std::setw(8) << bestScore
             << "\nWorst fitness score:   " << std::setw(8) << worstScore
             << "\nAverage fitness score: " << std::setw(8) << averageScore
             << "\nTotal swaps:           " << std::setw(8) << m_totalChanges
             << "\nTotal attempts:        " << std::setw(8) << m_totalAttempts
             << "\nTime to solve (ms):    " << std::setw(8)
             << std::chrono::duration <double, std::milli> (elapsedTime).count() 
             << "\n";

    for(int room = 0; room < NUM_ROOMS; room++) {
        saveFile << "\nRoom #" << room + 1 << ": " << m_rooms[room].PrintRoom();
    }

    saveFile.close();
    return true;
}