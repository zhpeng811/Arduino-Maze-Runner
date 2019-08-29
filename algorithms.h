/*
-------------------------------
Names: Meilin Lyu, Ze Hui Peng
IDs: 1577829, 1594884
CMPUT 275 Winter 2019
Final Project: Arduino Maze Runner
-------------------------------
*/

#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include "digraph.h"
#include "util.h"
#include "unionfind.h"

// all algorithms used for the project are declared here

// declarations for all algorithms, for details, see algorithms.cpp
void BreadthFirstSearch(const Digraph& graph, int startVertex,
                        unordered_map<int, int>& searchTree) ;

void Maze_DFS(Digraph& graph, int N, unordered_map<int,int>& visited, queue<int>& Q, 
              int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]);

void Kruskal_MST(Digraph& graph, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]);

void Prim_MST(Digraph& graph, int startvertex, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]);

#endif