#pragma once

#include <QDialog>
#include "ui_SimConfigDlg.h"

class SimConfigDlg : public QDialog
{
Q_OBJECT
public:
	SimConfigDlg();
	~SimConfigDlg();
	QString CmdLine() const;
	void SetCmdLine(const QString& str);
private:
	void Init();
private:
	Ui::SimConfigDlg ui;
};
