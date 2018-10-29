#pragma once

#include <vector>
#include <Eigen/Dense>

#include "VolumeData.h"
#include "ImageData.h"

class Solver {
public:
	Solver();
	~Solver();

	void addVolumeData(VolumeData *);
	void addImageData(ImageData *, Eigen::Vector3d R_vector);

	void solve();

	void showProjectionResult(std::string attr);

private:
	VolumeData * volume;
	std::vector<Eigen::Vector3d> voxels;
	std::vector<ImageData *> images;
	std::vector<Eigen::VectorXd> transforms;
};

static Eigen::Matrix3d rotate_vector2matrix(Eigen::Vector3d &v);
static double foo(const Eigen::VectorXd &x, Eigen::VectorXd &grad);
static double loss_func(const Eigen::VectorXd &x);
extern std::vector<Eigen::Vector3d> X;
extern std::vector<ImageData *> Y;
