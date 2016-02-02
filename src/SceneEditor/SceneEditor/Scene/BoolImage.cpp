#include "PreHeader.h"
#include "BoolImage.h"

BoolImage::BoolImage()
: mSize(32,32)
{}
BoolImage::~BoolImage(){
}
void BoolImage::SetImage(bool val,const QPixmap& pixmap){
	mPixmaps[val] = pixmap.scaled(mSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
}
void BoolImage::Paint(QPainter* painter,const QRect& rect,bool val) const{
	painter->drawPixmap(QRect(rect.topLeft(),mSize),mPixmaps[val]);
}