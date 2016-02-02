#include "PreHeader.h"
#include "UserFuncMng.h"
#include <QUndoStack>

UserFuncMng::UserFuncMng(QObject* parent)
: QObject(parent)
{
	Init();
	BindSignal();
}
UserFuncMng::~UserFuncMng(){
	Clear();
}
void UserFuncMng::PushCmd(QUndoCommand* pCmd){
	mpUndoStack->push(pCmd);
	emit Modify();
}
void UserFuncMng::ItemChanged(){
	foreach(UserFunc* pFunc,mFuncList){
		pFunc->OnItemChanged();
	}
}
void UserFuncMng::Redo(){
	mpUndoStack->redo();
	emit Modify();
}
void UserFuncMng::Undo(){
	mpUndoStack->undo();
	emit Modify();
}
void UserFuncMng::Init(){
	mpCmdStack = new QUndoStack(this);
	mpCmdStack->setUndoLimit(200);
	InitAction();
	InitUserFucn();
}
void UserFuncMng::Add(UserFunc* pUserFunc){
	mList.append(pUserFunc);
}
void UserFuncMng::BindSignal(){
	bool res = false;
	res = connect(mpRedo,SIGNAL(triggered()),SLOT(Redo())); assert(res);
	res = connect(mpUndo,SIGNAL(triggered()),SLOT(Undo())); assert(res);
	res = connect(mpCmdStack,SIGNAL(canRedoChanged(bool)),mpRedo,SLOT(setEnabled(bool))); assert(res);
	res = connect(mpCmdStack,SIGNAL(canUndoChanged(bool)),mpUndo,SLOT(setEnabled(bool))); assert(res);
}
void UserFuncMng::Clear(){

}
void UserFuncMng::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("编辑"),true,true);
	mpRedo = actMng.AddAction(QIcon(":/Icon/redo.png"),QStringLiteral("重做"));
	mpUndo = actMng.AddAction(QIcon(":/Icon/undo.png"),QStringLiteral("撤销"));
	actMng.EndPart();
	actMng.AddDefShourtcut([=](){
		//默认快捷键
		mpRedo->setShortcuts(QKeySequence::Redo);
		mpUndo->setShortcuts(QKeySequence::Undo);
	});
}
void UserFuncMng::InitUserFunc(){
	
}

