#include "PreHeader.h"
#include "MapDock.h"
#include "AppModAlias.h"
#include "ActionMng.h"
#include "ScreenMng.h"
#include "Macro.h"
#include "Setting.h"
#include "Path.h"

#include <QFileSystemModel>
#include <QAction>
#include <assert.h>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QMenu>
#include <QToolBar>
#include <QItemSelectionModel>

extern bool coutFormat;

MapDock::MapDock(QWidget* parent)
: QDockWidget(parent)
, mIsModify(false)
, mpModel(new QFileSystemModel(this))
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
MapDock::~MapDock(){
	
}
void MapDock::SetPath(const QString& val){
	Close();
	mPath = val;
	if(mPath.isEmpty()){
		ui.list->setModel(NULL);
	}else{
		ui.list->setModel(mpModel);
		ui.list->setRootIndex(mpModel->setRootPath(mPath));
		InitView();
	}
	mCurMap.clear();
	mSelMap.clear();
	NotifyMapChanged();
	UpdateAction();
	UpdateRecent();
}
void MapDock::Modified(){
	if(!mIsModify){
		mIsModify = true;
		NotifyMapStateChanged();
	}
}
void MapDock::Add(bool state){
	ui.addGroupBox->setVisible(state);
	if(state){
		ReadScreenInfo();
		ui.name->setText("");
		NameChanged("");
	}
}
void MapDock::New(){
	Close();
	WriteScreenInfo();
	QSize sz(ui.width->value(),ui.height->value());
	int num = ui.screenNum->value();
    int dir = ui.dir->value();
	mCurMap = FileName();
    sScreenMng.New(sz,num,dir);
	Save();
	NotifyMapChanged();
	UpdateAction();
	AddCurMaptoRecent();
}
void MapDock::Save(){
	QFile file(FilePath());
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(NULL,QStringLiteral("错误"),QStringLiteral("打开文件失败"));
		return;
	}
	sScreenMng.Save(file);
	mIsModify = false;
	NotifyMapStateChanged();
}
void MapDock::Load(const QString& file){
	QModelIndex idx = mpModel->index(QDir(mPath).absoluteFilePath(file));
	if(idx.isValid()){
		ui.list->setCurrentIndex(idx);
		Load();
	}
}
void MapDock::Load(){
	FAIL_RET_VOID(mCurMap!=mSelMap);
	Close();
	mCurMap = mSelMap;
	QFile file(FilePath());
	if(!file.open(QIODevice::ReadOnly)){
		QMessageBox::critical(NULL,QStringLiteral("错误"),QStringLiteral("打开文件失败"));
		mCurMap.clear();
		return;
	}
	if(!sScreenMng.Load(file)){
		mCurMap.clear();
	}
	NotifyMapChanged();
	UpdateAction();
	AddCurMaptoRecent();
}
void MapDock::Close(){
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
	NotifyMapChanged();
	UpdateAction();
}
void MapDock::Delete(){
	FAIL_RET_VOID(mCurMap!=mSelMap);
	mpModel->remove(ui.list->currentIndex());
	UpdateAction();
}
void MapDock::Select(const QModelIndex& idx,const QModelIndex&){
	if(idx.isValid()){
		mSelMap = mpModel->fileName(idx);
	}else{
		mSelMap.clear();
	}
	UpdateAction();
}
void MapDock::Search(const QString& val){
	QStringList filters;
	if(!val.isEmpty()){
		filters = val.split(' ');
	}
	mpModel->setNameFilters(filters);
}
void MapDock::NameChanged(const QString& val){
	//检测名字为空的情况
	if(val.isEmpty()){
		ui.hint->setText(QStringLiteral("名字不允许为空"));
		ui.create->setEnabled(false);
		return;
	}
	//检测重名情况
	QString fName(val);
    if(coutFormat){
        fName.append(".xml");
    }else{
        fName.append(".json");
    }
	if(mpModel->index(QDir(mPath).absoluteFilePath(fName)).isValid()){
		ui.hint->setText(QStringLiteral("不允许重名"));
		ui.create->setEnabled(false);
		return;
	}
	ui.hint->setText("");
	ui.create->setEnabled(true);
}
void MapDock::Init(){
	mpModel->setNameFilterDisables(false);
	ui.addGroupBox->setVisible(false);
	InitAction();
	UpdateAction();
}
void MapDock::InitView(){
	bool res = false;
	res = connect(ui.list->selectionModel(),&QItemSelectionModel::currentChanged,
	              this,&MapDock::Select); assert(res);
}
void MapDock::BindSignal(){
	bool res = false;
	res = connect(ui.load,SIGNAL(clicked()),SLOT(Load())); assert(res);
	res = connect(ui.del,SIGNAL(clicked()),SLOT(Delete())); assert(res);
	res = connect(ui.add,SIGNAL(clicked(bool)),SLOT(Add(bool))); assert(res);
	res = connect(ui.search,&QLineEdit::textChanged,
	              this,&MapDock::Search); assert(res);
	res = connect(ui.name,&QLineEdit::textChanged,
		          this,&MapDock::NameChanged); assert(res);
	res = connect(ui.create,SIGNAL(clicked()),SLOT(New())); assert(res);
	res = connect(mpSave,SIGNAL(triggered()),SLOT(Save())); assert(res);
	res = connect(mpClose,SIGNAL(triggered()),SLOT(Close())); assert(res);
}
void MapDock::NotifyMapChanged(){
	if(mPath.isEmpty() || mCurMap.isEmpty()){
		emit FileChanged(QString());
	}else{
		emit FileChanged(FilePath());
	}
	NotifyMapStateChanged();
}
void MapDock::NotifyMapStateChanged(){
	QString fileStr(mCurMap.isEmpty()?QStringLiteral("<空文件>"):mCurMap);
	QString prefix(QStringLiteral(""));
	prefix.append(mIsModify?"*":"");
	emit StateChanged(prefix.append(fileStr));
}
void MapDock::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("文件"),true,true);
	actMng.AddSeparator();
	mpSave = actMng.AddAction(QIcon(":/Icon/save.png"),QStringLiteral("保存"));
	mpClose = actMng.AddAction(QIcon(":/Icon/close.png"),QStringLiteral("关闭"));
	actMng.AddSeparator();
	actMng.EndPart();
	actMng.AddDefShourtcut([=](){
		mpSave->setShortcuts(QKeySequence::Save);
		mpClose->setShortcuts(QKeySequence::Close);
	});
	mpRecent = new QMenu(QStringLiteral("最近地图"));
	QMenu* pMenu = sActionMng.Menu(QStringLiteral("文件"));
	pMenu->addMenu(mpRecent);
}
void MapDock::UpdateAction(){
	if(mPath.isEmpty()){
		ui.load->setEnabled(false);
		ui.del->setEnabled(false);
		ui.add->setEnabled(false);
		mpSave->setEnabled(false);
		mpClose->setEnabled(false);
		mpRecent->setEnabled(false);
		return;
	}
	ui.add->setEnabled(true);
	mpRecent->setEnabled(true);
	bool bCurValid = !mCurMap.isEmpty();
	mpSave->setEnabled(bCurValid);
	mpClose->setEnabled(bCurValid);
	bool bSelValid = !(mSelMap.isEmpty() || mSelMap==mCurMap);
	ui.load->setEnabled(bSelValid);
	ui.del->setEnabled(bSelValid);
}
void MapDock::UpdateRecent(){
	mpRecent->clear();
	FAIL_RET_VOID(!mPath.isEmpty());
	bool res = false;
	foreach(const QString& map,sPath.RecentMap()){
		QAction* pAct = mpRecent->addAction(map);
		res = connect(pAct,&QAction::triggered,[=] () {
			Load(pAct->text());
		}); assert(res);
	}
}
QString MapDock::FilePath() const{
	return FilePath(mCurMap);
}
QString MapDock::FilePath(const QString& fileName) const{
	return QDir(mPath).absoluteFilePath(fileName);
}
QString MapDock::FileName() const{
    QString name = ui.name->text();
    if(coutFormat){
         name.append(".xml");
    }else{
        name.append(".json");
    }

	return name;
}
void MapDock::ReadScreenInfo(){
	QSettings& setting = sSetting;
	ui.width->setValue(setting.value("ScreenInfo/Width",1136).toInt());
	ui.height->setValue(setting.value("ScreenInfo/Height",640).toInt());
	ui.screenNum->setValue(setting.value("ScreenInfo/Number",1).toInt());
    ui.dir->setValue(setting.value("ScreenInfo/Dir",0).toInt());
}
void MapDock::WriteScreenInfo(){
	QSettings& setting = sSetting;
	setting.setValue("ScreenInfo/Width",ui.width->value());
	setting.setValue("ScreenInfo/Height",ui.height->value());
	setting.setValue("ScreenInfo/Number",ui.screenNum->value());
    setting.setValue("ScreenInfo/Dir",ui.dir->value());
}
void MapDock::AddCurMaptoRecent(){
	if(!mCurMap.isEmpty()){
		sPath.AddRecentMap(mCurMap);
		UpdateRecent();
	}
}


