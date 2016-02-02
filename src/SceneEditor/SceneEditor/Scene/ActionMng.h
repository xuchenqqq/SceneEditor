#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QList>
#include <QMultiMap>
#include <functional>
#include "ModDeclare.h"

class QMainWindow;
class QToolBar;
class QMenu;
class QAction;

class ActionMng : public QObject
{
Q_OBJECT
public:
	typedef std::function<void()> Func;
	ActionMng(QMainWindow* pMainWindow);
	~ActionMng();
	void BeginPart(const QString& name,bool hasMenu=false,bool hasTool=false);
	void EndPart();
	QAction* AddAction(const QIcon& icon,const QString& text);
	QAction* AddAction(const QString& text);
	void AddSeparator();
	void AddDefShourtcut(const Func& func);
	void SaveShortcut();
	void SetupShortcut();
	QToolBar* ToolBar(const QString& name) const;
	QMenu* Menu(const QString& name) const;
	QList<QString> Parts() const;
	QList<QAction*> Actions(const QString& part) const;
private:
	void AddAction(QAction* pAct);
	bool ReadShortcut();
	void SetupDefShortcut();
	void SetShortcut(const QString& key,
		             const QString& action,
		             const QString& shortcut);
private:
	QMainWindow* mpMainWin;
	typedef QMap<QString,QToolBar*> ToolMap;
	typedef ToolMap::iterator ToolMapIt;
	typedef ToolMap::const_iterator ToolMapCIt;
	ToolMap mToolMap;
	typedef QMap<QString,QMenu*> MenuMap;
	typedef MenuMap::iterator MenuMapIt;
	typedef MenuMap::const_iterator MenuMapCIt;
	MenuMap mMenuMap;
	QMultiMap<QString,QAction*> mActMap;
	QList<Func> mDefShortcut;
	QString mCurPart;
	bool    mCurMenu;
	bool    mCurTool;
};

M_DECL_MOD(ActionMng)
