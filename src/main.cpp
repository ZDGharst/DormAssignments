#include <iostream>
#include <random>
#include <string>

#include "globals.h"
#include "room.h"
#include "annealer.h"

int main(int argc, char* argv[]) {
    std::string inputFile = "data/roommates.txt";
    int temperature = INITIAL_TEMPERATURE;

    for(int arg = 1; arg < argc; arg++) {
        std::string argString = std::string(argv[arg]);
        if(argString == "-i") {
            if(arg + 1 < argc) {
                inputFile = std::string(argv[arg + 1]);
            }
            else {
                std::cout << "DormAssignments: missing input file operand after '-i'\n";
                return -1;
            }
        }
        else if(argString == "--temperature") {
            if(arg + 1 < argc) {
                temperature = std::stoi(argv[arg + 1]);
                if(temperature < 0) {
                    std::cout << "DormAssignments: temperature operand must be positive integer\n";
                    return -2;
                }
            }
            else {
                std::cout << "DormAssignments: missing temperature operand after '--temperature'\n";
                return -1;
            }
        }
    }

    Annealer annealer(inputFile, temperature);
    annealer.Solver();

    return 0;
}