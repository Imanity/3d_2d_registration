#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

#include "KDTree.h"

class __declspec(dllexport) ImageData {
public:
	ImageData();
	~ImageData();

	void readFromDSAFile(std::string file_path);

	int at(int n, int x, int y);
	int at(int n, Eigen::Vector2i pos);

	double linear(int n, double x, double y);

	void set(int n, int x, int y, int new_data);

	void show(int n);

	void binaryzation();

	void setMainID(int);

public:
	int n = 0;
	std::vector<cv::Mat> data;
	int nx, ny;
	float dx, dy;
	int distance_source_detector, distance_source_patient;
};
