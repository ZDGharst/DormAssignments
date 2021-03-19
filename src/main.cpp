#include <iostream>
#include <random>
#include <string>

#include "globals.h"
#include "room.h"
#include "annealer.h"

using namespace DormAssignments::Globals;

int main(int argc, char* argv[]) {
    std::string inputFile = "data/roommates.txt";
    int temperature = INITIAL_TEMPERATURE;
    double reduction = GEOMETRIC_TEMP_REDUCTION;

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
                if(temperature <= 0) {
                    std::cout << "DormAssignments: temperature operand must be positive integer\n";
                    return -2;
                }
            }
            else {
                std::cout << "DormAssignments: missing temperature operand after '--temperature'\n";
                return -1;
            }
        }
        else if(argString == "--reduction") {
            if(arg + 1 < argc) {
                reduction = std::stod(argv[arg + 1]);
                if(reduction <= 0 || reduction >= 1) {
                    std::cout << "DormAssignments: reduction operand must be between 0 and 1\n";
                    return -2;
                }
            }
            else {
                std::cout << "DormAssignments: missing reduction operand after '--reduction'\n";
                return -1;
            }
        }
    }

    Annealer annealer(inputFile, temperature, reduction);
    annealer.Solver();

    return 0;
}