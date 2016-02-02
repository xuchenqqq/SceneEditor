#include "PreHeader.h"
#include "ActionMng.h"
#include "Macro.h"
#include <assert.h>
#include <QToolBar>
#include <QMenu>
#include <QMainWindow>
#include <QMenuBar>
#include "AppModAlias.h"
#include "Setting.h"

const char* const SETTING_SHORTCUT = "Action/Shortcut";
const char* const SEPARATOR = "#";

ActionMng::ActionMng(QMainWindow* pMainWindow)
: QObject(pMainWindow)
, mpMainWin(pMainWindow)
{}
ActionMng::~ActionMng(){
}
void ActionMng::BeginPart(const QString& name,bool hasMenu,bool hasTool){
	ASSERT_MSG_RET_VOID(!name.isEmpty(),"名字为空");
	EndPart();
	mCurPart = name;
	mCurMenu = hasMenu;
	mCurTool = hasTool;
	if(mCurMenu && !mMenuMap.contains(mCurPart)){
		QMenu* pMenu = new QMenu(mCurPart);
		mMenuMap.insert(mCurPart,pMenu);
		mpMainWin->menuBar()->addMenu(pMenu);
	}
	if(mCurTool && !mToolMap.contains(mCurPart)){
		QToolBar* pTool = new QToolBar(mCurPart);
		mToolMap.insert(mCurPart,pTool);
		mpMainWin->addToolBar(pTool);
	}
}
void ActionMng::EndPart(){
	mCurPart.clear();
	mCurMenu = false;
	mCurTool = false;
}
QAction* ActionMng::AddAction(const QIcon& icon,const QString& text){
	QAction* pAct = new QAction(icon,text,this);
	AddAction(pAct);
	return pAct;
}
QAction* ActionMng::AddAction(const QString& text){
	QAction* pAct = new QAction(text,this);
	AddAction(pAct);
	return pAct;
}
void ActionMng::AddAction(QAction* pAct){
	ASSERT_MSG_RET_VOID(!mCurPart.isEmpty(),"当前模块无效");
	mActMap.insert(mCurPart,pAct);
	if(mCurMenu){
		mMenuMap[mCurPart]->addAction(pAct);
	}
	if(mCurTool){
		mToolMap[mCurPart]->addAction(pAct);
	}
}
void ActionMng::AddSeparator(){
	if(mCurMenu){
		mMenuMap[mCurPart]->addSeparator();
	}
	if(mCurTool){
		mToolMap[mCurPart]->addSeparator();
	}
}
void ActionMng::AddDefShourtcut(const Func& func){
	mDefShortcut.append(func);
}
void ActionMng::SetupShortcut(){
	if(!ReadShortcut()){
		SetupDefShortcut();
	}
}
bool ActionMng::ReadShortcut(){
	FAIL_RET(sSetting.contains(SETTING_SHORTCUT));
	QStringList db = sSetting.value(SETTING_SHORTCUT).toStringList();
	foreach(const QString& item,db){
		QStringList content = item.split(SEPARATOR);
		ASSERT_MSG_RET(content.size()==3,"快捷键配置格式错误");
		SetShortcut(content[0],content[1],content[2]);
	}
	return true;
}
void ActionMng::SetShortcut(const QString& key,
				 const QString& action,
				 const QString& shortcut){
	foreach(QAction* pAct,mActMap.values(key)){
		if(pAct->text() == action){
			pAct->setShortcut(QKeySequence(shortcut,QKeySequence::PortableText));
			return;
		}
	}
}
void ActionMng::SaveShortcut(){
	QStringList db;
	QMapIterator<QString,QAction*> i(mActMap);
	while(i.hasNext()){
		i.next();
		if(i.value()->shortcut().isEmpty() == false){
			QString item;
			item.append(i.key());
			item.append(SEPARATOR);
			item.append(i.value()->text());
			item.append(SEPARATOR);
			item.append(i.value()->shortcut().toString());
			db.append(item);
		}
	}
	if(!db.isEmpty()){
		sSetting.setValue(SETTING_SHORTCUT,db);
	}
}
void ActionMng::SetupDefShortcut(){
	foreach(const Func& func,mDefShortcut){
		func();
	}
}
QToolBar* ActionMng::ToolBar(const QString& name) const{
	ToolMapCIt it = mToolMap.find(name);
	ASSERT_MSG_RET_X(it!=mToolMap.end(),"没有创建相应的工具栏",0);
	return it.value();
}
QMenu* ActionMng::Menu(const QString& name) const{
	MenuMapCIt it = mMenuMap.find(name);
	ASSERT_MSG_RET_X(it!=mMenuMap.end(),"没有创建相应的菜单栏",0);
	return it.value();
}
QList<QString> ActionMng::Parts() const{
	return mActMap.uniqueKeys();
}
QList<QAction*> ActionMng::Actions(const QString& part) const{
	return mActMap.values(part);
}


