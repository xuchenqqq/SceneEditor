#pragma once

#include "qwt_scale_widget.h"
#include <QPainter>

class ViewScale : public QwtScaleWidget
{
Q_OBJECT
public:
    ViewScale(QwtScaleDraw::Alignment, QWidget *parent);
	~ViewScale();
	void SetAxisScale(double min,double max);
	void SetCursor(double min,double max);
protected:
	void paintEvent(QPaintEvent* event);
private:
	void DrawCursor(QPainter* painter,double value,const QColor& color);
	void DrawCursors(QPainter* painter);
private:
	double mAxisMin;
	double mAxisMax;
	double mMinCursor;
	double mMidCursor;
	double mMaxCursor;
	class QwtScaleEngine* mpScaleEngine;
};
