#include "PreHeader.h"
#include "FileCopyDlg.h"
#include <QFile>
#include <assert.h>
#include <QString>

void FileCopyThread::run(){
	emit Begin(mFList.size());
	QFile file;
	for(int i = 0; i < mFList.size(); ++i){
		QFileInfo info(mFList[i]);
		file.setFileName(mFList[i]);
		emit Name(info.fileName());
		file.copy(mDir.filePath(info.fileName()));
		emit Value(i+1);
		if(mbCancel) break;
	}
	emit End();
}

FileCopyDlg::FileCopyDlg(const QStringList& fileList,const QString& dstDir)
: mThread(fileList,dstDir){
		ui.setupUi(this);
		BindSignal();
}
void FileCopyDlg::Run(){
	mThread.start();
	exec();
}
void FileCopyDlg::BindSignal(){
	bool res = false;
	res = connect(&mThread,SIGNAL(Begin(int)),
		ui.progress,SLOT(setMaximum(int))); assert(res);
	res = connect(&mThread,SIGNAL(Value(int)),
		ui.progress,SLOT(setValue(int))); assert(res);
	res = connect(&mThread,SIGNAL(End()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(Cancel())); assert(res);
	res = connect(&mThread,SIGNAL(Name(const QString&)),
		ui.fileName,SLOT(setText(const QString&))); assert(res);
}


