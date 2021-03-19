compile_and_run:
	g++ -std=c++17 -o obj/DormAssignments.o -I include/ src/*.cpp
	./obj/DormAssignments.o

profiler: diagnostics strace

diagnostics:
	/usr/bin/time -v -p -o profile/time.log ./obj/DormAssignments.o
	
strace:
	strace -o profile/strace.log ./obj/DormAssignments.o
