#include "PreHeader.h"
#include "MapMng.h"
#include "AppModAlias.h"
#include "ActionMng.h"
#include "ScreenMng.h"
#include "NewMapDlg.h"
#include "Macro.h"

#include <QAction>
#include <assert.h>
#include <QMenu>
#include <QToolBar>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

MapMng::MapMng(QObject* parent)
: QObject(parent)
, mIsModify(false)
{
	Init();
	BindSignal();
}
MapMng::~MapMng(){

}
void MapMng::SetPath(const QString& val){
	mPath = val;
	mCurMap.clear();
	if(mPath.isEmpty()){
		PathInvalid();
	}else{
		FileInvalid();
	}
	NotifyState();
}
void MapMng::Modified(){
	mIsModify = true;
}
void MapMng::New(){
	NewMapDlg dlg(mPath);
	if(dlg.exec() == QDialog::Accepted){
		sScreenMng.New(dlg.ScreenSize(),dlg.ScreenNum());
		mCurMap = dlg.FileName();
		Save();
		FileValid();
		NotifyState();
	}
}
void MapMng::Load(){

}
void MapMng::Save(){
	
	
	mIsModify = false;
}
void MapMng::Close(){
	FAIL_RET_VOID(!mCurMap.isEmpty());
	if(mIsModify){
		int btn = QMessageBox::question(NULL,QStringLiteral("提示"),
		                                QStringLiteral("是否保存文件?"),
		                                QMessageBox::Yes,QMessageBox::No);
		if(btn==QMessageBox::Yes){
			Save();
		}
	}
	mCurMap.clear();
	sScreenMng.Clear();
	NotifyState();
}
void MapMng::NotifyState(){
	if(mPath.isEmpty() || mCurMap.isEmpty()){
		emit FileChanged(QString());
	}else{
		emit FileChanged(FilePath());
	}
	QString fileStr(mCurMap.isEmpty()?QStringLiteral("<空文件>"):mCurMap);
	emit StateChanged(QString(mIsModify?"*":"").append(fileStr));
}
void MapMng::Init(){
	InitAction();
	PathInvalid();
}
void MapMng::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("地图"));
	mpNew   = actMng.AddAction(QIcon(":/Icon/new.png"),QStringLiteral("新建"));
	mpSave  = actMng.AddAction(QIcon(":/Icon/save.png"),QStringLiteral("保存"));
	mpLoad  = actMng.AddAction(QIcon(":/Icon/open.png"),QStringLiteral("加载"));
	mpClose = actMng.AddAction(QIcon(":/Icon/close.png"),QStringLiteral("关闭"));
	actMng.EndPart();
	actMng.AddDefShourtcut([=](){
		mpNew->setShortcuts(QKeySequence::New);
		mpSave->setShortcuts(QKeySequence::Save);
		mpLoad->setShortcuts(QKeySequence::Open);
		mpClose->setShortcuts(QKeySequence::Close);
	});
}
void MapMng::BindSignal(){
	bool res = false;
	res = connect(mpNew,SIGNAL(triggered()),SLOT(New())); assert(res);
	res = connect(mpSave,SIGNAL(triggered()),SLOT(Save())); assert(res);
	res = connect(mpLoad,SIGNAL(triggered()),SLOT(Load())); assert(res);
	res = connect(mpClose,SIGNAL(triggered()),SLOT(Close())); assert(res);
}
void MapMng::PathInvalid(){
	mpNew->setEnabled(false);
	mpSave->setEnabled(false);
	mpLoad->setEnabled(false);
	mpClose->setEnabled(false);
}
void MapMng::FileValid(){
	mpNew->setEnabled(true);
	mpSave->setEnabled(true);
	mpLoad->setEnabled(true);
	mpClose->setEnabled(true);
}
void MapMng::FileInvalid(){
	mpNew->setEnabled(true);
	mpSave->setEnabled(false);
	mpLoad->setEnabled(true);
	mpClose->setEnabled(false);
}
QString MapMng::FilePath() const{
	return QDir(mPath).absoluteFilePath(mCurMap);
}