#include <iostream>
#include <random>

#include "globals.h"
#include "room.h"
#include "annealer.h"

int main(int argc, char* argv[]) {
    Annealer annealer;
    if(!annealer.LoadCompatibilities("data/roommates.txt")) {
        return -1;
    }

    annealer.PreloadRooms();
    annealer.SaveResultsToFile("output.txt");

    return 0;
}