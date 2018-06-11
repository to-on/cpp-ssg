all: Solver.o Generator.o main.o
	g++ -std=c++11 -O2 -o SimpleSudokuGenerator Solver.o Generator.o main.o

Solver.o: Solver.cpp
	g++ -std=c++11 -O2 -c Solver.cpp

Generator.o: Generator.cpp Solver.o
	g++ -std=c++11 -O2 -c Generator.cpp

main.o: main.cpp Solver.o Generator.o
	g++ -std=c++11 -O2 -c main.cpp

.PHONY: clean
clean:
	rm SimpleSudokuGenerator Solver.o Generator.o main.o
