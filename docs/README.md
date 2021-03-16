### Dorm Assignments
This program assigns two hundred roommates to fifty 4-person rooms using simulated annealing. Receives input from the file at [data/roommates.txt](../data/roommates.txt). Input is expected to be 200 rows of 200 integer values separated by spaces.

## Build
Type ```make``` in the terminal in the project folder to compile the project and run it once. After this, the program is located at ```./bin/DormAssignments.o```. You can run this again without compiling from the root of the project folder (or it won't find the input file).

Requires C++17: ```inline constexpr```. Created, debugged, tested, and profiled on Ubuntu 20.04.

## Constants
The file [globals.h](../include/globals.h) contains all the constants used by the program. These can be modified to speed up the program, get better results, or change the set up of rooms (8 student rooms with 400 students, for example). Take care when changing these globals, the program can break if you blindly change them (too low of a temperature could result in a division by zero).

## Results
Results are stored in [results/](../results/) and named after the timestamp of program run time.

## Author
Program by [Zach Gharst](http://gharst.engineer) for CS461 - Introduction to Artificial Intelligence at UMKC.