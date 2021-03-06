#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    int compatibilities[40000];
    std::ifstream compatibilityFile;
    compatibilityFile.open("data/roommates.txt");

    for(int i = 0; i < 40000; i++) {
        compatibilityFile >> compatibilities[i];
    }

    compatibilityFile.close();

    std::cout << compatibilities[203] << "\n";

    return 0;
}