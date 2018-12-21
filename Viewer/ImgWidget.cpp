#include "ImgWidget.h"

#include "../DSAReader/DSAReader.h"
#include "../3d_2d_registration/Projection.h"
#include "../3d_2d_registration/Solver.h"

ImgWidget::ImgWidget(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	this->setStyleSheet("background-color: rgb(0, 0, 0)");
	ui.label->setAlignment(Qt::AlignHCenter);
}

ImgWidget::~ImgWidget() {
}

void ImgWidget::setImage(ImageData *img) {
	data = img;
}

void ImgWidget::setImageMode(IMAGE_MODE mode) {
	imageMode = mode;
}

void ImgWidget::setFuseParams(std::vector<Eigen::Vector3d> &Xs, std::vector<Eigen::VectorXd> &transforms) {
	Fuse_Xs = Xs;

	Fuse_transforms = transforms;
}

void ImgWidget::setImageId(int id) {
	if (Fuse_transforms.size() <= id)
		return;

	Eigen::Vector3d R_Vec;
	R_Vec << Fuse_transforms[id](0), Fuse_transforms[id](1), Fuse_transforms[id](2);
	Fuse_R = rotate_vector2matrix(R_Vec);

	Fuse_T(0) = Fuse_transforms[id](3);
	Fuse_T(1) = Fuse_transforms[id](4);
	Fuse_T(2) = Fuse_transforms[id](5);

	Fuse_N = Fuse_transforms[id](6);
}

void ImgWidget::updateView() {
	std::stringstream sstream;
	sstream << data->n << "/" << data->data.size();

	QImage image;
	cv::Mat img;
	switch (imageMode) {
	case ORIGIN_IMAGE:
		img = data->data_origin[data->n].clone();
		putText(img, sstream.str().c_str(), cvPoint(0.75 * img.cols, 0.1 * img.rows), CV_FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0));
		image = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_Grayscale8);
		break;

	case BIN_IMAGE:
		img = data->data[data->n].clone();
		putText(img, sstream.str().c_str(), cvPoint(0.75 * img.cols, 0.1 * img.rows), CV_FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0));
		image = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_Grayscale8);
		break;

	case FUSE_IMAGE:
		if (!Fuse_Xs.size() || !Fuse_transforms.size()) {
			img = data->data_origin[data->n].clone();
			break;
		}
		img = Projection::perspective(Fuse_Xs, data, Fuse_R, Fuse_T, Fuse_N);
		putText(img, sstream.str().c_str(), cvPoint(0.75 * img.cols, 0.1 * img.rows), CV_FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0));
		image = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_RGB888);
		break;
	}

	ui.label->setPixmap(QPixmap::fromImage(image).scaled(ui.label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImgWidget::wheelEvent(QWheelEvent* event) {
	if (data == NULL)
		return;
	if (!data->data.size())
		return;
	if (event->delta() > 0)
		data->n -= 1;
	else
		data->n += 1;
	if (data->n < 0)
		data->n = 0;
	if (data->n >= data->data.size())
		data->n = data->data.size() - 1;
	emit updateN(data->n);
	updateView();
}
