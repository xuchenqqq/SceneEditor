#include "PreHeader.h"
#include "DockMng.h"
#include "Macro.h"
#include <assert.h>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include "AppModAlias.h"
#include "ActionMng.h"

DockMng::DockMng(QMainWindow* pMainWindow)
: QObject(pMainWindow)
, mpMainWindow(pMainWindow)
{
	Init();
}
DockMng::~DockMng()
{}
void DockMng::Reset(){
	AddToWindow();
	foreach(const ROW& row,mDocks){
		if(row.size()==1)continue;
		for(int i = 0;i < (int)(row.size()-1); ++i){
			mpMainWindow->tabifyDockWidget(row[i],row[i+1]);
		}
	}
}
void DockMng::Add(QDockWidget* pDock,const DockInfo& info){
	ASSERT_MSG_RET_VOID(pDock,"不允许无效Dock");
	ASSERT_MSG_RET_VOID(!Find(pDock).IsValid(),"不允许重复添加");
	mDocks.append(ROW());
	Insert(Pos(mDocks.size()-1,0),pDock,info);
}
void DockMng::Add(QDockWidget* preDock,QDockWidget* pDock,bool visible){
	ASSERT_MSG_RET_VOID(preDock,"不允许无效Dock");
	ASSERT_MSG_RET_VOID(pDock,"不允许无效Dock");
	Pos prePos = Find(preDock);
	ASSERT_MSG_RET_VOID(prePos.IsValid(),"前面的Dock必须已经添加");
	ASSERT_MSG_RET_VOID(!Find(pDock).IsValid(),"不允许重复添加");
	const DockInfo& preInfo = mDockInfos[preDock];
	DockInfo info(preInfo.area,preInfo.allowAreas,visible);
	prePos.col++;
	Insert(prePos,pDock,info);
}
DockMng::Pos DockMng::Find(QDockWidget* pDock) const{
	for(int row = 0; row < (int)mDocks.size(); ++row){
		for(int col = 0; col < (int)mDocks[row].size(); ++col){
			if(mDocks[row][col] == pDock){
				return Pos(row,col);
			}
		}
	}
	return Pos();
}
void DockMng::Insert(const Pos& pos,QDockWidget* pDock,const DockInfo& info){
	ROW& row = mDocks[pos.row];
	if(row.size() == pos.col){
		row.append(pDock);
	}else{
		row.insert(pos.col,pDock);
	}
	mDockInfos.insert(pDock,info);
	BindAction(pDock);
}
void DockMng::BindAction(QDockWidget* pDock){
	QMenu* pMenu = sActionMng.Menu(QStringLiteral("视图"));
	QAction* pAction = pMenu->addAction(pDock->windowTitle());
	pAction->setCheckable(true);
	pAction->setChecked(pDock->isVisible());
	bool res = false;
	res = connect(pAction,&QAction::triggered,pDock,&QDockWidget::setVisible); assert(res);
	res = connect(pDock,&QDockWidget::visibilityChanged,pAction,&QAction::setChecked); assert(res);
}

void DockMng::AddToWindow(){
	foreach(const ROW& row,mDocks){
		foreach(QDockWidget* pDock,row){
			const DockInfo& info = mDockInfos[pDock];
			pDock->setAllowedAreas(info.allowAreas);
			pDock->setVisible(info.visible);
			mpMainWindow->addDockWidget(info.area,pDock);
		}
	}
}
void DockMng::Init(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("视图"),true);
		QAction* pActReset = actMng.AddAction(QStringLiteral("界面复位"));
		actMng.AddSeparator();
	actMng.EndPart();
	bool res = false;
	res = connect(pActReset,&QAction::triggered,this,&DockMng::Reset); assert(res);
}
