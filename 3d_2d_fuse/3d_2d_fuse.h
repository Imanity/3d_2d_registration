#pragma once

#ifdef DLL_EXPORTS
#define FUSION_EXPORT __declspec(dllexport)
#else
#define FUSION_EXPORT __declspec(dllimport)
#endif

#include <vector>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

#include "../3d_2d_registration/VolumeData.h"
#include "../3d_2d_registration/ImageData.h"

class FUSION_EXPORT FusionSolver {
public:
	FusionSolver(VolumeData &vol, std::vector<Eigen::VectorXd> &transforms, std::vector<ImageData> &imgs);
	~FusionSolver();

	void solveFrame(int frame);

	bool scalar(Eigen::Vector3d &p);

private:
	VolumeData volume;
	std::vector<ImageData> imagedatas;
	std::vector<cv::Mat> images;
	std::vector<Eigen::VectorXd> transforms;
	int downsample_rate = 3;

private:
	void Register();
	void Merge(int color);
};
