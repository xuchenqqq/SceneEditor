#include "ObjectDock.h"
#include "ui_ObjectDock.h"
#include "PreHeader.h"
#include "EffectDock.h"
#include <QFileInfo>
#include "Macro.h"
#include "FolderCopyDlg.h"
#include "FolderDelDlg.h"
#include "ObjectModel.h"
#include <assert.h>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>
#include "Asset.h"

ObjectDock::ObjectDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ObjectDock),
    mpModel(new ObjectModel(this))
{
    ui->setupUi(this);
    Init();
    BindSignal();
}

ObjectDock::~ObjectDock()
{
    delete ui;
    delete mpModel;
}

void ObjectDock::SetPath(const QString &path)
{
     mPath = path;
    if(mPath.isEmpty())
    {
        ui->tree->setModel(NULL);
        ui->tree->setContextMenuPolicy(Qt::NoContextMenu);
    }
    else if(ui->tree->model() == NULL)
    {
        ui->tree->setModel(mpModel);
        ui->tree->setRootIndex(mpModel->setRootPath(mPath));
        ui->tree->setContextMenuPolicy(Qt::ActionsContextMenu);
        InitView();
    }
    else
    {
        ui->tree->setRootIndex(mpModel->setRootPath(mPath));
        ui->tree->setCurrentIndex(QModelIndex());
    }
    UpdateAction();
}

void ObjectDock::AddFolder(){
    QString srcDir = QFileDialog::getExistingDirectory(this,QStringLiteral("添加目录"),".",
        QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    FAIL_RET_VOID(!srcDir.isEmpty());
    FolderCopyDlg(srcDir,CurrentPath()).Run();
}

void ObjectDock::DelFolder()
{
    QModelIndex idx = ui->tree->currentIndex();
    FAIL_RET_VOID(idx.isValid());
    bool isYes = QMessageBox::warning(this,
            QStringLiteral("警告"),QStringLiteral("是否确定要删除"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
    FAIL_RET_VOID(isYes);
    QFileInfo info = mpModel->fileInfo(idx);
    if (info.isDir())
    {
        FolderDelDlg(info.absoluteFilePath()).Run();
        mpModel->rmdir(idx);
    }
}

void ObjectDock::Explore(){
    QDesktopServices::openUrl(QUrl::fromLocalFile(CurrentPath()));
}

void ObjectDock::CurrentChanged(const QModelIndex& idx,const QModelIndex&){
    UpdateAction();
    if(idx.isValid())
    {
        Preview(mpModel->filePath(idx));
    }
}
QString ObjectDock::CurrentPath(){
    QModelIndex idx = ui->tree->currentIndex();
    if(idx.isValid()){
        return mpModel->fileInfo(idx).absolutePath();
    }else{
        return mPath;
    }
}
void ObjectDock::Init(){
    InitAction();
    UpdateAction();
}

void ObjectDock::InitView(){
    ui->tree->header()->hide();
    ui->tree->hideColumn(1);
    ui->tree->hideColumn(2);
    ui->tree->hideColumn(3);
    ui->tree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tree->setDragEnabled(true);
    QItemSelectionModel* pModel = ui->tree->selectionModel();
    bool res = false;
    res = connect(pModel,&QItemSelectionModel::currentChanged,
                  this,&ObjectDock::CurrentChanged);assert(res);
}

void ObjectDock::BindSignal()
{
    bool res = false;
    res = connect(ui->addFloder,SIGNAL(clicked()),SLOT(AddFolder()));assert(res);
    res = connect(ui->delFloder,SIGNAL(clicked()),SLOT(DelFolder()));assert(res);
    res = connect(mpAddFolder,SIGNAL(triggered()),SLOT(AddFolder())); assert(res);
    res = connect(mpDelFolder,SIGNAL(triggered()),SLOT(DelFolder())); assert(res);
    res = connect(mpExplore,SIGNAL(triggered()),SLOT(Explore())); assert(res);
    res = connect(ui->preview,SIGNAL(clicked(bool)),ui->groupBox,SLOT(setVisible(bool))); assert(res);
}

void ObjectDock::InitAction(){
    mpAddFolder = new QAction(QIcon(":/Icon/addAssetFolder.png"),QStringLiteral("添加文件夹"),this);
    mpDelFolder= new QAction(QIcon(":/Icon/delAsset.png"),QStringLiteral("删除文件夹"),this);
    mpExplore= new QAction(QIcon(":/Icon/explorer.png"),QStringLiteral("在资源管理器中打开"),this);
    ui->tree->addAction(mpAddFolder);
    ui->tree->addAction(mpDelFolder);
    ui->tree->addAction(mpExplore);
}

void ObjectDock::UpdateAction(){
    if(mPath.isEmpty()){
        ui->delFloder->setEnabled(false);
        ui->addFloder->setEnabled(false);
        mpAddFolder->setEnabled(false);
        mpDelFolder->setEnabled(false);
        mpExplore->setEnabled(false);
    }else{
        bool bCurValid = ui->tree->currentIndex().isValid();
        ui->delFloder->setEnabled(bCurValid);
        mpDelFolder->setEnabled(bCurValid);
        ui->addFloder->setEnabled(true);
        mpAddFolder->setEnabled(true);
        mpExplore->setEnabled(true);
    }
}
void ObjectDock::Preview(const QString &path)
{
    FAIL_RET_VOID(ui->groupBox->isVisible());
    FAIL_RET_VOID(mFilePath != path);
    QFileInfo info(path);
    if(info.isFile()&&info.isReadable()&&Asset::IsSupportPreview(info.suffix()))
    {
        mFilePath = path;
    }
    else
    {
        mFilePath.clear();
    }
    LoadingImage(ui->image->size());
}

void ObjectDock::LoadingImage(const QSize& size)
{
    if(mFilePath.isEmpty())
    {
        ui->image->clear();
        ui->image->setText(QStringLiteral("<文件不可预览>"));
    }
    else
    {
        QImage src(mFilePath);
        QImage dst = src.scaled(size,Qt::KeepAspectRatio);
        ui->image->setText("");
        ui->image->setPixmap(QPixmap::fromImage(dst));
    }
}

