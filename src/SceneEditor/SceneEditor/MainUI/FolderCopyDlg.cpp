#include "PreHeader.h"
#include "FolderCopyDlg.h"
#include <QFile>
#include <assert.h>
#include <QFileInfo>
#include "Macro.h"
#include "QtDebug"

int GetFolderFileNum(const QDir& dir){
	int sum = 0;
	QFileInfoList infoList = 
		dir.entryInfoList(QDir::Files|QDir::NoSymLinks|QDir::Dirs|QDir::NoDotAndDotDot); 

	foreach(QFileInfo info, infoList){
		if(info.isDir()){
			sum += GetFolderFileNum(info.filePath());
		}else{
			sum++;
		}
	}
	return sum;
}

void FolderCopyThread::run(){
	int fNum = GetFolderFileNum(mSrcDir);
	emit Begin(fNum);
	CopyFolder(mSrcDir,mDstDir);
	emit End();
}

bool FolderCopyThread::CopyFolder(const QDir& srcDir,const QDir& dstDir){
	FAIL_RET(srcDir.exists());
	FAIL_RET(dstDir.exists());
	dstDir.mkdir(srcDir.dirName());
	QDir to(dstDir.filePath(srcDir.dirName())); 
	
	QFileInfoList infoList = 
		srcDir.entryInfoList(QDir::Files|QDir::NoSymLinks|QDir::Dirs|QDir::NoDotAndDotDot); 

	foreach(QFileInfo info, infoList)
	{
		if(info.isDir()){
			FAIL_RET( CopyFolder(info.filePath(),to) );
		}else{
			//if(false && to.exists(info.fileName())){
			//	to.remove(info.fileName()); 
			//}
            FAIL_RET(QFile::copy(info.filePath(), to.filePath(info.fileName())));
            emit Name(info.fileName());
            emit Increase();
		}
		FAIL_RET(!mbCancel);
	}
	return true;
}


FolderCopyDlg::FolderCopyDlg(const QString& srcDir,const QString& dstDir)
: mThread(srcDir,dstDir)
{
	ui.setupUi(this);
	BindSignal();
}
void FolderCopyDlg::Run(){
	mThread.start();
    exec();
}
void FolderCopyDlg::BindSignal(){
	bool res = false;
	res = connect(&mThread,SIGNAL(Begin(int)),
		ui.progress,SLOT(setMaximum(int))); assert(res);
	res = connect(&mThread,SIGNAL(Increase()),SLOT(Increase())); assert(res);
	res = connect(&mThread,SIGNAL(End()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(Cancel())); assert(res);
	res = connect(&mThread,SIGNAL(Name(const QString&)),
		ui.fileName,SLOT(setText(const QString&))); assert(res);
}
void FolderCopyDlg::Increase(){
    if(ui.progress->value() <100)
    {
        ui.progress->setValue(ui.progress->value()+1);
    }
    else
    {
        ui.progress->setValue(100);
    }
}

