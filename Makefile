run_dorm_assignments_program:
	g++ -o bin/DormAssignments.o -I include/ src/*.cpp
	./bin/DormAssignments.o
