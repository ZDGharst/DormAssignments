run_dorm_assignments_program:
	g++ -std=c++17 -o bin/DormAssignments.o -I include/ src/*.cpp
	./bin/DormAssignments.o
