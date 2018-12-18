#pragma once

#include <vector>

class DisjSets {
public:
	DisjSets(int n);
	~DisjSets();

	void unite(int x, int y);
	bool is_same(int x, int y);

	std::vector<int> output_cluster();

public:
	std::vector<int> pre;
	std::vector<int> rank;

private:
	int find_pre(int x);
};

extern int find_bin(std::vector<int> &v, int x);
