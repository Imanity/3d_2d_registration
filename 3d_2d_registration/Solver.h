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
	void addImageData(ImageData *);

	void solve(std::vector<Eigen::Vector3d> initial_value);

private:
	VolumeData * volume;
	std::vector<ImageData *> images;
};
