#include "VolumeData.h"

#include <fstream>

VolumeData::VolumeData() : data(nullptr) {
}

VolumeData::~VolumeData() {
}

void VolumeData::readFromVolumeFile(std::string file_path) {
	std::ifstream inFile(file_path, std::ios::in);
	inFile >> nx >> ny >> nz >> dx >> dy >> dz;
	nvox = nx * ny * nz;
	data = new short[nvox];
	std::string inputData;
	inFile >> inputData;
	for (int i = 0; i < nvox; ++i) {
		char lByte = inputData.at(2 * i);
		char hByte = inputData.at(2 * i + 1);
		data[i] = (hByte << 8) | lByte;
	}
	inFile.close();
}

int VolumeData::idx(Eigen::Vector3i v) {
	return idx(v(0), v(1), v(2));
}

int VolumeData::idx(int x, int y, int z) {
	int i = x + y * nx + z * nx * ny;
	if (x < 0 || x >= nx || y < 0 || y >= ny || z < 0 || z >= nz) {
		return -1;
	}
	return i;
}

Eigen::Vector3i VolumeData::coord(int idx_) {
	Eigen::Vector3i v;
	if (idx_ < 0 || idx_ >= nvox) {
		v << 0, 0, 0;
		return v;
	}
	int z = idx_ / (nx * ny);
	int tmp = idx_ % (nx * ny);
	int y = tmp / nx;
	int x = tmp % nx;
	v << x, y, z;
	return v;
}

short VolumeData::at(int x, int y, int z) {
	int i = idx(x, y, z);
	if (i < 0) return -1;
	return data[i];
}

short VolumeData::at(Eigen::Vector3i pos) {
	int i = idx(pos);
	if (i < 0) return -1;
	return data[i];
}

short VolumeData::trilinear(float x, float y, float z) {
	int x0 = int(x), x1 = x0 + 1;
	int y0 = int(y), y1 = y0 + 1;
	int z0 = int(z), z1 = z0 + 1;

	float xd = x - x0, yd = y - y0, zd = z - z0;
	float c00 = at(x0, y0, z0) * (1 - xd) + at(x1, y0, z0) * xd;
	float c01 = at(x0, y0, z1) * (1 - xd) + at(x1, y0, z1) * xd;
	float c10 = at(x0, y1, z0) * (1 - xd) + at(x1, y1, z0) * xd;
	float c11 = at(x0, y1, z1) * (1 - xd) + at(x1, y1, z1) * xd;

	float c0 = c00 * (1 - yd) + c10 * yd;
	float c1 = c01 * (1 - yd) + c11 * yd;

	float c = c0 * (1 - zd) + c1 * zd;
	return short(c + 0.5);
}

void VolumeData::set(int x, int y, int z, short new_data) {
	int i = idx(x, y, z);
	if (i < 0) return;
	this->data[i] = new_data;
}
