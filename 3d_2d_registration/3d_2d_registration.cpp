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
	
	Eigen::Vector3d R_front, R_side;
	R_front << 3.1416 / 2.0, 0, 0;
	R_side << 3.1416 / 2.0, -3.1416 / 12.0, -3.1416 / 2.0;

	Solver solver;

	solver.addVolumeData(volume);
	solver.addImageData(image_front, R_front);
	solver.addImageData(image_side, R_side);
	
	solver.solve();

	solver.showProjectionResult("../Output/initial");

    return 0;
}
