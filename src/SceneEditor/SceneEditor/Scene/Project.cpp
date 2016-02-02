#include "PreHeader.h"
#include "Project.h"
#include "AppModAlias.h"
#include "ActionMng.h"
#include "Macro.h"
#include "NewProjectDlg.h"
#include "JlCompress.h"
#include "Path.h"
#include "Cache.h"
#include "UIAccess.h"

#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QDateTime>

Project::Project(QObject* parent)
	: QObject(parent)
{
	Init();
	BindSignal();
}
Project::~Project(){
	delete mpPath;
	delete mpCache;
}
void Project::New(){
	NewProjectDlg dlg(sUiAccess.Ptr("main"));
	if(dlg.exec() == QDialog::Accepted){
		mpPath->NewPath(dlg.Dir(),dlg.Name());
		UpdateState();
        emit(OpenProject(true));
	}
}
void Project::Load(){
	QString path = QFileDialog::getExistingDirectory(
		                                       sUiAccess.Ptr("main"),
											   QStringLiteral("打开工程"),
											   mpPath->LastProject(),
											   QFileDialog::ShowDirsOnly|
											   QFileDialog::DontResolveSymlinks);
	FAIL_RET_VOID(!path.isEmpty());
	Load(path);
    emit(OpenProject(true));
}
void Project::Load(const QString& path){
	FAIL_RET_VOID(path!=mpPath->Root());
	mpPath->LoadPath(path);
	UpdateState();
	UpdateRecent();
}
void Project::Zip(){
	FAIL_RET_VOID(mpPath->IsValid());
	QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd__hh-mm-ss");
	fileName.append(".zip");
	QString filePath = mpPath->ZipFilePath(fileName);
	bool res = JlCompress::compressDir(filePath,mpPath->WorkPath());
	QString hint;
	if(res){
		hint = QStringLiteral("文件压缩成功，文件路径为：\n");
		hint.append(filePath);
	}else{
		hint = QStringLiteral("文件压缩失败");
	}
	QMessageBox::information(NULL,QStringLiteral("提示"),hint);
}
void Project::UpdateState(){
	mpZip->setEnabled(mpPath->IsValid());
	emit AssetDirChanged(mpPath->AssetPath());
	emit MapDirChanged(mpPath->MapPath());
    emit ObjectDirChanged(mpPath->ObjectPath());
}
void Project::Init(){
	mpPath = new Path;
	AppMod::Register(mpPath);
	mpCache=new Cache(mpPath);
	AppMod::Register(mpCache);
	InitAction();
	UpdateState();
	UpdateRecent();
}
void Project::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("文件"),true,true);
		mpNew = actMng.AddAction(QIcon(":/Icon/newProject.png"),QStringLiteral("新建工程"));
		mpLoad = actMng.AddAction(QIcon(":/Icon/openProject.png"),QStringLiteral("打开工程"));
		mpZip = actMng.AddAction(QIcon(":/Icon/zipProject.png"),QStringLiteral("压缩工程"));
		actMng.AddSeparator();
	actMng.EndPart();

	QMenu* pMenu = sActionMng.Menu(QStringLiteral("文件"));
	mpRecent = pMenu->addMenu(QStringLiteral("最近工程"));
}
void Project::BindSignal(){
	bool res = false;
	res = connect(mpNew,SIGNAL(triggered()),SLOT(New())); assert(res);
	res = connect(mpLoad,SIGNAL(triggered()),SLOT(Load())); assert(res);
	res = connect(mpZip,SIGNAL(triggered()),SLOT(Zip())); assert(res);
}
void Project::UpdateRecent(){
	mpRecent->clear();
	bool res = false;
	foreach(const QString& proj,mpPath->RecentProject()){
		QAction* pAct = mpRecent->addAction(proj);
		res = connect(pAct,&QAction::triggered,[=] () {
			Load(pAct->text());
		}); assert(res);
	}
}


