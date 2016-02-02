#include "PreHeader.h"
#include "CmdLayer.h"
#include "GSScreen.h"
#include "CmdID.h"
#include "CmdItem.h"
#include "Macro.h"
#include <assert.h>

//////CmdLayerInsert
CmdLayerInsert::CmdLayerInsert(const GSLayer::Desc& desc)
: mDesc(desc)
{
}
int CmdLayerInsert::id() const{
	return CmdID::LAYER_INSERT;
}
void CmdLayerInsert::redo(){
	GSScreen* pScreen = GSScreen::Find(mDesc.screenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
    pScreen->InsertLayer(mDesc);
}
void CmdLayerInsert::undo(){
	GSScreen* pScreen = GSScreen::Find(mDesc.screenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
	pScreen->RemoveLayer(mDesc.id);
}

//////CmdLayerRemove
CmdLayerRemove::CmdLayerRemove(const GSLayer::Desc& desc)
: mDesc(desc)
{
	GSLayer* pLayer = GSLayer::Find(desc.id);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	//严格按照以下顺序创建
	foreach(GSItem* pItem,pLayer->ItemList()){
		new CmdItemRemove(pItem->GetDesc(),this);
	}
}
int CmdLayerRemove::id() const{
	return CmdID::LAYER_REMOVE;
}
void CmdLayerRemove::redo(){
	//1.顺序删除子对象
	QUndoCommand::redo();
	//2.删除自身
	GSScreen* pScreen = GSScreen::Find(mDesc.screenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
	pScreen->RemoveLayer(mDesc.id);
}
void CmdLayerRemove::undo(){
	//1.创建自身
	GSScreen* pScreen = GSScreen::Find(mDesc.screenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
    pScreen->InsertLayer(mDesc);
	//2.逆序创建子对象
	QUndoCommand::undo();
}

//////CmdLayerOrder
CmdLayerOrder::CmdLayerOrder(const GSLayer* pLayer,const GSLayer* pTargetLayer)
: mScreenId(pLayer->Screen()->ID())
, mId(pLayer->ID())
, mNewPostId(pTargetLayer->ID())
, mOldPostId(pLayer->AfterID())
{
}
int CmdLayerOrder::id() const{
	return CmdID::LAYER_ORDER;
}
void CmdLayerOrder::redo(){
	GSScreen* pScreen = GSScreen::Find(mScreenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
	pScreen->OrderLayer(mId,mNewPostId);
}
void CmdLayerOrder::undo(){
	GSScreen* pScreen = GSScreen::Find(mScreenId);
	ASSERT_MSG_RET_VOID(pScreen,"找不到ID对应的Screen");
	pScreen->OrderLayer(mId,mOldPostId);
}



