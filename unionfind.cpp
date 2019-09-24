
// Note: this file is taken from CMPUT 275 Lecture 16 code from eClass
// Source&Credit: Professor Zachary Friggstad


#include "unionfind.h"
#include <cassert>

void UnionFind::addItem(int v) {
	// does nothing if v is already in the set
	if (parent.find(v) == parent.end()) {
		parent[v] = v;
		rank[v] = 0;
	}
}

int UnionFind::find(int v) {
	// make sure v is in the data structure
	assert(parent.find(v) != parent.end());

	while (parent[v] != v) {
		v = parent[v];
	}
	return v;
}

bool UnionFind::merge(int u, int v) {
	int rep_u = find(u), rep_v = find(v);

	// already in same component
	if (rep_u == rep_v) {
		return false;
	}

	// redirect the smaller rank to point to the larger rank
	if (rank[rep_u] < rank[rep_v]) {
		parent[rep_u] = rep_v;
	}
	else if (rank[rep_u] > rank[rep_v]) {
		parent[rep_v] = rep_u;
	}
	else {
		// if ranks are the same, point rep_u to rep_v and increase the rank of rep_v
		parent[rep_u] = rep_v;
		++rank[rep_v];
	}

	return true;
}
