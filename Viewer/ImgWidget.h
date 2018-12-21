#pragma once

#include <QWheelEvent>
#include <QMouseEvent>
#include <QWidget>
#include "ui_ImgWidget.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>
#include <Eigen/Dense>

#include "../3d_2d_registration/ImageData.h"

enum IMAGE_MODE {
	ORIGIN_IMAGE,
	BIN_IMAGE,
	FUSE_IMAGE
};

class ImgWidget : public QWidget {
	Q_OBJECT

public:
	ImgWidget(QWidget *parent = Q_NULLPTR);
	~ImgWidget();

	void setImage(ImageData *);
	void setImageMode(IMAGE_MODE mode);
	void setFuseParams(std::vector<Eigen::Vector3d> &Xs, std::vector<Eigen::VectorXd> &transforms);
	void setImageId(int id);
	void updateView();
	void wheelEvent(QWheelEvent* event);

signals:
	void updateN(int n);

private:
	Ui::ImgWidget ui;
	IMAGE_MODE imageMode = ORIGIN_IMAGE;

	std::vector<Eigen::VectorXd> Fuse_transforms;
	std::vector<Eigen::Vector3d> Fuse_Xs;
	Eigen::Matrix3d Fuse_R;
	Eigen::Vector3d Fuse_T;
	double Fuse_N;

public:
	ImageData *data = NULL;
};
