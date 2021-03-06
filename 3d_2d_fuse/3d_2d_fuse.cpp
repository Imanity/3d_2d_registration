#define DLL_EXPORTS

#include <algorithm>

#include "3d_2d_fuse.h"
#include "DisjSets.h"

#include "../3d_2d_registration/Solver.h"
#include "../3d_2d_registration/Projection.h"

FusionSolver::FusionSolver(VolumeData &vol, std::vector<Eigen::VectorXd> &transforms, std::vector<ImageData> &imgs) {
	this->transforms = transforms;
	this->imagedatas = imgs;

	volume.nx = vol.nx / downsample_rate + 1;
	volume.ny = vol.ny / downsample_rate + 1;
	volume.nz = vol.nz / downsample_rate + 1;
	volume.dx = vol.dx * downsample_rate;
	volume.dy = vol.dy * downsample_rate;
	volume.dz = vol.dz * downsample_rate;
	volume.nvox = volume.nx * volume.ny * volume.nz;
	volume.data = new short[volume.nvox];
	for (int i = 0; i < volume.nvox; ++i) {
		volume.data[i] = -1;
	}
	for (int i = 0; i < vol.nvox; ++i) {
		if (vol.data[i]) {
			Eigen::Vector3i pos = vol.coord(i);
			volume.set(pos(0) / downsample_rate, pos(1) / downsample_rate, pos(2) / downsample_rate, 0);
		}
	}
}

FusionSolver::~FusionSolver() {
}

void FusionSolver::solveFrame(int frame) {
	std::cout << "Frame " << frame << std::endl;
	images.clear();
	for (int i = 0; i < imagedatas.size(); ++i) {
		cv::Mat img;
		cv::erode(imagedatas[i].data[frame], img, cv::Mat(), cv::Point(-1, -1), 2);
		images.push_back(img);
	}

	for (int j = 0; j < volume.nvox; ++j)
		if (volume.data[j] >= 0)
			volume.data[j] = 1;

	Register();
	Merge(0);
	Merge(1);
}

bool FusionSolver::scalar(Eigen::Vector3d &p) {
	return volume.at(p(0) / volume.dx, p(1) / volume.dy, p(2) / volume.dz) == 1;
}

void FusionSolver::Register() {
	for (int i = 0; i < images.size(); ++i) {
		Eigen::Vector3d R_vector;
		R_vector << transforms[i](0), transforms[i](1), transforms[i](2);
		Eigen::Matrix3d R = rotate_vector2matrix(R_vector);

		Eigen::Vector3d T;
		T << transforms[i](3), transforms[i](4), transforms[i](5);

		double N = transforms[i](6);

		for (int j = 0; j < volume.nvox; ++j) {
			if (volume.data[j] < 0)
				continue;
			Eigen::Vector3i X_pos = volume.coord(j);
			Eigen::Vector3d X;
			X << volume.dx * X_pos(0), volume.dy * X_pos(1), volume.dz * X_pos(2);
			Eigen::Vector2d Y = Projection::perspective(X, R, T, N);
			double x = Y(0) / imagedatas[i].dx + imagedatas[i].nx / 2;
			double y = Y(1) / imagedatas[i].dy + imagedatas[i].ny / 2;

			if ((int)x < 0 || (int)x >= imagedatas[i].nx) {
				volume.data[j] = 0;
				continue;
			}
			if ((int)y < 0 || (int)y >= imagedatas[i].ny) {
				volume.data[j] = 0;
				continue;
			}
			if (images[i].at<uchar>(y, x) == 255) {
				volume.data[j] = 0;
				continue;
			}
		}
	}
}

void FusionSolver::Merge(int color) {
	int min_cluster_size = 100;

	std::vector<int> disj_idx;
	std::vector<int> color_idx(volume.nvox, -1);
	int total_voxel_num = 0;
	for (int i = 0; i < volume.nvox; ++i) {
		if (volume.data[i] >= 0) {
			total_voxel_num++;
		}
		if (volume.data[i] == color) {
			color_idx[i] = disj_idx.size();
			disj_idx.push_back(i);
		}
	}

	if (disj_idx.size() > total_voxel_num - min_cluster_size) {
		return;
	}

	DisjSets disj_set(disj_idx.size());

	for (int i = 0; i < volume.nvox; ++i) {
		if (volume.data[i] != color)
			continue;
		Eigen::Vector3i c = volume.coord(i);
		int idx_neighbor[] = { volume.idx(c + Eigen::Vector3i(0, 0, 1)), volume.idx(c + Eigen::Vector3i(0, 1, 0)), volume.idx(c + Eigen::Vector3i(1, 0, 0)) };
		for (int j = 0; j < 3; ++j) {
			if (idx_neighbor[j] < 0 || idx_neighbor[j] >= volume.nvox)
				continue;
			if (volume.data[idx_neighbor[j]] != color)
				continue;
			disj_set.unite(color_idx[idx_neighbor[j]], color_idx[i]);
		}
	}

	int max_cluster_id = 0;
	std::vector<int> disj_cluster = disj_set.output_cluster();
	for (int i = 0; i < disj_cluster.size(); ++i) {
		if (disj_cluster[i] > max_cluster_id)
			max_cluster_id = disj_cluster[i];
	}

	for (int i = 0; i < max_cluster_id; ++i) {
		int cluster_size = 0;
		for (int j = 0; j < disj_cluster.size(); ++j)
			if (disj_cluster[j] == i)
				cluster_size++;
		if (cluster_size < min_cluster_size) {
			for (int j = 0; j < disj_cluster.size(); ++j)
				if (disj_cluster[j] == i)
					volume.data[disj_idx[j]] = 1 - color;
		}
	}
}
