#include "PreHeader.h"
#include "ItemClipboard.h"
#include "AppModAlias.h"
#include "EditorScene.h"
#include "CmdItem.h"
#include "qdebug.h"

ItemClipboard::ItemClipboard()
{}
ItemClipboard::~ItemClipboard(){
}
void ItemClipboard::Copy(QList<GSItem*> itemList){
	mItemDescs.clear();
	if(itemList.size()){
		mRect.setTopLeft(itemList[0]->pos());
		mRect.setBottomRight(itemList[0]->pos());
	}
	foreach(GSItem* pItem,itemList){
        GSItem::Desc desc = pItem->GetDesc();
        desc.id = RtUID::Null;
        desc.layerId = RtUID::Null;
        desc.afterId = RtUID::Null;
		mItemDescs.append(desc);
		RectAddPoint(desc.pos);
	}
}
void ItemClipboard::Paste(const QPointF& center,RtUID::Type layerId){
	QPointF offset;
	if(!center.isNull()){
		offset = center - mRect.center();
	}
	foreach(GSItem::Desc desc,mItemDescs){
		desc.pos+=offset;
		desc.id = RtUID::Create();
        desc.layerId = layerId;
		sScene.PushCmd(new CmdItemInsert(desc));
	}
}
void ItemClipboard::RectAddPoint(const QPointF& pos){
	if(pos.x() < mRect.left()){
		mRect.setLeft(pos.x());
	}else if(pos.x() > mRect.right()){
		mRect.setRight(pos.x());
	}
	
	if(pos.y() < mRect.top()){
		mRect.setTop(pos.y());
	}else if(pos.y() > mRect.bottom()){
		mRect.setBottom(pos.y());
	}
}
