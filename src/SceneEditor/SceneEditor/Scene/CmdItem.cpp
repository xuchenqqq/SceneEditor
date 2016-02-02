#include "PreHeader.h"
#include "CmdItem.h"
#include "CmdID.h"
#include "GSLayer.h"
#include <assert.h>
#include "Macro.h"


//////CmdItemInsert
CmdItemInsert::CmdItemInsert(const GSItem::Desc& desc)
: mDesc(desc)
{
}
CmdItemInsert::~CmdItemInsert(){

}
int CmdItemInsert::id() const{
	return CmdID::ITEM_INSERT;
}
void CmdItemInsert::redo(){
	GSLayer* pLayer = GSLayer::Find(mDesc.layerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->InsertItem(mDesc);
}
void CmdItemInsert::undo(){
	GSLayer* pLayer = GSLayer::Find(mDesc.layerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->RemoveItem(mDesc.id);
}

//////CmdItemRemove
CmdItemRemove::CmdItemRemove(const GSItem::Desc& desc,QUndoCommand* parent)
: QUndoCommand(parent)
, mDesc(desc)
{

}
CmdItemRemove::~CmdItemRemove(){

}
int CmdItemRemove::id() const{
	return CmdID::ITEM_REMOVE;
}
void CmdItemRemove::redo(){
	GSLayer* pLayer = GSLayer::Find(mDesc.layerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->RemoveItem(mDesc.id);
}
void CmdItemRemove::undo(){
	GSLayer* pLayer = GSLayer::Find(mDesc.layerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->InsertItem(mDesc);
}

//////CmdItemScale
CmdItemScale::CmdItemScale(const GSItem* pItem,qreal scale)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mNewScale(scale)
, mOldScale(pItem->scale())
{

}
CmdItemScale::~CmdItemScale(){

}
int CmdItemScale::id() const{
	return CmdID::ITEM_SCALE;
}
void CmdItemScale::redo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	pItem->setScale(mNewScale);
}
void CmdItemScale::undo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	pItem->setScale(mOldScale);
}

//////CmdItemMove
CmdItemMove::CmdItemMove(const GSItem* pItem,const QPointF& srcPos,const QPointF& dstPos)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mNewPos(dstPos)
, mOldPos(srcPos)
{
}
CmdItemMove::CmdItemMove(const GSItem* pItem,const QPointF& dstPos)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mNewPos(dstPos)
, mOldPos(pItem->pos())
{}
CmdItemMove::~CmdItemMove(){

}
bool CmdItemMove::mergeWith(const QUndoCommand* other){
	return false;
	FAIL_RET(other->id() == id());
	const CmdItemMove* otherScale = static_cast<const CmdItemMove*>(other);
	mNewPos = otherScale->mNewPos;
	return true;
}
int CmdItemMove::id() const{
	return CmdID::ITEM_MOVE;
}
void CmdItemMove::redo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	if(pItem->pos() != mNewPos){
		pItem->SetPosNotRecord(mNewPos);
	}
}
void CmdItemMove::undo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	if(pItem->pos() != mOldPos){
		pItem->SetPosNotRecord(mOldPos);
	}
}

//////CmdItemOrder
CmdItemOrder::CmdItemOrder(const GSItem* pItem,const GSItem* pTargetItem)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mNewPostId(pTargetItem?pTargetItem->ID():RtUID::Null)
, mOldPostId(pItem->AfterID())
{}
int CmdItemOrder::id() const{
	return CmdID::ITEM_ORDER;
}
void CmdItemOrder::redo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->OrderItem(mId,mNewPostId);
}
void CmdItemOrder::undo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	pLayer->OrderItem(mId,mOldPostId);
}

//////CmdItemImage
CmdItemImage::CmdItemImage(const GSItem* pItem,const QString& image)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mNewImage(image)
, mOldImage(pItem->File())
{}
int CmdItemImage::id() const{
	return CmdID::ITEM_ORDER;
}
void CmdItemImage::redo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	pItem->SetFile(mNewImage);
}
void CmdItemImage::undo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	pItem->SetFile(mOldImage);
}

CmdItemFlip* CmdItemFlip::NewX(const GSItem* pItem,bool state){
	return new CmdItemFlip(pItem,X,state);
}
CmdItemFlip* CmdItemFlip::NewY(const GSItem* pItem,bool state){
	return new CmdItemFlip(pItem,Y,state);
}
CmdItemFlip::CmdItemFlip(const GSItem* pItem,Type type,bool state)
: mLayerId(pItem->Layer()->ID())
, mId(pItem->ID())
, mType(type)
, mNew(state)
, mOld(type==X?pItem->FlipX():pItem->FlipY())
{}
int CmdItemFlip::id() const{
	return CmdID::ITEM_FLIP;
}
void CmdItemFlip::redo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	if(mType==X){
		pItem->SetFlipX(mNew);
	}else{
		pItem->SetFlipY(mNew);
	}	
}
void CmdItemFlip::undo(){
	GSLayer* pLayer = GSLayer::Find(mLayerId);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	GSItem* pItem = pLayer->Item(mId);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	if(mType==X){
		pItem->SetFlipX(mOld);
	}else{
		pItem->SetFlipY(mOld);
	}	
}


