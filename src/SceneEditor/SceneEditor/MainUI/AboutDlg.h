#pragma once

#include <QDialog>
#include "ui_AboutDlg.h"

class AboutDlg : public QDialog
{
Q_OBJECT
public:
	AboutDlg();
	~AboutDlg();
private:
	void BindSignal();
	void Init();
private:
	Ui::AboutDlg ui;
};

