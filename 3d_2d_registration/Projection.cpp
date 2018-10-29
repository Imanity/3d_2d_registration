#include "Projection.h"

Projection::Projection() {
}

Projection::~Projection() {
}

Eigen::Vector2d Projection::perspective(Eigen::Vector3d &X, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N) {
	Eigen::Vector3d X0 = R * X + T;
	Eigen::Vector2d Y;
	Y << X0(0) * N / X0(2), X0(1) * N / X0(2);
	return Y;
}

std::vector<Eigen::Vector2d> Projection::perspective(std::vector<Eigen::Vector3d> &Xs, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N) {
	std::vector<Eigen::Vector2d> Ys;
	for (int i = 0; i < Xs.size(); ++i)
		Ys.push_back(perspective(Xs[i], R, T, N));
	return Ys;
}

cv::Mat Projection::perspective(std::vector<Eigen::Vector3d> &Xs, ImageData *img, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N) {
	cv::Mat fuse(img->ny, img->nx, CV_8UC3, cv::Scalar::all(0));
	for (int i = 0; i < img->ny; ++i) for (int j = 0; j < img->nx; ++j) {
		fuse.at<cv::Vec3b>(i, j)(0) = fuse.at<cv::Vec3b>(i, j)(1) = fuse.at<cv::Vec3b>(i, j)(2) = img->at(j, i);
	}
	std::vector<Eigen::Vector2d> Ys = perspective(Xs, R, T, N);
	for (int i = 0; i < Ys.size(); ++i) {
		double x = Ys[i](0) / img->dx + img->nx / 2;
		double y = Ys[i](1) / img->dy + img->ny / 2;
		if (x < 0 || x >= img->nx)
			continue;
		if (y < 0 || y >= img->ny)
			continue;
		fuse.at<cv::Vec3b>(y, x)(0) = 255;
	}
	return fuse;
}
