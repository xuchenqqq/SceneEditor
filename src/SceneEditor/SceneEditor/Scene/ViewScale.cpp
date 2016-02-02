#include "PreHeader.h"
#include "ViewScale.h"
#include "qwt_scale_engine.h"
#include <QStyleOption>

ViewScale::ViewScale(QwtScaleDraw::Alignment alignment, QWidget *parent)\
: QwtScaleWidget(alignment,parent)
, mAxisMin(0)
, mAxisMax(100)
, mMinCursor(1)
, mMidCursor(2)
, mMaxCursor(3)
, mpScaleEngine(new QwtLinearScaleEngine)
{}
ViewScale::~ViewScale(){
	delete mpScaleEngine;
}
void ViewScale::SetAxisScale(double min,double max){
	mAxisMin = min;
	mAxisMax = max;
	setScaleDiv(mpScaleEngine->divideScale(min,max,10,5));
}
void ViewScale::SetCursor(double min,double max){
	mMinCursor = min;
	mMaxCursor = max;
	mMidCursor = (min+max)/2.0;
	update();
}
void ViewScale::paintEvent(QPaintEvent* event){
    QPainter painter( this );
    painter.setClipRegion( event->region() );

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    draw( &painter );
	DrawCursors(&painter);
}
void ViewScale::DrawCursor(QPainter* painter,double value,const QColor& color){
	QRect r = contentsRect();
	Qt::Orientation orientation = scaleDraw()->orientation();
	double axisLen = mAxisMax-mAxisMin;
	if(value-mAxisMin>0 && value <mAxisMax){
		if(orientation == Qt::Horizontal){
			int x = r.width()/axisLen * (value-mAxisMin) + r.left();
			painter->setPen(color);
			painter->drawLine(x,r.top(),x,r.bottom());
		}else{
			int y = r.height()/axisLen * (mAxisMax-value) + r.top();
			painter->setPen(color);
			painter->drawLine(r.left(),y,r.right(),y);
		}
	}
}
void ViewScale::DrawCursors(QPainter* painter){
	DrawCursor(painter,mMinCursor,Qt::darkGreen);
	DrawCursor(painter,mMaxCursor,Qt::darkGreen);
	DrawCursor(painter,mMidCursor,Qt::darkRed);
}


