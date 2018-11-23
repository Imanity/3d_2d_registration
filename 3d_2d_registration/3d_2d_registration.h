#pragma once

#ifdef DLL_EXPORTS
#define REGISTRATION_EXPORT __declspec(dllexport)
#else
#define REGISTRATION_EXPORT __declspec(dllimport)
#endif

#include "VolumeData.h"
#include "ImageData.h"
#include "Solver.h"

class REGISTRATION_EXPORT RegistrationSolver {
public:
	RegistrationSolver(VolumeData *v, std::vector<ImageData *> imgs, std::vector<Eigen::Vector3d> Rs);
	~RegistrationSolver();
	
	std::vector<Eigen::VectorXd> Solve();

public:
	Solver solver;
};
