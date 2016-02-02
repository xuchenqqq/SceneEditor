#include "PreHeader.h"
#include "SceneEditor.h"
#include "AssetDock.h"
#include <assert.h>
#include "EditorScene.h"
#include "GSView.h"
#include "MsgDock.h"
#include "AppMod.h"
#include "ScreenDock.h"
#include "LayerDock.h"
#include "ItemDock.h"
#include "ActionMng.h"
#include "Project.h"
#include "GSScreen.h"
#include "ScreenMng.h"
#include "Simulator.h"
#include "Setting.h"
#include "MapDock.h"
//#include "EffectDock.h"
#include "ObjectDock.h"
#include "DockMng.h"
#include "AttrDock.h"
#include "Auth.h"
#include "AboutDlg.h"
#include "UIAccess.h"
#include "TimerService.h"
#include "ShortcutDlg.h"
#include <QApplication>
#include <QToolBar>

bool coutFormat = false;

SceneEditor::SceneEditor(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	Init();
	BindSignal();
	showMaximized();
}

SceneEditor::~SceneEditor(){
	Clear();
}
void SceneEditor::Init(){
	//严格按照以下顺序，进行初始化。
	InitMod();
	InitUI();
	InitAction();
	InitMainWin();
}
void SceneEditor::InitUI(){
	mpMsgDock   = new MsgDock(this);  AppMod::Register(mpMsgDock);
	mpAssetDock = new AssetDock(this);
//	mpEffectDock = new EffectDock(this);
    mpObjectDock = new ObjectDock(this);
	mpMapDock   = new MapDock(this);
	mpScreenDock= new ScreenDock(this);
	mpLayerDock = new LayerDock(this);
	mpItemDock  = new ItemDock(this);
	mpAttrDock = new AttrDock(this); AppMod::Register(mpAttrDock);
	
	mpDockMng->Add(mpMsgDock,DockInfo(Qt::BottomDockWidgetArea,false));
	mpDockMng->Add(mpAssetDock,Qt::LeftDockWidgetArea);
    mpDockMng->Add(mpAssetDock,mpMapDock);
    mpDockMng->Add(mpAssetDock,mpAttrDock);
    //mpDockMng->Add(mpAssetDock,mpEffectDock);
    mpDockMng->Add(mpAssetDock,mpObjectDock);
    DockInfo gsInfo(Qt::RightDockWidgetArea,Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea);
	mpDockMng->Add(mpScreenDock,gsInfo);
	mpDockMng->Add(mpLayerDock,gsInfo);
	mpDockMng->Add(mpItemDock,gsInfo);
//	mpDockMng->Add(mpAttrDock,gsInfo);
	mpDockMng->Reset();
}
void SceneEditor::InitMod(){
	//严格按照以下顺序，进行初始化
	mpSetting  = new QSettings("config.ini",QSettings::IniFormat,this); AppMod::Register(mpSetting);
	mpActionMng= new ActionMng(this); 	AppMod::Register(mpActionMng);
	mpScreenMng= new ScreenMng(this);   AppMod::Register(mpScreenMng);
	mpProject  = new Project(this);
	mpScene    = new EditorScene(this);  AppMod::Register(mpScene);
	mpView     = new GSView(this); mpView->SetScene(mpScene); AppMod::Register(mpView);
	mpSimulator= new Simulator(this);
	mpDockMng  = new DockMng(this);
	mpAuth     = new Auth(this);
	mpUIAccess = new UIAccess; AppMod::Register(mpUIAccess);
	mpTimer  = new TimerService(this); AppMod::Register(mpTimer);
}
void SceneEditor::InitMainWin(){
	setCentralWidget(mpView);
	setContextMenuPolicy(Qt::NoContextMenu);
	mTaskTimer.setSingleShot(true);
	mTaskTimer.start(0);
	mpUIAccess->Set("main",this);
}
void SceneEditor::Clear(){
	ClearUI();
	ClearMod();
}
void SceneEditor::ClearMod(){
	//严格按照以下顺序
	delete mpSimulator;
	delete mpView;
	delete mpScreenMng;
	delete mpScene;
	delete mpProject;
	delete mpDockMng;
	delete mpActionMng;
	delete mpUIAccess;
	delete mpTimer;
}
void SceneEditor::ClearUI(){
	//严格按照以下顺序
	delete mpItemDock;
	delete mpLayerDock;
	delete mpScreenDock;
	delete mpAssetDock;
	delete mpMapDock;
	delete mpMsgDock;
}
void SceneEditor::BindSignal(){
	bool res = false;
	//ScreenDock
	res = connect(mpScreenDock,&ScreenDock::ScreenChanged,
		mpLayerDock,&LayerDock::OnScreenChanged); assert(res);
	res = connect(mpScreenDock,SIGNAL(ScreenRectChanged(const QRectF&)),
		mpView,SLOT(FitInView(const QRectF&))); assert(res);
	//LayerDock
	res = connect(mpLayerDock,&LayerDock::LayerChanged,
		mpItemDock,&ItemDock::OnLayerChanged); assert(res);
	//ItemDock
	res = connect(mpItemDock,&ItemDock::ClearSelection,
		          mpScene,&QGraphicsScene::clearSelection); assert(res);
	//ScreenMng
	res = connect(mpScreenMng,&ScreenMng::Updated,
		mpScreenDock,&ScreenDock::ScreenUpdated); assert(res);
	//Scene
	res = connect(mpScene,&EditorScene::Modify,mpMapDock,&MapDock::Modified); assert(res);
	//Project
	res = connect(mpProject,&Project::AssetDirChanged,
		mpAssetDock,&AssetDock::SetPath); assert(res);
	res = connect(mpProject,&Project::MapDirChanged,
		mpMapDock,&MapDock::SetPath); assert(res);
//	res = connect(mpProject,&Project::EffectDirChanged,
//		mpEffectDock,&EffectDock::SetPath); assert(res);
    res = connect(mpProject,&Project::ObjectDirChanged,
        mpObjectDock,&ObjectDock::SetPath); assert(res);
    //MapDock
    res = connect(mpMapDock,&MapDock::StateChanged,
                  this,&QMainWindow::setWindowTitle); assert(res);

    res = connect(mpMapDock,&MapDock::StateChanged,
                  mpSimulator,&Simulator::SetMapName); assert(res);

    res = connect(mpProject,&Project::AssetDirChanged,
                  mpSimulator,&Simulator::SetAssertPath); assert(res);
    res = connect(mpProject,&Project::MapDirChanged,
                  mpSimulator,&Simulator::SetMapPath); assert(res);
    res = connect(mpProject,&Project::ObjectDirChanged,
                  mpSimulator,&Simulator::setObjectPath); assert(res);
    res = connect(mpProject,&Project::OpenProject,this,
                  &SceneEditor::ChangeMpCoutFormat); assert(res);

	//Auth
	res = connect(mpAuth,SIGNAL(SigQuit()),SLOT(Quit())); assert(res);
	//内部信号
	res = connect(mpQuit,SIGNAL(triggered()),SLOT(Quit())); assert(res);
	res = connect(mpAbout,SIGNAL(triggered()),SLOT(About())); assert(res);
	res = connect(mpPreview,&QAction::triggered,
		          mpScreenMng,&ScreenMng::Preview); assert(res);
	res = connect(mpHotkey,SIGNAL(triggered()),SLOT(HotkeyConf())); assert(res);
	res = connect(&mTaskTimer,&QTimer::timeout,
		          this,&SceneEditor::PreTask); assert(res);
    res = connect(mpCoutFormat,SIGNAL(triggered()),SLOT(CoutFormat())); assert(res);

}
void SceneEditor::InitAction(){
	mpActionMng->BeginPart(QStringLiteral("文件"),true);
	mpActionMng->AddSeparator();
	mpQuit = mpActionMng->AddAction(QIcon(":/Icon/quit.png"),QStringLiteral("退出"));
	mpActionMng->EndPart();

	mpActionMng->BeginPart(QStringLiteral("视图"),false,true);
	mpActionMng->AddSeparator();
	mpPreview = mpActionMng->AddAction(QIcon(":/Icon/preview.png"),QStringLiteral("预览"));
	mpActionMng->EndPart();	
	mpPreview->setCheckable(true);

	mpActionMng->BeginPart(QStringLiteral("设置"),true);
    mpHotkey = mpActionMng->AddAction(QStringLiteral("自定义快捷键"));
    mpCoutFormat = mpActionMng->AddAction(QStringLiteral("输出/读取xml"));
    mpCoutFormat->setCheckable(true);
    mpCoutFormat->setChecked(false);
    mpActionMng->EndPart();

	mpActionMng->BeginPart(QStringLiteral("帮助"),true,true);
	mpActionMng->AddSeparator();
	mpAbout = mpActionMng->AddAction(QIcon(":/Icon/about.png"),QStringLiteral("关于"));
	mpActionMng->EndPart();
	//必须添加完所有Action后，在建立快捷键。
	mpActionMng->SetupShortcut();
}
void SceneEditor::closeEvent(QCloseEvent *event){
	mpMapDock->Close();
	QMainWindow::closeEvent(event);
}

void SceneEditor::Quit(){
    mpMapDock->Close();
	qApp->quit();
}
void SceneEditor::PreTask(){
	mpAuth->Check(false);
}
void SceneEditor::About(){
	AboutDlg().exec();
}
void SceneEditor::HotkeyConf(){
	ShortcutDlg(this).exec();
}
void SceneEditor::CoutFormat()
{
    if(coutFormat)
    {
        coutFormat = false;
    }
    else
    {
        coutFormat = true;
    }
}

void SceneEditor::ChangeMpCoutFormat(const bool isOpen){
    if(isOpen){
        mpCoutFormat->setEnabled(false);
    }
}
