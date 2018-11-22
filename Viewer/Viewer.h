#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Viewer.h"

#include "VtkWidget.h"
#include "ImgWidget.h"

class Viewer : public QMainWindow {
	Q_OBJECT

public:
	Viewer(QWidget *parent = Q_NULLPTR);

public slots:
	void addVolume();
	void addImage();

	void updateVolumeSelected();
	void updateImageSelected();

	void registerVolumeImage();
	void fuseVolumeImage();

private:
	Ui::ViewerClass ui;

	VtkWidget *vtk_widget;
	ImgWidget *img_widget;

	std::vector<VolumeData> volumes;
	std::vector<ImageData> images;
};
