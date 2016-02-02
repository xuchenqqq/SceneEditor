#pragma once

#include <QPixmap>
#include <QSize>

class BoolImage
{
public:
	BoolImage();
	~BoolImage();
	void SetImage(bool val,const QPixmap& pixmap);
	void Paint(QPainter* painter,const QRect& rect,bool val) const;
private:
	QPixmap mPixmaps[2];
	QSize mSize;
};