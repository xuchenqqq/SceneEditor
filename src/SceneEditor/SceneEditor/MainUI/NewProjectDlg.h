#pragma once

#include <QDialog>
#include <QDir>
#include "ui_NewProjectDlg.h"

class NewProjectDlg : public QDialog
{
Q_OBJECT
public:
	NewProjectDlg(QWidget* parent);
	~NewProjectDlg();
	QString Dir() const;
	QString Name() const;
private slots:
	void SelectDir();
	void NameChanged(const QString& name);
private:
	void Init();
	void BindSignal();
	void CheckInputValid();
private:
	Ui::NewProjectDlg ui;
};

