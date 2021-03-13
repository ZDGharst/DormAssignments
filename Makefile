compile_and_run:
	g++ -std=c++17 -o bin/DormAssignments.o -I include/ src/*.cpp
	./bin/DormAssignments.o

debug_all: diagnostics strace

diagnostics:
	/usr/bin/time -v -p -o debug/system.log ./bin/DormAssignments.o
	
strace:
	strace -o debug/strace.log .bin/DormAssignments.o
