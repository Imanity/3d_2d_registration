#include "Solver.h"


Solver::Solver() {
}

Solver::~Solver() {
}

void Solver::addVolumeData(VolumeData *vol) {
	volume = vol;
}

void Solver::addImageData(ImageData *img) {
	images.push_back(img);
}

void Solver::solve(std::vector<Eigen::Vector3d> initial_value) {
}
