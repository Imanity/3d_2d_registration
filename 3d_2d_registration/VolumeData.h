#pragma once

#include <vector>
#include <Eigen/Dense>

class __declspec(dllexport) VolumeData {
public:
	VolumeData();
	~VolumeData();

	void readFromVolumeFile(std::string file_path);

	int idx(Eigen::Vector3i v);
	int idx(int x, int y, int z);

	Eigen::Vector3i coord(int idx_);

	short at(int x, int y, int z);
	short at(Eigen::Vector3i pos);

	short trilinear(float x, float y, float z);

	void set(int x, int y, int z, short new_data);

	void erode(int r);

public:
	short* data;
	int nx, ny, nz;
	float dx, dy, dz;
	int nvox;
};
