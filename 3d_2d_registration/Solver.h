#pragma once

#include <vector>
#include <Eigen/Dense>

#include "VolumeData.h"
#include "ImageData.h"

class TransformMatrix {
public:
	TransformMatrix() : yaw(0), pitch(0), roll(0), tx(0), ty(0) { getMatrix(); }
	TransformMatrix(double y, double p, double r, double t_x, double t_y) : yaw(y), pitch(p), roll(r), tx(t_x), ty(t_y) { getMatrix(); }
	TransformMatrix(Eigen::VectorXd v) : yaw(v(0)), pitch(v(1)), roll(v(2)), tx(v(3)), ty(v(4)) { getMatrix(); }
	~TransformMatrix() {}
	void getMatrix();

	double yaw, pitch, roll;
	double tx, ty;
	Eigen::Matrix3d M;
};

class Solver {
public:
	Solver();
	~Solver();

	void addVolumeData(VolumeData *);
	void addImageData(ImageData *);

	void solve(std::vector<Eigen::Vector3d> initial_value);

	void show2d();

private:
	std::vector<Eigen::Vector2d> transformVolume(VolumeData * v, TransformMatrix &m);
	Eigen::Vector2d transformPoint(Eigen::Vector3d &p, TransformMatrix &m);

private:
	VolumeData * volume;
	std::vector<ImageData *> images;
	std::vector<TransformMatrix> transforms;
};
