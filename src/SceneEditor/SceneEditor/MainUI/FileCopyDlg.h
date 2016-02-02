#ifndef FILE_COPY_DLG_H
#define FILE_COPY_DLG_H

#include <QDialog>
#include "ui_FileOpDlg.h"
#include <QString>
#include <QThread>
#include <QDir>

class FileCopyThread : public QThread
{
Q_OBJECT
public:
	FileCopyThread(const QStringList& fileList,const QString& dstDir)
	: mFList(fileList)
	, mDir(dstDir)
	, mbCancel(false)
	{}
	void Cancel(){mbCancel = true;}
signals:
	void Begin(int);
	void Value(int);
	void Name(const QString& name);
	void End();
protected:
	void run();
private:
	QStringList mFList;
    QDir mDir;
	bool mbCancel;
};

class FileCopyDlg : public QDialog
{
Q_OBJECT
public:
	FileCopyDlg(const QStringList& fileList,const QString& dstDir);
	void Run();
private slots:
	void Cancel(){ mThread.Cancel(); }
private:
	void BindSignal();
private:
	Ui::FileOpDlg ui;
	FileCopyThread mThread;
};


#endif
