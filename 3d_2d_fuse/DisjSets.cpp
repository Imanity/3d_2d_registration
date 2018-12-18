#include "DisjSets.h"

DisjSets::DisjSets(int n) {
	pre.clear();
	rank.clear();
	for (int i = 0; i < n; ++i) {
		pre.push_back(i);
		rank.push_back(1);
	}
}

DisjSets::~DisjSets() {
}

void DisjSets::unite(int x, int y) {
	int rootx, rooty;
	rootx = find_pre(x);
	rooty = find_pre(y);
	if (rootx == rooty) {
		return;
	}
	if (rank[rootx] > rank[rooty]) {
		pre[rooty] = rootx;
	} else {
		if (rank[rootx] == rank[rooty]) {
			rank[rooty]++;
		}
		pre[rootx] = rooty;
	}
}

bool DisjSets::is_same(int x, int y) {
	return find_pre(x) == find_pre(y);
}

int DisjSets::find_pre(int x) {
	if (pre[x] == x) {
		return x;
	}
	return pre[x] = find_pre(pre[x]);
}

std::vector<int> DisjSets::output_cluster() {
	std::vector<int> cluster(pre.size(), -1);
	int cluster_id = 0;
	while (true) {
		int id = -1;
		for (int i = 0; i < cluster.size(); ++i) {
			if (cluster[i] < 0) {
				id = i;
				break;
			}
		}
		if (id < 0)
			break;
		cluster[id] = cluster_id;
		for (int i = id + 1; i < cluster.size(); ++i) {
			if (is_same(i, id)) {
				cluster[i] = cluster_id;
			}
		}
		cluster_id++;
	}
	return cluster;
}

int find_bin(std::vector<int> &v, int x) {
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] == x)
			return i;
	}
	return -1;
}
