#include "ImageData.h"
#include "../DSAReader/DSAReader.h"

ImageData::ImageData() {
}

ImageData::~ImageData() {
}

void ImageData::readFromDSAFile(std::string file_path) {
	DsaData dsa(file_path);
	data = dsa.dsa;
	dx = dsa.pixel_spacing[0];
	dy = dsa.pixel_spacing[1];
	nx = data[0].cols;
	ny = data[0].rows;
	int left = 0, right = data[0].cols - 1, top = 0, bottom = data[0].rows - 1;
	while (left < data[0].cols) {
		bool is_blank = true;
		for (int i = 0; i < data[0].rows; ++i) {
			if (at(0, left, i) != 0) {
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
		for (int i = 0; i < data[0].rows; ++i) {
			if (at(0, right, i) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		right--;
	}
	while (top < data[0].rows) {
		bool is_blank = true;
		for (int i = 0; i < data[0].cols; ++i) {
			if (at(0, i, top) != 0) {
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
		for (int i = 0; i < data[0].cols; ++i) {
			if (at(0, i, bottom) != 0) {
				is_blank = false;
				break;
			}
		}
		if (!is_blank) {
			break;
		}
		bottom--;
	}
	for (int i = 0; i < data.size(); ++i)
		data[i] = data[i](cv::Rect(left, top, right - left, bottom - top));
	nx = data[0].cols;
	ny = data[0].rows;
	distance_source_detector = dsa.distance_source_detector;
	distance_source_patient = dsa.distance_source_patient;
}

int ImageData::at(int n, int x, int y) {
	if (n < 0 || n >= data.size())
		return -1;
	if (x < 0)
		x = 0;
	if (x >= nx)
		x = nx - 1;
	if (y < 0)
		y = 0;
	if (y >= ny)
		y = ny - 1;
	return data[n].at<uchar>(y, x);
}
int ImageData::at(int n, Eigen::Vector2i pos) {
	return at(n, pos(0), pos(1));
}

double ImageData::linear(int n, double x, double y) {
	int _x = floor(x), x_ = ceil(x);
	int _y = floor(y), y_ = ceil(y);
	double lx = x - _x, ly = y - _y;
	return lx * ly * at(n, x_, y_) + (1 - lx) * (1 - ly) * at(n, _x, _y) + lx * (1 - ly) * at(n, x_, _y) + ly * (1 - lx) * at(n, _x, y_);
}

void ImageData::set(int n, int x, int y, int new_data) {
	data[n].at<uchar>(y, x) = new_data;
}

void ImageData::show(int n) {
	cv::Mat data_transformed;
	if (dy >= dx)
		cv::resize(data[n], data_transformed, cv::Size(data[n].cols, dy / dx * data[n].rows));
	else
		cv::resize(data[n], data_transformed, cv::Size(dx / dy * data[n].cols, data[n].rows));
	cv::imshow("Image", data_transformed);
	cv::waitKey();
}

void ImageData::binaryzation() {
	for (int n = 0; n < data.size(); ++n) {
		for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j) {
			data[n].at<uchar>(j, i) = data[n].at<uchar>(j, i) < 100 ? 0 : 255;
		}
		cv::Mat data_filtered = data[n].clone();
		for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j) {
			int num = 0, sum = 0;
			for (int x = -1; x <= 1; ++x) for (int y = -1; y <= 1; ++y) {
				if (i + x < 0 || i + x >= nx || j + y < 0 || j + y >= ny)
					continue;
				sum++;
				if (data[n].at<uchar>(j + y, i + x))
					num++;
			}
			if (num >= sum / 2)
				data_filtered.at<uchar>(j, i) = 255;
			else
				data_filtered.at<uchar>(j, i) = 0;
		}
		for (int i = 0; i < nx; ++i) for (int j = 0; j < ny; ++j)
			data[n].at<uchar>(j, i) = data_filtered.at<uchar>(j, i);
	}
}

void ImageData::setMainID(int n_) {
	n = n_;
}
