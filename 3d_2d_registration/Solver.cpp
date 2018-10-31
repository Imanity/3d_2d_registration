#include "Solver.h"
#include "Projection.h"
#include "LBFGS.h"

using namespace LBFGSpp;

std::vector<Eigen::Vector3d> X;
std::vector<ImageData *> Y;

Solver::Solver() {
	X.clear();
	Y.clear();
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
			X.push_back(voxel);
		}
	}
}

void Solver::addImageData(ImageData *img, Eigen::Vector3d R_vector) {
	images.push_back(img);
	Y.push_back(img);
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
	LBFGSParam<double> param;
	param.max_iterations = 30;
	LBFGSSolver<double> solver(param);

	Eigen::VectorXd x(7 * transforms.size());
	for (int i = 0; i < transforms.size(); ++i)
		for (int j = 0; j < 7; ++j)
			x(7 * i + j) = transforms[i](j);

	double fx;
	int niter = solver.minimize(foo, x, fx);

	for (int i = 0; i < transforms.size(); ++i)
		for (int j = 0; j < 7; ++j)
			transforms[i](j) = x(7 * i + j);
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

Eigen::Matrix3d rotate_vector2matrix(Eigen::Vector3d &v) {
	double x = v(0), y = v(1), z = v(2);
	Eigen::Matrix3d Rx, Ry, Rz;
	Rx << 1.0, 0.0, 0.0, 0.0, cos(x), -sin(x), 0.0, sin(x), cos(x);
	Ry << cos(y), 0.0, sin(y), 0.0, 1.0, 0.0, -sin(y), 0.0, cos(y);
	Rz << cos(z), -sin(z), 0.0, sin(z), cos(z), 0.0, 0.0, 0.0, 1.0;
	return Rx * Ry * Rz;
}

double foo(const Eigen::VectorXd &x, Eigen::VectorXd &grad) {
	double loss = loss_func(x);
	double grad_delta[] = { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
	grad = Eigen::VectorXd::Zero(x.rows());
	for (int i = 0; i < x.rows(); ++i) {
		Eigen::VectorXd x_ = x;
		x_(i) += grad_delta[i % 7];
		grad(i) = (loss_func(x_) - loss) / grad_delta[i % 7];
	}
	return loss;
}

double loss_func(const Eigen::VectorXd &x) {
	int overlap_d = 10;

	clock_t start, stop;
	start = clock();

	std::vector<std::vector<Eigen::Vector2d>> ys;

	std::vector<std::vector<double>> loss;
	
	std::vector<std::vector<double>> lambda;

	for (int i = 0; i < Y.size(); ++i) {
		std::vector<Eigen::Vector2d> y;
		ys.push_back(y);
		std::vector<double> loss_image(X.size(), 0);
		loss.push_back(loss_image);
		std::vector<double> lambda_image(X.size(), 1.0 / Y.size());
		lambda.push_back(lambda_image);
	}

	for (int i = 0; i < Y.size(); ++i) {
		Eigen::Vector3d R_vector;
		R_vector << x(7 * i), x(7 * i + 1), x(7 * i + 2);
		Eigen::Matrix3d R = rotate_vector2matrix(R_vector);

		Eigen::Vector3d T;
		T << x(7 * i + 3), x(7 * i + 4), x(7 * i + 5);

		double N = x(7 * i + 6);

		ys[i] = Projection::perspective(X, R, T, N);

		for (int j = 0; j < ys[i].size(); ++j) {
			double x_proj = ys[i][j](0) / Y[i]->dx + Y[i]->nx / 2;
			double y_proj = ys[i][j](1) / Y[i]->dy + Y[i]->ny / 2;
			loss[i][j] = Y[i]->linear(x_proj, y_proj);
		}
	}

	for (int j = 0; j < X.size(); ++j) {
		double sum = 0;
		for (int i = 0; i < Y.size(); ++i)
			sum += lambda[i][j];
		if (sum <= 0) {
			for (int i = 0; i < Y.size(); ++i)
				lambda[i][j] = 1.0 / Y.size();
			continue;
		}
		for (int i = 0; i < Y.size(); ++i)
			lambda[i][j] = lambda[i][j] / sum;
	}

	double total_loss = 0;
	for (int i = 0; i < Y.size(); ++i) for (int j = 0; j < X.size(); ++j) {
		total_loss += lambda[i][j] * loss[i][j];
	}

	total_loss /= X.size();
	total_loss /= 255.0;

	stop = clock();
	/*std::cout << "x: ";
	for (int i = 0; i < x.rows(); ++i) {
		std::cout << x(i) << ", ";
	}
	std::cout << std::endl;*/
	std::cout << "Loss: " << total_loss << ", used " << stop - start << "ms." << std::endl;
	return total_loss;
}
