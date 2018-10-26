#include "Solver.h"

void TransformMatrix::getMatrix() {
	Eigen::Matrix3d Rx, Ry, Rz;
	Rx << 1.0, 0.0, 0.0, 0.0, cos(yaw), -sin(yaw), 0.0, sin(yaw), cos(yaw);
	Ry << cos(pitch), 0.0, sin(pitch), 0.0, 1.0, 0.0, -sin(pitch), 0.0, cos(pitch);
	Rz << cos(roll), -sin(roll), 0.0, sin(roll), cos(roll), 0.0, 0.0, 0.0, 1.0;
	M = Rx * Ry * Rz;
}

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
	transforms.clear();
	for (int i = 0; i < images.size(); ++i) {
		double yaw = initial_value[i](0), pitch = initial_value[i](1), roll = initial_value[i](2);
		TransformMatrix m(yaw, pitch, roll, 0, 0);
		std::vector<Eigen::Vector2d> v_transformed = transformVolume(volume, m);
		double x_min = DBL_MAX, y_min = DBL_MAX;
		for (int j = 0; j < v_transformed.size(); ++j) {
			if (v_transformed[j](0) < x_min)
				x_min = v_transformed[j](0);
			if (v_transformed[j](1) < y_min)
				y_min = v_transformed[j](1);
		}
		double tx = -x_min, ty = -y_min;
		transforms.push_back(TransformMatrix(yaw, pitch, roll, tx, ty));
	}
	show2d();
}

void Solver::show2d() {
	for (int i = 0; i < images.size(); ++i) {
		cv::Mat img = images[i]->data.clone();
		std::vector<Eigen::Vector2d> v_transformed = transformVolume(volume, transforms[i]);
		for (int j = 0; j < v_transformed.size(); ++j) {
			double x_pos = v_transformed[j](0) / images[i]->dx;
			double y_pos = v_transformed[j](1) / images[i]->dy;
			if (x_pos < 0 || x_pos >= img.cols)
				continue;
			if (y_pos < 0 || y_pos >= img.rows)
				continue;
			img.at<uchar>(y_pos, x_pos) = 0;
		}
		char str[5];
		_itoa_s(i, str, 10);
		cv::imshow(str, img);
	}
	cv::waitKey();
}

std::vector<Eigen::Vector2d> Solver::transformVolume(VolumeData * v, TransformMatrix &m) {
	std::vector<Eigen::Vector2d> pt_s;
	for (int i = 0; i < v->nvox; ++i) {
		if (!v->data[i])
			continue;
		Eigen::Vector3i X_ = v->coord(i);
		Eigen::Vector3d X;
		X << v->dx * X_(0), v->dy * X_(1), v->dz * X_(2);
		Eigen::Vector2d Y = transformPoint(X, m);
		pt_s.push_back(Y);
	}
	return pt_s;
}

Eigen::Vector2d Solver::transformPoint(Eigen::Vector3d &p, TransformMatrix &m) {
	Eigen::Vector3d Y_ = m.M * p;
	Eigen::Vector2d Y;
	Y << Y_(0) + m.tx, Y_(1) + m.ty;
	return Y;
}
