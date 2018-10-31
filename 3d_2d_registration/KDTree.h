#pragma once

#include <vector>
#include <stack>
#include <algorithm>

#include <Eigen/Dense>
#include <iostream>

struct KDTreeNode {
	Eigen::Vector2d position;
	int split_field;
	struct KDTreeNode *kd_left = NULL;
	struct KDTreeNode *kd_right = NULL;
	struct KDTreeNode *parent = NULL;
	struct KDTreeNode *brother = NULL;
	bool is_left = false, is_right = false;
};

class Rect {
public:
	double x, y, sx, sy;
	Rect() {}
	Rect(double x_, double y_, double sx_, double sy_) : x(x_), y(y_), sx(sx_), sy(sy_) {}
	~Rect() {}
	Rect trimLeft(int split_field, Eigen::Vector2d &p) {
		if (split_field == 0)
			return Rect(x, y, p(0), sy);
		else
			return Rect(x, y, sx, p(1));
	}
	Rect trimRight(int split_field, Eigen::Vector2d &p) {
		if (split_field == 0)
			return Rect(p(0), y, sx, sy);
		else
			return Rect(x, p(1), sx, sy);
	}
	double dist(Eigen::Vector2d &p) {
		double dx = 0, dy = 0;
		if (p(0) < x)
			dx = x - p(0);
		else if (p(0) > sx)
			dx = p(0) - sx;
		if (p(1) < y)
			dy = y - p(1);
		else if (p(1) > sy)
			dy = p(1) - sy;
		return sqrt(dx * dx + dy * dy);
	}
};

class KDTree {
public:
	KDTree();
	KDTree(std::vector<Eigen::Vector2d> &points);
	~KDTree();

	Eigen::Vector2d search(Eigen::Vector2d target);

private:
	KDTreeNode * buildTree(std::vector<Eigen::Vector2d> &points);
	void getNearestNeighbor(Eigen::Vector2d &q, KDTreeNode *t, Eigen::Vector2d &p, double &best_dist, Rect BB);
	double dist(Eigen::Vector2d &p, Eigen::Vector2d &q);

private:
	KDTreeNode * root;
};
