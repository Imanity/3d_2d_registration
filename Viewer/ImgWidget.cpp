#include "ImgWidget.h"

#include "../DSAReader/DSAReader.h"

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

void ImgWidget::updateView() {
	QImage image;
	cv::Mat img = data->data[data->n].clone();
	std::stringstream sstream;
	sstream << data->n << "/" << data->data.size();
	putText(img, sstream.str().c_str(), cvPoint(0.75 * img.cols, 0.1 * img.rows), CV_FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 0));
	image = QImage((const unsigned char *)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_Grayscale8);
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
	updateView();
}
