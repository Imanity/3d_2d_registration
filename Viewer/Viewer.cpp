#include <QFileDialog>

#include "Viewer.h"
#include "../3d_2d_registration/3d_2d_registration.h"
#include "../3d_2d_fuse/3d_2d_fuse.h"

Viewer::Viewer(QWidget *parent)	: QMainWindow(parent) {
	ui.setupUi(this);

	vtk_widget = new VtkWidget(this);
	ui.vtkLayout->addWidget(vtk_widget);

	img_widget = new ImgWidget(this);
	ui.imageLayout->addWidget(img_widget);

	connect(ui.volumeAdd, SIGNAL(clicked()), this, SLOT(addVolume()));
	connect(ui.imageAdd, SIGNAL(clicked()), this, SLOT(addImage()));

	connect(ui.volumeList, SIGNAL(itemSelectionChanged()), this, SLOT(updateVolumeSelected()));
	connect(ui.imageList, SIGNAL(itemSelectionChanged()), this, SLOT(updateImageSelected()));

	connect(img_widget, SIGNAL(updateN(int)), this, SLOT(updateImageN(int)));

	connect(ui.actionRegister, SIGNAL(triggered()), this, SLOT(registerVolumeImage()));
	connect(ui.actionFuse, SIGNAL(triggered()), this, SLOT(fuseVolumeImage()));
}

void Viewer::addVolume() {
	QString fileToOpen = QFileDialog::getOpenFileName(this, "Open", "", "VOL Files (*.vol)");
	if (!fileToOpen.size())
		return;
	VolumeData newData;
	newData.readFromVolumeFile(fileToOpen.toStdString());
	volumes.push_back(newData);
	ui.volumeList->addItem(QString::fromStdString(fileToOpen.toStdString().substr(fileToOpen.toStdString().find_last_of("/"))));
}

void Viewer::addImage() {
	QString fileToOpen = QFileDialog::getOpenFileName(this, "Open", "", "IMG Files (*.*)");
	if (!fileToOpen.size())
		return;
	ImageData newData;
	newData.readFromDSAFile(fileToOpen.toStdString());
	newData.binaryzation();
	images.push_back(newData);
	ui.imageList->addItem(QString::fromStdString(fileToOpen.toStdString().substr(fileToOpen.toStdString().find_last_of("/"))));
}

void Viewer::updateVolumeSelected() {
	vtk_widget->setVolume(&volumes[ui.volumeList->currentRow()]);
	vtk_widget->updateView();
}

void Viewer::updateImageSelected() {
	img_widget->setImage(&images[ui.imageList->currentRow()]);
	img_widget->updateView();
}

void Viewer::updateImageN(int n) {
	int min_image_n = INT_MAX;
	for (int i = 0; i < images.size(); ++i) {
		if (min_image_n > images[i].data.size())
			min_image_n = images[i].data.size();
	}
	if (n >= min_image_n)
		return;
	for (int i = 0; i < images.size(); ++i)
		images[i].n = n;
	vtk_widget->current_scalar_id = n;
	vtk_widget->updateView();
}

void Viewer::registerVolumeImage() {
	if (!volumes.size() || !images.size())
		return;
	std::vector<ImageData *> imgs;
	std::vector<Eigen::Vector3d> Rs;
	for (int i = 0; i < images.size(); ++i) {
		imgs.push_back(&images[i]);
		QString img_name = ui.imageList->item(i)->text();
		Eigen::Vector3d R;
		R << 0, 0, 0;
		if (img_name.contains("front")) {
			R(0) = 3.1416 / 2.0;
		} else if (img_name.contains("side")) {
			R(0) = 3.1416 / 2.0;
			R(1) = -3.1416 / 12.0;
			R(2) = -3.1416 / 2.0;
		}
		Rs.push_back(R);
	}
	RegistrationSolver solver(&volumes[ui.volumeList->currentRow()], imgs, Rs);
	std::vector<Eigen::VectorXd> transforms = solver.Solve();
	if (!transforms.size())
		return;

	QString fileToSave = QFileDialog::getSaveFileName(this, "Save", "", "TXT Files (*.txt)");
	ofstream outFile(fileToSave.toStdString());
	outFile << transforms.size() << " " << transforms[0].rows() << endl;
	for (int i = 0; i < transforms.size(); ++i) {
		for (int j = 0; j < transforms[i].rows(); ++j) {
			outFile << transforms[i](j) << " ";
		}
		outFile << endl;
	}
	outFile.close();
}

void Viewer::fuseVolumeImage() {
	std::vector<Eigen::VectorXd> transforms;
	QString fileToOpen = QFileDialog::getOpenFileName(this, "Open", "", "TXT Files (*.txt)");
	if (!fileToOpen.size())
		return;
	ifstream inFile(fileToOpen.toStdString());

	int i_max = 0, j_max = 0;
	inFile >> i_max >> j_max;
	for (int i = 0; i < i_max; ++i) {
		std::vector<double> xTmp;
		for (int j = 0; j < j_max; ++j) {
			double x_;
			inFile >> x_;
			xTmp.push_back(x_);
		}
		Eigen::Map<Eigen::ArrayXd> x(xTmp.data(), j_max);
		transforms.push_back(x);
	}
	inFile.close();

	int frame_range = INT_MAX;
	for (int i = 0; i < images.size(); ++i) {
		if (frame_range > images[i].data.size())
			frame_range = images[i].data.size();
	}

	std::vector<Eigen::Vector3d> points;
	for (int i = 0; i < vtk_widget->mesh->GetNumberOfPoints(); ++i) {
		double p[3];
		vtk_widget->mesh->GetPoint(i, p);
		Eigen::Vector3d P;
		P << p[0], p[1], p[2];
		points.push_back(P);
	}

	FusionSolver solver(volumes[ui.volumeList->currentRow()], transforms, images);

	std::vector<std::vector<int>> scalars;

	for (int i = 0; i < frame_range; ++i) {
		std::vector<int> scalars_;
		solver.solveFrame(i);
		for (int j = 0; j < points.size(); ++j) {
			scalars_.push_back(solver.scalar(points[j]) ? 0 : 1);
		}
		scalars.push_back(scalars_);
	}

	vtk_widget->updateScalars(scalars);

	vtk_widget->setRenderingMode(MESH_RENDERING_WITH_SCALARS);
}
