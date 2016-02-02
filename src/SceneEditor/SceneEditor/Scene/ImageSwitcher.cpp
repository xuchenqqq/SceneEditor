#include "PreHeader.h"
#include "ImageSwitcher.h"
#include "BoolImage.h"

ImageSwitcher::ImageSwitcher(QWidget* parent,const BoolImage* pBoolImage)
: QWidget(parent)
, mState(false)
, mpBoolImage(pBoolImage)
{
	setAutoFillBackground(true);
}
ImageSwitcher::~ImageSwitcher(){
}
void ImageSwitcher::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	mpBoolImage->Paint(&painter,event->rect(),mState);
}
void ImageSwitcher::mousePressEvent(QMouseEvent *){
	mState = !mState;
	update();
	emit ValueChanged();
}
void ImageSwitcher::leaveEvent(QEvent *){
	emit EditFinished();
}
