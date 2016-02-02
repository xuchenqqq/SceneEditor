#pragma once

#include <QWidget>

class BoolImage;

class ImageSwitcher : public QWidget
{
Q_OBJECT
public:
	ImageSwitcher(QWidget* parent,const BoolImage* pBoolImage);
	~ImageSwitcher();
	void SetValue(bool state){mState = state;}
	bool Value() const{return mState;}
signals:
	void ValueChanged();
	void EditFinished();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
private:
	bool mState;
	const BoolImage* mpBoolImage; 
};