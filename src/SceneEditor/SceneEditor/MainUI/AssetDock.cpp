#include "PreHeader.h"
#include "AssetDock.h"
#include <assert.h>
#include <QFileInfo>
#include <QFileDialog>
#include <QAction>
#include "FileCopyDlg.h"
#include "FolderCopyDlg.h"
#include "FolderDelDlg.h"
#include "Macro.h"
#include <QDesktopServices>
#include <QMessageBox>
#include "Asset.h"
#include "Macro.h"


AssetDock::AssetDock(QWidget * parent)
: QDockWidget(parent)
, mModel(this)
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
AssetDock::~AssetDock(){

}
void AssetDock::Init(){
	InitAction();
	UpdateAction();
}
void AssetDock::InitView(){
	ui.tree->header()->hide();
	ui.tree->hideColumn(1);
	ui.tree->hideColumn(2);
	ui.tree->hideColumn(3);
	ui.tree->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tree->setDragEnabled(true);
	QItemSelectionModel* pModel = ui.tree->selectionModel();
	bool res = false;
	res = connect(pModel,&QItemSelectionModel::currentChanged,
		this,&AssetDock::CurrentChanged); assert(res);
}
void AssetDock::BindSignal(){
	bool res = false;
	res = connect(ui.addFile,SIGNAL(clicked()),SLOT(AddFile())); assert(res);
	res = connect(ui.addFolder,SIGNAL(clicked()),SLOT(AddFolder())); assert(res);
	res = connect(ui.del,SIGNAL(clicked()),SLOT(DelAsset())); assert(res);
	res = connect(ui.filter,SIGNAL(textChanged(const QString&)),
		SLOT(Filter(const QString&)));  assert(res);

	res = connect(ui.visible,SIGNAL(clicked(bool)),
		ui.preview,SLOT(setVisible(bool))); assert(res);

	res = connect(mpAddFile,SIGNAL(triggered()),SLOT(AddFile())); assert(res);
	res = connect(mpAddFolder,SIGNAL(triggered()),SLOT(AddFolder())); assert(res);
	res = connect(mpDelAsset,SIGNAL(triggered()),SLOT(DelAsset())); assert(res);
	res = connect(mpExplore,SIGNAL(triggered()),SLOT(ExploreAsset())); assert(res);
}
void AssetDock::AddFile(){
	QStringList fileList = 
		QFileDialog::getOpenFileNames(this,QStringLiteral("添加文件"),".","*.*");
	FAIL_RET_VOID(!fileList.isEmpty());
	FileCopyDlg(fileList,CurrentPath()).Run();
}
void AssetDock::AddFolder(){
	QString srcDir = QFileDialog::getExistingDirectory(this,QStringLiteral("添加目录"),".",
		QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	FAIL_RET_VOID(!srcDir.isEmpty());
	FolderCopyDlg(srcDir,CurrentPath()).Run();
}
void AssetDock::DelAsset(){
	QModelIndex idx = ui.tree->currentIndex();
	FAIL_RET_VOID(idx.isValid());
	bool isYes = QMessageBox::warning(this,
		QStringLiteral("警告"),QStringLiteral("是否确定要删除"),
		QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
	FAIL_RET_VOID (isYes);
	QFileInfo info = mModel.fileInfo(idx);
	if(info.isDir()){
		FolderDelDlg(info.absoluteFilePath()).Run();
		mModel.rmdir(idx);
	}else if(info.isFile()){
		mModel.remove(idx);
	}
}
void AssetDock::ExploreAsset(){
	QDesktopServices::openUrl(QUrl::fromLocalFile(CurrentPath()));
}
void AssetDock::Filter(const QString& str){
	QStringList filters;
	if(!str.isEmpty()){
		filters = str.split(' ');
	}
	mModel.setNameFilters(filters);
}
void AssetDock::SetPath(const QString& path){
	mPath = path;
	if(mPath.isEmpty()){
		ui.tree->setModel(NULL);
		ui.tree->setContextMenuPolicy(Qt::NoContextMenu);
	}else if(ui.tree->model()==NULL){
		ui.tree->setModel(&mModel);
		ui.tree->setRootIndex(mModel.setRootPath(mPath));
		ui.tree->setContextMenuPolicy(Qt::ActionsContextMenu);
		InitView();
	}else{
		ui.tree->setRootIndex(mModel.setRootPath(mPath));
		//hack: setRootIndex时，并不会改变CurrentIndex，需要自己设置一个无效的CurrentIndex，
		//这应该是Qt的bug。
		ui.tree->setCurrentIndex(QModelIndex());
	}
	UpdateAction();
}
void AssetDock::CurrentChanged(const QModelIndex& idx,const QModelIndex&){
	if(idx.isValid()){
		Preview(mModel.filePath(idx));
	}
	UpdateAction();
}
QString AssetDock::CurrentPath(){
	QModelIndex idx = ui.tree->currentIndex();
	if(idx.isValid()){
		return mModel.fileInfo(idx).absolutePath();
	}else{
		return mPath;
	}
}
void AssetDock::InitAction(){
	mpAddFile   = new QAction(QIcon(":/Icon/addAsset.png"),QStringLiteral("添加文件"),this);
	mpAddFolder = new QAction(QIcon(":/Icon/addAssetFolder.png"),QStringLiteral("添加文件夹"),this);
	mpDelAsset= new QAction(QIcon(":/Icon/delAsset.png"),QStringLiteral("删除"),this);
	mpExplore= new QAction(QIcon(":/Icon/explorer.png"),QStringLiteral("在资源管理器中打开"),this);
	ui.tree->addAction(mpAddFile);
	ui.tree->addAction(mpAddFolder);
	ui.tree->addAction(mpDelAsset);
	ui.tree->addAction(mpExplore);
}
void AssetDock::Preview(const QString& path){
	FAIL_RET_VOID(ui.preview->isVisible());
	FAIL_RET_VOID(mFilePath != path);
	QFileInfo info(path); 
	if( info.isFile() &&
		info.isReadable() &&
		Asset::IsSupportPreview(info.suffix()))
	{
		mFilePath = path;
	}else{
		mFilePath.clear();
	}
	LoadImage(ui.image->size());
}
void AssetDock::LoadImage(const QSize& sz){
	if(mFilePath.isEmpty()){
		ui.image->clear();
		ui.image->setText(QStringLiteral("<文件不可预览>"));
	}else{
		QImage src(mFilePath);
		QImage dst = src.scaled(sz,Qt::KeepAspectRatio);
		ui.image->setText("");
		ui.image->setPixmap(QPixmap::fromImage(dst));
	}
}
void AssetDock::UpdateAction(){
	if(mPath.isEmpty()){
		ui.del->setEnabled(false);
		ui.addFile->setEnabled(false);
		ui.addFolder->setEnabled(false);
		mpAddFile->setEnabled(false);
		mpAddFolder->setEnabled(false);
		mpDelAsset->setEnabled(false);
		mpExplore->setEnabled(false);
	}else{
		bool bCurAssetValid = ui.tree->currentIndex().isValid();
		ui.del->setEnabled(bCurAssetValid);
		mpDelAsset->setEnabled(bCurAssetValid);
		ui.addFile->setEnabled(true);
		ui.addFolder->setEnabled(true);
		mpAddFile->setEnabled(true);
		mpAddFolder->setEnabled(true);
		mpExplore->setEnabled(true);
	}
}

