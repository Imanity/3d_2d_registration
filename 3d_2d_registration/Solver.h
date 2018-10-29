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
	Eigen::Matrix3d rotate_vector2matrix(Eigen::Vector3d &v);

private:
	VolumeData * volume;
	std::vector<Eigen::Vector3d> voxels;
	std::vector<ImageData *> images;
	std::vector<Eigen::VectorXd> transforms;
};
