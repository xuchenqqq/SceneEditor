#pragma once

#include <QObject>
#include <QList>

class UserFunc;
class QUndoStack;
class QUndoCommand;
class GSItem;
class QAction;

class UserFuncMng : public QObject
{
Q_OBJECT
public:
	UserFuncMng(QObject* parent);
	~UserFuncMng();
	void PushCmd(QUndoCommand* pCmd);
signals:
	void Modify();
private slots:
	void ItemChanged();
	void Redo();
	void Undo();
private:
	void Init();
	void Add(UserFunc* pUserFunc);
	void BindSignal();
	void Clear();
	void InitAction();
	void InitUserFunc();
private:
	friend class UserFunc;
	QList<UserFunc*> mFuncList;
	QList<GSItem*> mSelItems;
	QUndoStack* mpCmdStack;
	QAction* mpRedo;
	QAction* mpUndo;
};

