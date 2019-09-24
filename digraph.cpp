
#include "digraph.h"

void Digraph::addVertex(int v) {
  // If it already exists, does nothing.
  // Otherwise, adds v with an empty adjacency set.
  nbrs[v];
}

void Digraph::addEdge(int u, int v) {
  addVertex(v);
  nbrs[u].insert(v); // will also add u to nbrs if it was not there already
}


bool Digraph::isVertex(int v) {
  return nbrs.find(v) != nbrs.end();
}

bool Digraph::isEdge(int u, int v) {
  // check that u is in the keys and that it's associated set contains v
  return nbrs.find(u) != nbrs.end() && nbrs[u].find(v) != nbrs[u].end();
}

unordered_set<int>::const_iterator Digraph::neighbours(int v) const {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.begin();
}

unordered_set<int>::const_iterator Digraph::endIterator(int v) const {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.end();
}

unordered_set<int> Digraph::all_neighbours(int v) {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second;
}

int Digraph::numNeighbours(int v) {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.size();
}

// find a neighbour that have not been visited
// if all neighbours are visited , return -1
int Digraph::random_nbr(int v, unordered_map<int,int> visited) {
  unordered_set<int> s_nbrs = all_neighbours(v);
  // random number between 0 to numNeibours - 1
  // there is a maximum of four neighbours of each node
  int num_nbrs, random_num, nbr;
  do {
    num_nbrs = s_nbrs.size();
    random_num = randint(0, num_nbrs - 1);

    auto iter = s_nbrs.begin();
    for (int i = 0; i < random_num; i++) {
      iter++;
    }
    nbr = *iter;

    if (visited.find(nbr) != visited.end() ) {
      s_nbrs.erase(iter);
    }
    else {
      break;
    }
    
    if ( s_nbrs.size() == 0) {
      return -1;
    }
  } while (true);

  return nbr;
}

int Digraph::size() {
  return nbrs.size();
}

// find all vertices and return it in a vector of int
vector<int> Digraph::vertices() {
  vector<int> v;
  for (unordered_map<int, unordered_set<int>>::const_iterator it = nbrs.begin();
    it != nbrs.end(); it++) {
      v.push_back(it->first);
    }
  return v;
}

// find all edges and return it in a vector of pair<int, int>
vector<PII> Digraph::edges() {
  vector<PII> edge;
  unordered_set<int> edge_nbr;
  int vertex;
  for (auto iter = nbrs.begin(); iter != nbrs.end(); iter++) {
    vertex = iter->first;
    edge_nbr = iter->second;
    // each edge_nbr is also an unordered set
    for (auto it = edge_nbr.begin(); it != edge_nbr.end(); it++) {
      edge.push_back(PII(vertex, *it));
    }
  }
  return edge;
}

bool Digraph::isWalk(vector<int> walk) {
  if (walk.size() == 0)
    return false;
  if (walk.size() == 1)
    return isVertex(walk[0]);
  for (vector<int>::size_type i=0; i<walk.size()-1; i++)
    if (!isEdge(walk[i], walk[i+1]))
      return false;

  return true;
}

bool Digraph::isPath(vector<int> path) {
  set<int> s(path.begin(), path.end());
  if (s.size() < path.size())
    return false;

  return isWalk(path);
}
