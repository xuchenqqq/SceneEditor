#pragma once

#include <QList>

class QUndoCommand;

class UserFunc
{
public:
	UserFunc();
	virtual ~UserFunc();
	virtual void OnItemChanged() = 0;
protected:
	void PushCmd(QUndoCommand* pCmd);
	const QList<GSItem*>& Items() const;
};

