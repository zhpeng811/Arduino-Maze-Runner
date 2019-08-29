# -------------------------------
# Names: Meilin Lyu, Ze Hui Peng
# IDs: 1577829, 1594884
# CMPUT 275 Winter 2019
# Final Project: Arduino Maze Runner
# -------------------------------


CC = g++
OBJS = digraph.o util.o maze_generator.o serialport.o unionfind.o algorithms.o
CFLAGS = -c -Wall -O2 -std=c++11
LFLAGS = 
PROGRAM = maze_generator

all: $(PROGRAM)
	
$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LFLAGS)

serialport.o: serialport.cpp serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

maze_generator.o : maze_generator.cpp
	$(CC) maze_generator.cpp -o maze_generator.o $(CFLAGS)

digraph.o: digraph.h digraph.cpp
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

util.o: util.cpp util.h
	$(CC) util.cpp -o util.o $(CFLAGS)

unionfind.o: unionfind.cpp unionfind.h
	$(CC) unionfind.cpp -o unionfind.o $(CFLAGS)

algorithms.o: algorithms.cpp algorithms.h
	$(CC) algorithms.cpp -o algorithms.o $(CFLAGS)

clean:
	@rm -f $(OBJS)
	@rm -f $(PROGRAM)