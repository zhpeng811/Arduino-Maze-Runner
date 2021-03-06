
#include "algorithms.h"

/* Source: Cmput 275 Lecture 12 code available on eClass
   Note: Code is directly taken from the eClass, comments are added to
         note that we understand all concepts
   Running Time: O(|E|), |E| is the number of edges
   Args: Digraph& graph: a reference to the maze solution graph used
                         to find the path from start to end vertex
         int startVertex: the start vertex to find its shortest path
                          to all reachable vertices. For the maze the
                          vertex will always be 0.
         unordered_map<int,int> searchTree: a reference to the searchTree
                                            used to find the path.
*/
void BreadthFirstSearch(const Digraph& graph, int startVertex,
                        unordered_map<int, int>& searchTree) {

  searchTree.clear(); // make sure we start with a empty tree

  // the start vertex have no predecessor
  searchTree[startVertex] = -1;

  queue<int> q;
  q.push(startVertex);

  // continuously poping items from the queue and find its neighbours, if the
  // neighbour is not in the searchtree, push the neighbour into the queue and
  // add it to the searchtree. This will find the shortest path to all reachable
  // vertices from the start vertex.
  // Note: since each maze generation algorithm we used(DFS and MSTs) guarantee
  // no cycles in the path, so any path in the maze is a shortest path.
  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
      if (searchTree.find(*iter) == searchTree.end()) {
        searchTree[*iter] = v;
        q.push(*iter);
      }
    }
  }

}


/* Source: https://www.algosome.com/articles/maze-generation-depth-first.html
  pseudocode:
  step 1: Randomly select a node (or cell) N.
  step 2: Push the node N onto a queue.
  step 3: Mark the cell N as visited.
  step 4: Randomly select an adjacent cell A of node N that has not been visited. 
          If all the neighbors of N have been visited:
          - Continue to pop items off the queue Q until a node is encountered with 
          at least one non-visited neighbor - assign this node to N and go to step 4.
          - If no nodes exist: stop.
  step 5: Break the wall between N and A.
  step 6: Assign the value of A to N.
  step 7: Go to step 2
*/
/* Description: DepthFirstSearch algorithm used to generate a random maze
   Running Time: O(|E|), |E| is the number of edges
   Args: Digraph& graph: a reference to the default maze graph
         int N: vertex that is currently been processed
                at first call this vertex is random generated by the
                random_node() function.
         unordered_map<int,int>& visited: stored all vertices that have been
                                          visited/processed.
         queue<int>& Q: a queue used to storing visited elements and popping
                        elements when a node have no unvisited neighbours.
         int &arr[][]: a reference to the maze_map array.
*/
void Maze_DFS(Digraph& graph, int N, unordered_map<int,int>& visited, 
              queue<int>& Q, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]) { 
  int nbr, node;
  // step 1: passed as argument -> N
  Q.push(N); // step 2
  visited[N] = -1; // step 3

  do {
    nbr = graph.random_nbr(N, visited); //step 4
    if (nbr == -1) {
      node = Q.front();
      Q.pop();
      N = node;
    }
    if (Q.empty()) {
      return;
    }
  } while (nbr == -1);

  // break_wall function is defined in util.cpp
  break_wall(N, nbr, arr); // step 5

  N = nbr; // step 6
  Maze_DFS(graph, N, visited, Q, arr); // step 7: recursion
}


/* Source: pseudocode from Lecture 16 slides on eClass 
   pseudocode(modified version for random maze generation):
   step 1. uf <- UnionFind()
   step 2. add all vertices to uf as a set with one element with rank 0
   step 3. for each edge uv that is randomly selected:
            - if uf.find(u) != uf.find(v):
                 - uf.union(u,v) (merge u,v to a single set)
                 - remove the wall between u and v
*/
/* Description: Kruskal's Minimum Spanning Tree algorithm used
                for generating a random maze
   Running Time: O(|E|*log(|E|)), |E| is the number of edges
   Args: Digraph& graph: a reference to the default maze graph
         int &arr[][]: a reference to the maze_map array.
*/
void Kruskal_MST(Digraph& graph, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]) {
    // obtain all vertices and edges using the public methods
    // in Digraph class
    vector<int> vertices = graph.vertices();
    vector<PII> edge = graph.edges();
    UnionFind uf; // step 1
    int u, v, rand_num; 

    // step 2
    for (auto iter = vertices.begin(); iter != vertices.end(); iter++) {
        uf.addItem(*iter);
    }

    // step 3
    while ( !edge.empty() ) {
        // randomly selecting an edge
        rand_num = randint(0, edge.size() - 1);
        // we used vector because its iterator is a random-access iterator
        // it is easier to access the n'th element, where n is the rand_num
        // generated using randint() function
        // can also use vector.at(n) to access element, 
        // but erase() function below need the iterator as the argument
        auto iter = edge.begin() + rand_num;

        u = iter->first;
        v = iter->second;
        // condition in the loop
        if (uf.find(u) != uf.find(v)) {
            uf.merge(u,v);
            break_wall(u, v, arr);
        }
        // this might cause more time if the edge removed is closer
        // to the front of the vector. Since it will take time moving
        // elements, but since we doesn't have a enormous amount of edges
        // to consider, this can be assumed in constant running time
        edge.erase(iter);
    }

}


/* Source: http://mcicsci.blogspot.com/2013/12/randomized-prims-algorithm.html
   1. Choose a random cell within the maze grid and design it as a start cell.
   2. Add the start cell to (by now empty) inCells set.
   3. Mark cells around the start cell as frontier, i.e. add them to frontierCells set.
   4. While frontierCells set is not empty:
        1. Choose a random frontier cell cF from frontierCells.
        2. Choose a random in-cell cI adjacent to cF.
        3. Add cF to inCells.
        4. Mark all out-cells around cF as frontier.
        5. Add a path between cI and cF to the maze paths set.
        6. Remove cF from frontierCells set
*/

/* Description: Prim's Minimum Spanning Trees algorithms used for
                generate a random maze.
   A note on the algorithm: Prim's algorithm is a greedy algorithm
                            that the randomly generated maze will
                            have enormous amount of very short 
                            dead ends.
   Running Time: O(|E|log(|E|)), |E| is the number of edges
   Args: Digraph& graph: a reference to the default maze graph
         int startvertex: the starting vertex randomly generated
                          by the random_node() function
         int &arr[][]: a reference to the maze_map array. 
*/
void Prim_MST(Digraph& graph, int startvertex, int (&arr)[MAZE_HEIGHT][MAZE_WIDTH]) {
    unordered_set<int> inCells, nbrs, frontierCells;
    unordered_map<int,int> visited;
    int rand_num, cF, cI, cell;
    // step 1 -> passed as argument startvertex

    // step 2
    inCells.insert(startvertex);

    // step 3
    nbrs = graph.all_neighbours(startvertex);
    for (auto iter = nbrs.begin(); iter != nbrs.end(); iter++) {
        frontierCells.insert(*iter);
    }

    // step 4
    while (!frontierCells.empty()) {
        // while loop step 1
        rand_num = randint(0, frontierCells.size() - 1);
        auto cFiter = frontierCells.begin();
        // unordered set uses forward iterator
        // which only supports post increment 
        for (int i = 0; i < rand_num; i++) {
            cFiter++; 
        }
        cF = *cFiter;

        // while loop step 2
        cI = graph.random_nbr(cF, visited);
        // find neighbours of the frontier cell
        // until that neighbour is in the inCells set
        while (inCells.find(cI) == inCells.end()) {
            visited[cI] = -1;
            cI = graph.random_nbr(cF, visited);
        }
        visited.clear();

        
        // while loop step 3
        inCells.insert(cF);
       
        // while loop step 4
        nbrs = graph.all_neighbours(cF);
        for (auto iter = nbrs.begin(); iter != nbrs.end(); iter++) {
            cell = *iter;
            if (inCells.find(cell) == inCells.end()) {
                frontierCells.insert(cell);
            }
        }

        // while loop step 5
        break_wall(cI, cF, arr);

        // while loop step 6
        // unordered set erase takes constant time
        frontierCells.erase(cF);
    }
}
