#include <iostream>
#include <random>

#include "globals.h"
#include "room.h"
#include "annealer.h"

int main() {
    Annealer annealer("data/roommates.txt");
    annealer.Solver();

    return 0;
}