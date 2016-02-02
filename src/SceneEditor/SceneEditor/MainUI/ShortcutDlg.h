#pragma once

#include <QDialog>
#include "ui_ShortcutDlg.h"

class ShortcutDlg : public QDialog
{
Q_OBJECT
public:
	ShortcutDlg(QWidget* parent);
	~ShortcutDlg();
private slots:
	void SelectItem(QTreeWidgetItem* item,int col);
	void Exit();
private:
	void Init();
	void BindSignal();
	void SetupAction();
private:
	Ui::ShortcutDlg ui;
};
