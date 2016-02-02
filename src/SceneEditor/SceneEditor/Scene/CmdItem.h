#pragma once

#include <QUndoCommand>
#include "GSItem.h"
#include "RtUID.h"

class CmdItemInsert : public QUndoCommand
{
public:
	CmdItemInsert(const GSItem::Desc& desc);
	~CmdItemInsert();
	int id() const;
	void redo();
	void undo();
private:
	GSItem::Desc mDesc;
};

class CmdItemRemove : public QUndoCommand
{
public:
	CmdItemRemove(const GSItem::Desc& desc,QUndoCommand* parent=NULL);
	~CmdItemRemove();
	int id() const;
	void redo();
	void undo();
private:
	GSItem::Desc mDesc;
};

class CmdItemScale : public QUndoCommand
{
public:
	CmdItemScale(const GSItem* pItem,qreal scale);
	~CmdItemScale();
	int id() const;
	void redo();
	void undo();
private:
	RtUID::Type mLayerId;
	RtUID::Type mId;
	qreal mNewScale;
	qreal mOldScale;
};

class CmdItemMove : public QUndoCommand
{
public:
	CmdItemMove(const GSItem* pItem,const QPointF& dstPos);
	CmdItemMove(const GSItem* pItem,const QPointF& srcPos,const QPointF& dstPos);
	~CmdItemMove();
	bool mergeWith(const QUndoCommand* command);
	int id() const;
	void redo();
	void undo();
private:
	RtUID::Type mLayerId;
	RtUID::Type mId;
	QPointF mNewPos;
	QPointF mOldPos;
};

class CmdItemOrder : public QUndoCommand
{
public:
	CmdItemOrder(const GSItem* pItem,const GSItem* pTargetItem=NULL);
	int id() const;
	void redo();
	void undo();
private:
	RtUID::Type mLayerId;
	RtUID::Type mId;
	RtUID::Type mNewPostId;
	RtUID::Type mOldPostId;
};

class CmdItemImage : public QUndoCommand
{
public:
	CmdItemImage(const GSItem* pItem,const QString& image);
	int id() const;
	void redo();
	void undo();
private:
	RtUID::Type mLayerId;
	RtUID::Type mId;
	QString mNewImage;
	QString mOldImage;
};

class CmdItemFlip : public QUndoCommand
{
public:
	static CmdItemFlip* NewX(const GSItem* pItem,bool state);
	static CmdItemFlip* NewY(const GSItem* pItem,bool state);
	int id() const;
	void redo();
	void undo();
private:
	enum Type{
		X,
		Y	
	};
	CmdItemFlip(const GSItem* pItem,Type type,bool state);
private:
	RtUID::Type mLayerId;
	RtUID::Type mId;
	Type mType;
	bool mNew;
	bool mOld;
};

