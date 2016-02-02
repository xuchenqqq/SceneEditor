#ifndef FOLDER_DEL_DLG_H
#define FOLDER_DEL_DLG_H

#include <QThread>
#include <QDialog>
#include <QDir>
#include <QString>
#include "ui_FileOpDlg.h"

class FolderDelThread : public QThread
{
Q_OBJECT
public:
	FolderDelThread(const QString& dir)
	: mDir(dir)
	, mbCancel(false)
	{}
	void Cancel(){mbCancel = true;}
signals:
	void Begin(int);
	void Increase();
	void Name(const QString& name);
	void End();
protected:
	void run();
private:
	bool RmFolder(const QDir& dir);
private:
	QDir mDir;
	bool mbCancel;
};

class FolderDelDlg : public QDialog
{
Q_OBJECT
public:
	FolderDelDlg(const QString& dir);
	void Run();
private slots:
	void Cancel(){ mThread.Cancel(); }
	void Increase();
private:
	void BindSignal();
private:
	Ui::FileOpDlg ui;
	FolderDelThread mThread;
};

#endif