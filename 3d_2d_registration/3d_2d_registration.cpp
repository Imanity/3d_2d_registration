#include <iostream>

#include "VolumeData.h"
#include "ImageData.h"
#include "Solver.h"

using namespace std;

int main() {
	VolumeData *volume = new VolumeData();
	volume->readFromVolumeFile("../Data/vessel.vol");

	ImageData *image_front = new ImageData();
	ImageData *image_side = new ImageData(); 
	image_front->readFromDSAFile("../Data/front", 8);
	image_side->readFromDSAFile("../Data/side", 8);
	std::cout << image_front->distance_source_detector << " " << image_front->distance_source_patient << std::endl;
	
	Eigen::Vector3d initial_front, initial_side;
	initial_front << 3.1416 / 2.0, 0, 0;
	initial_side << 3.1416 / 2.0, -3.1416 / 12.0, -3.1416 / 2.0;
	std::vector<Eigen::Vector3d> initial_value;
	initial_value.push_back(initial_front);
	initial_value.push_back(initial_side);

	Solver solver;

	solver.addVolumeData(volume);
	solver.addImageData(image_front);
	solver.addImageData(image_side);
	
	solver.solve(initial_value);

    return 0;
}
