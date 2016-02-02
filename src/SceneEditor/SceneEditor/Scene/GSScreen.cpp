#include "PreHeader.h"
#include "GSScreen.h"
#include "Macro.h"
#include <assert.h>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonArray>

GSScreen::Map GSScreen::msMap;

GSScreen::GSScreen(const QSize& size,const QPoint& pos,RtUID::Type id)
: mSize(size)
, mId(id)
{
	setPos(pos);
	Bind();
}
GSScreen::~GSScreen(){
	UnBind();
}
void GSScreen::Preview(){
	setVisible(true);
	foreach(GSLayer* pLayer,LayerList()){
		pLayer->Preview();
	}
}
QRectF GSScreen::boundingRect() const{
	return QRectF(0,-mSize.height(),mSize.width(),mSize.height());
}
void GSScreen::paint(QPainter* painter,const QStyleOptionGraphicsItem*,QWidget*){
	painter->setRenderHint(QPainter::Antialiasing,true);
	painter->setPen(QPen(Qt::black, 0.5, Qt::DotLine));
    painter->setBrush(Qt::NoBrush);
	painter->drawRect(boundingRect());
	painter->setRenderHint(QPainter::Antialiasing,false);
}
bool GSScreen::Clear(SelItem& item) const{
	QList<QGraphicsItem*> childList = childItems();
	FAIL_RET(!childList.isEmpty());
	if(childList.last()->zValue() > 0){
		item.isSelect = true;
		item.pItem = childList.last();
		item.pItem->setZValue(0);
	}else{
		item.isSelect = false;
		item.pItem = 0;
	}
	return true;
}
void GSScreen::Restore(SelItem& item) const{
	if(item.isSelect){
		item.pItem->setZValue(1);
	}
}
QList<GSLayer*> GSScreen::LayerList() const{
	QList<GSLayer*> list;
	SelItem selItem;
	GSLayer* pLayer = NULL;
	FAIL_RET_X(Clear(selItem),list);
	foreach(QGraphicsItem* pItem,childItems()){
		pLayer = qgraphicsitem_cast<GSLayer*>(pItem);
		ASSERT_MSG_RET_X(pLayer,"Screen子节点必须是Layer",QList<GSLayer*>());
		list.append(pLayer);
	}
	Restore(selItem);
	return list;
}
GSLayer* GSScreen::InsertLayer(const GSLayer::Desc& desc){
	GSLayer* pPostLayer = NULL;
	if(desc.afterId!=RtUID::Null){
		pPostLayer = GSLayer::Find(desc.afterId);
		ASSERT_MSG_RET_X(pPostLayer,"无效Layer Id",NULL);
	}
    GSLayer* pLayer = new GSLayer(desc);
	pLayer->setParentItem(this);
	if(pPostLayer)
		pLayer->stackBefore(pPostLayer);
	emit LayerInserted(pLayer,pPostLayer);
	return pLayer;
}
void GSScreen::RemoveLayer(RtUID::Type id){
	GSLayer* pLayer = GSLayer::Find(id);
	ASSERT_MSG_RET_VOID(pLayer,"ID无效");
	scene()->removeItem(pLayer);
	emit LayerRemoved(pLayer);
	delete pLayer;
}
void GSScreen::OrderLayer(RtUID::Type id,RtUID::Type postId){
	GSLayer* pLayer = GSLayer::Find(id);
	ASSERT_MSG_RET_VOID(pLayer,"找不到ID对应的Layer");
	if(postId != RtUID::Null){
		GSLayer* pPostLayer = GSLayer::Find(postId);
		ASSERT_MSG_RET_VOID(pPostLayer,"找不到ID对应的Layer");
		pLayer->setParentItem(NULL);
		pLayer->setParentItem(this);
		pLayer->stackBefore(pPostLayer);
		emit LayerOrdered(pLayer,pPostLayer);
	}else{
		//如果PostID为RtUID::Null，插入到最后
		pLayer->setParentItem(NULL);
		pLayer->setParentItem(this);
		emit LayerOrdered(pLayer,NULL);
	}
}
GSScreen* GSScreen::Find(RtUID::Type id){
	MapIt it = msMap.find(id);
	return (it==msMap.end())?NULL:it.value();
}
void GSScreen::Bind(){
	ASSERT_MSG_RET_VOID(msMap.contains(mId)==false,"重复绑定ID");
	msMap.insert(mId,this);
}
void GSScreen::UnBind(){
	ASSERT_MSG_RET_VOID(msMap.contains(mId),"之前没有绑定ID");
	msMap.remove(mId);
}
QRectF GSScreen::RectBaseScene() const{
	return QRectF(pos().x(),-mSize.height(),mSize.width(),mSize.height());
}

