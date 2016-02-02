#pragma once

#include <QLineEdit>

class ImageLineEdit : public QLineEdit
{
Q_OBJECT
public:
	ImageLineEdit(QWidget* parent=0);
	~ImageLineEdit();
signals:
	void ImageDroped();
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
};