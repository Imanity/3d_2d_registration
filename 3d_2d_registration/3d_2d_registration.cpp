#define DLL_EXPORTS
#include "3d_2d_registration.h"

RegistrationSolver::RegistrationSolver(VolumeData *v, std::vector<ImageData *> imgs, std::vector<Eigen::Vector3d> Rs) {
	solver.addVolumeData(v);
	for (int i = 0; i < imgs.size(); ++i)
		solver.addImageData(imgs[i], Rs[i]);
}

RegistrationSolver::~RegistrationSolver() {
}

std::vector<Eigen::VectorXd> RegistrationSolver::Solve() {
	solver.showProjectionResult("../Output/initial");

	solver.solve();

	solver.showProjectionResult("../Output/result");

	return solver.transforms;
}
