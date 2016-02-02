#include "PreHeader.h"
#include "FolderDelDlg.h"
#include "Macro.h"
#include "QFileOp.h"
#include <QFileInfo>
#include <assert.h>

void FolderDelThread::run(){
	int fNum = QFileOp::DirFileNum(mDir);
	emit Begin(fNum);
	RmFolder(mDir);
	emit End();
}
bool FolderDelThread::RmFolder(const QDir& dir){
	if(!dir.exists()) return true;
	
	QFileInfoList fileList = dir.entryInfoList();
	foreach(QFileInfo info,fileList)
	{
		QString fileName = info.fileName();
		if(fileName == "." || fileName==".."){
			//暂时先不处理
		}else if(info.isFile() || info.isSymLink()){
			QFile file(info.filePath());
			FAIL_RET( file.setPermissions(QFile::WriteOwner) );
			FAIL_RET( file.remove() );
			emit Increase();
			emit Name(fileName);
		}else if(info.isDir()){	
			FAIL_RET(RmFolder(info.filePath()));	
		}else{
			//不存在其他类型的情况
			assert(false); 
			return false;
		}
	}
	FAIL_RET(dir.rmdir("."));
	return true;
}


FolderDelDlg::FolderDelDlg(const QString& dir)
: mThread(dir)
{
	ui.setupUi(this);
	ui.hint->setText(QStringLiteral("正在删除文件："));
	setWindowTitle(QStringLiteral("删除文件夹"));
	BindSignal();
}
void FolderDelDlg::Run(){
	mThread.start();
	exec();
}
void FolderDelDlg::Increase(){
	ui.progress->setValue(ui.progress->value()+1);
}
void FolderDelDlg::BindSignal(){
	bool res = false;
	res = connect(&mThread,SIGNAL(Begin(int)),
		ui.progress,SLOT(setMaximum(int))); assert(res);
	res = connect(&mThread,SIGNAL(Increase()),SLOT(Increase())); assert(res);
	res = connect(&mThread,SIGNAL(End()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(Cancel())); assert(res);
	res = connect(&mThread,SIGNAL(Name(const QString&)),
		ui.fileName,SLOT(setText(const QString&))); assert(res);

}

