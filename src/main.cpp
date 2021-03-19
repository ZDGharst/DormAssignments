#include <iostream>
#include <random>
#include <string>

#include "globals.h"
#include "room.h"
#include "annealer.h"

int main(int argc, char* argv[]) {
    std::string inputFile = "data/roommates.txt";
    for(int arg = 1; arg < argc; arg++) {
        if(std::string(argv[arg]) == "-i") {
            if(arg + 1 < argc) {
                inputFile = std::string(argv[arg + 1]);
            }
            else {
                std::cout << "DormAssignments: missing input file operand after '-i'\n";
                return -1;
            }
        }
    }

    Annealer annealer(inputFile);
    annealer.Solver();

    return 0;
}