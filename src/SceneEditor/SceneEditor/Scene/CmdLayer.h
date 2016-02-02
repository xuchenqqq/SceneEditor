#ifndef CMD_LAYER_H
#define CMD_LAYER_H

#include <QUndoCommand>
#include "RtUID.h"
#include "GSLayer.h"

class CmdLayerInsert : public QUndoCommand
{
public:
    CmdLayerInsert(const GSLayer::Desc& desc);
	int id() const;
	void redo();
	void undo();
private:
    GSLayer::Desc mDesc;
};

class CmdLayerRemove : public QUndoCommand
{
public:
	CmdLayerRemove(const GSLayer::Desc& desc);
	int id() const;
	void redo();
	void undo();
private:
	GSLayer::Desc mDesc;
};

class CmdLayerOrder : public QUndoCommand
{
public:
	CmdLayerOrder(const GSLayer* pLayer,const GSLayer* pTargetLayer);
	int id() const;
	void redo();
	void undo();
private:
	RtUID::Type mScreenId;
	RtUID::Type mId;
	RtUID::Type mNewPostId;
	RtUID::Type mOldPostId;
};

#endif
