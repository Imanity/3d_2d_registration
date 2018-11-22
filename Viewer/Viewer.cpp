#include <QFileDialog>

#include "Viewer.h"
#include "../3d_2d_registration/3d_2d_registration.h"

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
	solver.Solve();
}

void Viewer::fuseVolumeImage() {
	//
}
