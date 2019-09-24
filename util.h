
#ifndef _UTIL_H_
#define _UTIL_H_

#include <random>

using namespace std;

// maze size constants are declared here
#define MAZE_HEIGHT 30
#define MAZE_WIDTH 40 
#define MAZE_SIZE MAZE_HEIGHT * MAZE_WIDTH

// generate a random number in the range [min,max]
int randint(int min, int max);

// generate a random node(vertex) with only even identifiers allowed
int random_node();

// break the wall between two vertices
void break_wall(int vertex, int nbr, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]);

#endif
