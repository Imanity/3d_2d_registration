#pragma once

#ifdef DLL_EXPORTS
#define DSA_READER_EXPORT __declspec(dllexport)
#else
#define DSA_READER_EXPORT __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class DSA_READER_EXPORT DsaData {
public:
	DsaData(std::string dsa_path);
	~DsaData();

	std::vector<cv::Mat> LoadDcmDataSet(std::string file_path);

public:
	std::vector<cv::Mat> dsa;
	int frame_num;
	float pixel_spacing[2];
};
