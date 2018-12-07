#pragma once

#include <QWheelEvent>
#include <QMouseEvent>
#include <QWidget>
#include "ui_ImgWidget.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

#include "../3d_2d_registration/ImageData.h"

class ImgWidget : public QWidget {
	Q_OBJECT

public:
	ImgWidget(QWidget *parent = Q_NULLPTR);
	~ImgWidget();

	void setImage(ImageData *);
	void updateView();
	void wheelEvent(QWheelEvent* event);

signals:
	void updateN(int n);

private:
	Ui::ImgWidget ui;

public:
	ImageData *data = NULL;
};
