#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

#include "KDTree.h"

class ImageData {
public:
	ImageData();
	~ImageData();

	void readFromDSAFile(std::string file_path, int n);

	int at(int x, int y);
	int at(Eigen::Vector2i pos);

	double linear(double x, double y);

	void set(int x, int y, int new_data);

	void show();

	void binaryzation();

public:
	cv::Mat data;
	int nx, ny;
	float dx, dy;
	int distance_source_detector, distance_source_patient;
	KDTree kdtree;
};
