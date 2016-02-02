#ifndef FOLDER_COPY_DLG_H
#define FOLDER_COPY_DLG_H

#include <QThread>
#include <QDialog>
#include <QDir>
#include <QString>
#include "ui_FileOpDlg.h"

class FolderCopyThread : public QThread
{
Q_OBJECT
public:
	FolderCopyThread(const QString& srcDir,const QString& dstDir)
	: mSrcDir(srcDir)
	, mDstDir(dstDir)
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
	bool CopyFolder(const QDir& srcDir,const QDir& dstDir);
private:
	QDir mSrcDir;
	QDir mDstDir;
	bool mbCancel;
};


class FolderCopyDlg : public QDialog
{
Q_OBJECT
public:
	FolderCopyDlg(const QString& srcDir,const QString& dstDir);
	~FolderCopyDlg(){}
	void Run();
private slots:
	void Cancel(){ mThread.Cancel(); }
	void Increase();
private:
	void BindSignal();
private:
	Ui::FileOpDlg ui;
	FolderCopyThread mThread;
};

#endif