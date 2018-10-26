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
}

int ImageData::at(int x, int y) {
	return data.at<uchar>(y, x);
}
int ImageData::at(Eigen::Vector2i pos) {
	return data.at<uchar>(pos(1), pos(0));
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
