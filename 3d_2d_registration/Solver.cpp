#include "Solver.h"
#include "Projection.h"

Solver::Solver() {
}

Solver::~Solver() {
}

void Solver::addVolumeData(VolumeData *vol) {
	volume = vol;
	voxels.clear();
	for (int i = 0; i < volume->nvox; ++i) {
		if (volume->data[i]) {
			Eigen::Vector3i voxel_idx = volume->coord(i);
			Eigen::Vector3d voxel;
			voxel << volume->dx * voxel_idx(0), volume->dy * voxel_idx(1), volume->dz * voxel_idx(2);
			voxels.push_back(voxel);
		}
	}
}

void Solver::addImageData(ImageData *img, Eigen::Vector3d R_vector) {
	images.push_back(img);
	Eigen::Matrix3d R = rotate_vector2matrix(R_vector);

	Eigen::Vector3d voxel_center;
	voxel_center << volume->dx * volume->nx / 2, volume->dy * volume->ny / 2, volume->dz * volume->nz / 2;
	Eigen::Vector3d voxel_projected = R * voxel_center;
	Eigen::Vector3d T;
	T << -voxel_projected(0), -voxel_projected(1), img->distance_source_patient;

	Eigen::VectorXd x(7);
	x << R_vector(0), R_vector(1), R_vector(2), T(0), T(1), T(2), img->distance_source_detector;
	transforms.push_back(x);
}

void Solver::solve() {
}

void Solver::showProjectionResult(std::string attr) {
	for (int i = 0; i < images.size(); ++i) {
		Eigen::Vector3d R_vector;
		R_vector << transforms[i](0), transforms[i](1), transforms[i](2);
		Eigen::Matrix3d R = rotate_vector2matrix(R_vector);

		Eigen::Vector3d T;
		T << transforms[i](3), transforms[i](4), transforms[i](5);

		double N = transforms[i](6);

		std::stringstream sstream;
		sstream << attr << "_" << i << ".png";
		cv::imwrite(sstream.str(), Projection::perspective(voxels, images[i], R, T, N));
	}
}

Eigen::Matrix3d Solver::rotate_vector2matrix(Eigen::Vector3d &v) {
	double x = v(0), y = v(1), z = v(2);
	Eigen::Matrix3d Rx, Ry, Rz;
	Rx << 1.0, 0.0, 0.0, 0.0, cos(x), -sin(x), 0.0, sin(x), cos(x);
	Ry << cos(y), 0.0, sin(y), 0.0, 1.0, 0.0, -sin(y), 0.0, cos(y);
	Rz << cos(z), -sin(z), 0.0, sin(z), cos(z), 0.0, 0.0, 0.0, 1.0;
	return Rx * Ry * Rz;
}
