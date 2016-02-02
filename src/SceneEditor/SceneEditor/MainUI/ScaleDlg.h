#ifndef SCALE_DLG_H
#define SCALE_DLG_H

#include <QDialog>
#include "ui_ScaleDlg.h"

class ScaleDlg : public QDialog
{
Q_OBJECT
public:
	ScaleDlg();
	~ScaleDlg();
	qreal Scale() const;
private slots:
	void SliderChanged(int val);
	void SpinBoxChanged(int val);
private:
	void BindSignal();
private:
	Ui::ScaleDlg ui;
};


#endif
