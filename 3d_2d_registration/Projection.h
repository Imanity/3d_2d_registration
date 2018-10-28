#pragma once

#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <vector>

#include "ImageData.h"

class Projection {
public:
	Projection();
	~Projection();

	static Eigen::Vector2d perspective(Eigen::Vector3d &X, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N);
	static std::vector<Eigen::Vector2d> perspective(std::vector<Eigen::Vector3d> &Xs, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N);
	static cv::Mat perspective(std::vector<Eigen::Vector3d> &Xs, ImageData *img, Eigen::Matrix3d &R, Eigen::Vector3d &T, double N);
};
