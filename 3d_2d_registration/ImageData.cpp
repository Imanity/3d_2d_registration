#include "ImageData.h"
#include "../DSAReader/DSAReader.h"

ImageData::ImageData() {
}

ImageData::~ImageData() {
}

void ImageData::readFromDSAFile(std::string file_path, int n) {
	DsaData dsa(file_path);
	if (n >= dsa.frame_num)
		return;
	data = dsa.dsa[n];
	dx = dsa.pixel_spacing[0];
	dy = dsa.pixel_spacing[1];
	nx = data.cols;
	ny = data.rows;
	int left = 0, right = data.cols - 1, top = 0, bottom = data.rows - 1;
	while (left < data.cols) {
		bool is_blank = true;
		for (int i = 0; i < data.rows; ++i) {
			if (at(left, i) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		left++;
	}
	while (right >= 0) {
		bool is_blank = true;
		for (int i = 0; i < data.rows; ++i) {
			if (at(right, i) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		right--;
	}
	while (top < data.rows) {
		bool is_blank = true;
		for (int i = 0; i < data.cols; ++i) {
			if (at(i, top) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		top++;
	}
	while (bottom >= 0) {
		bool is_blank = true;
		for (int i = 0; i < data.cols; ++i) {
			if (at(i, bottom) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		bottom--;
	}
	data = data(cv::Rect(left, top, right - left, bottom - top));
	nx = data.cols;
	ny = data.rows;
	distance_source_detector = dsa.distance_source_detector;
	distance_source_patient = dsa.distance_source_patient;
}

int ImageData::at(int x, int y) {
	if (x < 0)
		x = 0;
	if (x >= nx)
		x = nx - 1;
	if (y < 0)
		y = 0;
	if (y >= ny)
		y = ny - 1;
	return data.at<uchar>(y, x);
}
int ImageData::at(Eigen::Vector2i pos) {
	return at(pos(0), pos(1));
}

double ImageData::linear(double x, double y) {
	int _x = floor(x), x_ = ceil(x);
	int _y = floor(y), y_ = ceil(y);
	double lx = x - _x, ly = y - _y;
	return lx * ly * at(x_, y_) + (1 - lx) * (1 - ly) * at(_x, _y) + lx * (1 - ly) * at(x_, _y) + ly * (1 - lx) * at(_x, y_);
}

void ImageData::set(int x, int y, int new_data) {
	data.at<uchar>(y, x) = new_data;
}

void ImageData::show() {
	cv::Mat data_transformed;
	if (dy >= dx)
		cv::resize(data, data_transformed, cv::Size(data.cols, dy / dx * data.rows));
	else
		cv::resize(data, data_transformed, cv::Size(dx / dy * data.cols, data.rows));
	cv::imshow("Image", data_transformed);
	cv::waitKey();
}

void ImageData::binaryzation() {
	for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j) {
		data.at<uchar>(j, i) = data.at<uchar>(j, i) < 100 ? 0 : 255;
	}
	cv::Mat data_filtered = data.clone();
	for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j) {
		int num = 0, sum = 0;
		for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y) {
			if (i + x < 0 || i + x >= nx || j + y < 0 || j + y >= ny)
				continue;
			sum++;
			if (data.at<uchar>(j + y, i + x))
				num++;
		}
		if (num >= sum / 2)
			data_filtered.at<uchar>(j, i) = 255;
		else
			data_filtered.at<uchar>(j, i) = 0;
	}
	for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j)
		data.at<uchar>(j, i) = data_filtered.at<uchar>(j, i);

	std::vector<Eigen::Vector2d> binary_points;
	for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j) {
		if (at(i, j)) {
			Eigen::Vector2d p;
			p << dx * i, dy * j;
			binary_points.push_back(p);
		}
	}
	kdtree = KDTree(binary_points);
}
