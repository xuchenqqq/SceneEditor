#pragma once

#include <QDialog>
#include "ui_KeySetDlg.h"

class QAction;

class KeySetDlg : public QDialog
{
Q_OBJECT
public:
	KeySetDlg(QWidget* parent,QAction* pAct);
	~KeySetDlg();
private slots:
	void Save();
private:
	void BindSignal();
	void Init();
	void SetupKeyTable();
private:
	Ui::KeySetDlg ui;
	QAction* mpAct;
};