#include "PreHeader.h"
#include "EffectDock.h"
#include <QFileInfo>
#include "Macro.h"
#include "FolderCopyDlg.h"
#include "FolderDelDlg.h"
#include "EffectModel.h"
#include <assert.h>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>

EffectDock::EffectDock(QWidget * parent)
: QDockWidget(parent)
, mpModel(new EffectModel(this))
{
    ui.setupUi(this);
    Init();
    BindSignal();
}
EffectDock::~EffectDock(){
    delete mpModel;
}
void EffectDock::SetPath(const QString& path){
    mPath = path;
    if(mPath.isEmpty()){
        ui.tree->setModel(NULL);
        ui.tree->setContextMenuPolicy(Qt::NoContextMenu);
    }else if(ui.tree->model()==NULL){
        ui.tree->setModel(mpModel);
        ui.tree->setRootIndex(mpModel->setRootPath(mPath));
        ui.tree->setContextMenuPolicy(Qt::ActionsContextMenu);
        InitView();
    }else{
        ui.tree->setRootIndex(mpModel->setRootPath(mPath));
        //hack: setRootIndex时，并不会改变CurrentIndex，需要自己设置一个无效的CurrentIndex，
        //这应该是Qt的bug。
        ui.tree->setCurrentIndex(QModelIndex());
    }
    UpdateAction();
}
void EffectDock::AddFolder(){
    QString srcDir = QFileDialog::getExistingDirectory(this,QStringLiteral("添加目录"),".",
        QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    FAIL_RET_VOID(!srcDir.isEmpty());
    FolderCopyDlg(srcDir,CurrentPath()).Run();
}
void EffectDock::DelFolder(){
    QModelIndex idx = ui.tree->currentIndex();
    FAIL_RET_VOID(idx.isValid());
    bool isYes = QMessageBox::warning(this,
        QStringLiteral("警告"),QStringLiteral("是否确定要删除"),
        QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
    FAIL_RET_VOID (isYes);
    QFileInfo info = mpModel->fileInfo(idx);
    if(info.isDir()){
        FolderDelDlg(info.absoluteFilePath()).Run();
        mpModel->rmdir(idx);
    }
}
void EffectDock::Explore(){
    QDesktopServices::openUrl(QUrl::fromLocalFile(CurrentPath()));
}
void EffectDock::CurrentChanged(const QModelIndex&,const QModelIndex&){
    UpdateAction();
}
QString EffectDock::CurrentPath(){
    QModelIndex idx = ui.tree->currentIndex();
    if(idx.isValid()){
        return mpModel->fileInfo(idx).absolutePath();
    }else{
        return mPath;
    }
}
void EffectDock::Init(){
    InitAction();
    UpdateAction();
}
void EffectDock::InitView(){
    ui.tree->header()->hide();
    ui.tree->hideColumn(1);
    ui.tree->hideColumn(2);
    ui.tree->hideColumn(3);
    ui.tree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tree->setDragEnabled(true);
    QItemSelectionModel* pModel = ui.tree->selectionModel();
    bool res = false;
    res = connect(pModel,&QItemSelectionModel::currentChanged,
        this,&EffectDock::CurrentChanged); assert(res);
}
void EffectDock::BindSignal(){
    bool res = false;
    res = connect(ui.addFolder,SIGNAL(clicked()),SLOT(AddFolder())); assert(res);
    res = connect(ui.delFolder,SIGNAL(clicked()),SLOT(DelFolder())); assert(res);
    res = connect(mpAddFolder,SIGNAL(triggered()),SLOT(AddFolder())); assert(res);
    res = connect(mpDelFolder,SIGNAL(triggered()),SLOT(DelFolder())); assert(res);
    res = connect(mpExplore,SIGNAL(triggered()),SLOT(Explore())); assert(res);
}
void EffectDock::InitAction(){
    mpAddFolder = new QAction(QIcon(":/Icon/addAssetFolder.png"),QStringLiteral("添加文件夹"),this);
    mpDelFolder= new QAction(QIcon(":/Icon/delAsset.png"),QStringLiteral("删除文件夹"),this);
    mpExplore= new QAction(QIcon(":/Icon/explorer.png"),QStringLiteral("在资源管理器中打开"),this);
    ui.tree->addAction(mpAddFolder);
    ui.tree->addAction(mpDelFolder);
    ui.tree->addAction(mpExplore);
}
void EffectDock::UpdateAction(){
    if(mPath.isEmpty()){
        ui.delFolder->setEnabled(false);
        ui.addFolder->setEnabled(false);
        mpAddFolder->setEnabled(false);
        mpDelFolder->setEnabled(false);
        mpExplore->setEnabled(false);
    }else{
        bool bCurValid = ui.tree->currentIndex().isValid();
        ui.delFolder->setEnabled(bCurValid);
        mpDelFolder->setEnabled(bCurValid);
        ui.addFolder->setEnabled(true);
        mpAddFolder->setEnabled(true);
        mpExplore->setEnabled(true);
    }
}

