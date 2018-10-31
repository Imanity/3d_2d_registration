#include "KDTree.h"

using namespace std;
using namespace Eigen;

KDTree::KDTree() {
}

KDTree::KDTree(std::vector<Eigen::Vector2d> &points) {
	root = buildTree(points);
}

KDTree::~KDTree() {
}

KDTreeNode *KDTree::buildTree(std::vector<Eigen::Vector2d> &points) {
	int n = points.size();
	if (n == 0) {
		return NULL;
	}
	KDTreeNode *node = new KDTreeNode;

	// Get split field
	double variance[2] = { 0, 0 };
	double mu[2] = { 0, 0 };
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < 2; ++j)
			mu[j] += points[i](j);
	for (int i = 0; i < 2; ++i)
		mu[i] /= n;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < 2; ++j)
			variance[j] += (points[i](j) - mu[j]) * (points[i](j) - mu[j]);
	int split_field = 0;
	if (variance[1] > variance[0])
		split_field = 1;
	node->split_field = split_field;

	// Get node data
	double *node_val = new double[n];
	for (int i = 0; i < n; ++i)
		node_val[i] = points[i](split_field);
	sort(node_val, node_val + n);
	double mid_val = node_val[n / 2];
	int mid_val_id = 0;
	for (int i = 0; i < n; ++i) {
		if (points[i](split_field) == mid_val) {
			mid_val_id = i;
			break;
		}
	}
	node->position(0) = points[mid_val_id](0);
	node->position(1) = points[mid_val_id](1);


	// Get child
	vector<Vector2d> left_points;
	vector<Vector2d> right_points;
	for (int i = 0; i < n; ++i) {
		if (i == mid_val_id)
			continue;
		Vector2d p;
		for (int j = 0; j < 2; ++j)
			p(j) = points[i](j);
		if (points[i](split_field) <= mid_val)
			left_points.push_back(p);
		else
			right_points.push_back(p);
	}
	node->kd_left = buildTree(left_points);
	node->kd_right = buildTree(right_points);

	if (node->kd_left != NULL) {
		node->kd_left->brother = node->kd_right;
		node->kd_left->parent = node;
		node->kd_left->is_left = true;
		node->kd_left->is_right = false;
	}
	if (node->kd_right != NULL) {
		node->kd_right->brother = node->kd_left;
		node->kd_right->parent = node;
		node->kd_right->is_left = false;
		node->kd_right->is_right = true;
	}

	return node;
}

Eigen::Vector2d KDTree::search(Eigen::Vector2d target) {
	Eigen::Vector2d nearest;
	double best_dist = DBL_MAX;
	getNearestNeighbor(target, root, nearest, best_dist, Rect(-DBL_MAX, -DBL_MAX, DBL_MAX, DBL_MAX));
	return nearest;
}

void KDTree::getNearestNeighbor(Eigen::Vector2d &q, KDTreeNode *t, Eigen::Vector2d &p, double &best_dist, Rect BB) {
	if (t == NULL || BB.dist(q) > best_dist) {
		return;
	}
	double dis = dist(q, t->position);
	if (dis < best_dist) {
		best_dist = dis;
		p = t->position;
	}
	int split_field = t->split_field;
	if (q(split_field) < t->position(split_field)) {
		getNearestNeighbor(q, t->kd_left, p, best_dist, BB.trimLeft(split_field, t->position));
		getNearestNeighbor(q, t->kd_right, p, best_dist, BB.trimRight(split_field, t->position));
	}
	else {
		getNearestNeighbor(q, t->kd_right, p, best_dist, BB.trimRight(split_field, t->position));
		getNearestNeighbor(q, t->kd_left, p, best_dist, BB.trimLeft(split_field, t->position));
	}
}

double KDTree::dist(Eigen::Vector2d &p, Eigen::Vector2d &q) {
	double dis = 0;
	for (int i = 0; i < 2; ++i)
		dis += (p(i) - q(i)) * (p(i) - q(i));
	dis = sqrt(dis);
	return dis;
}
