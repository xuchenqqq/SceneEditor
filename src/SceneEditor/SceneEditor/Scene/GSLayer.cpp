#include "PreHeader.h"
#include "GSLayer.h"
#include "Macro.h"
#include <assert.h>
#include "GSScreen.h"
#include <QGraphicsColorizeEffect>
#include <QList>
#include "GSItem.h"
#include "CmdItem.h"
#include "Asset.h"
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include "AppModAlias.h"
#include "EditorScene.h"
#include "CmdItem.h"
#include <QTimerEvent>
#include <QJsonObject>
#include "JsonConvert.h"
#include <QtDebug>

GSLayer::Map GSLayer::msMap;

const int RECORD_POS_INTERVAL = 1000;

GSLayer::GSLayer(const Desc& desc)
: mId(desc.id)
, mScreenId(desc.screenId)
, mSpeed(desc.speed)
, mName(desc.name)
, isObjectLayer(desc.isObjectLayer)
{
	Bind();
	setPos(QPointF(0,0));
	GSScreen* pScreen = Screen();
	mRect = pScreen->boundingRect();
	QGraphicsColorizeEffect * pEffect = new QGraphicsColorizeEffect(this);
	pEffect->setColor(QColor(200,200,200));
	setGraphicsEffect(pEffect);
}
GSLayer::~GSLayer(){
	UnBind();
}
void GSLayer::Preview(){
	setEnabled(false);
	graphicsEffect()->setEnabled(false);
	setAcceptDrops(false);
	setZValue(0.0);
}
QString GSLayer::IsObjectLayer() const{
    QString str;
    if (isObjectLayer){
        str = "对象层";
    }else{
        str = "";
    }
    return str;
}
void GSLayer::Lock(){
	setEnabled(false);
	graphicsEffect()->setEnabled(true);
	setAcceptDrops(false);
	setZValue(0.0);
}
void GSLayer::UnLock(){
	setEnabled(true);
	graphicsEffect()->setEnabled(false);
	setAcceptDrops(true);
	setZValue(1.0);
}
GSLayer::Desc GSLayer::GetDesc() const{
    return Desc(mId,mScreenId,AfterID(),mSpeed,mName,isObjectLayer);
}
QSize GSLayer::Size() const{
	return mRect.size().toSize();
}
QRectF GSLayer::boundingRect() const{
	return mRect;
}
GSScreen* GSLayer::Screen() const{
	return GSScreen::Find(mScreenId);
}
void GSLayer::InsertItem(const GSItem::Desc& desc){
	GSItem* pPostItem = NULL;
	if(desc.afterId!=RtUID::Null){
		pPostItem = Item(desc.afterId);
		ASSERT_MSG_RET_VOID(pPostItem,"");
	}
	GSItem* pItem = new GSItem(desc);
	pItem->setParentItem(this);
	if(pPostItem)
		pItem->stackBefore(pPostItem);
	mItemMap.insert(pItem->ID(),pItem);
	emit ItemInserted(pItem,pPostItem);
}
void GSLayer::InsertItem(const GSItem::Desc &desc, QMap<QString, QString> attr){
    GSItem* pPostItem = NULL;
    if(desc.afterId!=RtUID::Null){
        pPostItem = Item(desc.afterId);
        ASSERT_MSG_RET_VOID(pPostItem,"");
    }
    GSItem* pItem = new GSItem(desc);
    pItem->customAttr = attr;
    pItem->setParentItem(this);
    if(pPostItem)
        pItem->stackBefore(pPostItem);
    mItemMap.insert(pItem->ID(),pItem);
    emit ItemInserted(pItem,pPostItem);
}

void GSLayer::RemoveItem(RtUID::Type id){
	GSItem* pItem = Item(id);
	ASSERT_MSG_RET_VOID(pItem,"");
	scene()->removeItem(pItem);
	mItemMap.remove(id);
	emit ItemRemoved(pItem);
	delete pItem;
}
void GSLayer::OrderItem(RtUID::Type id,RtUID::Type afterId){
	GSItem* pItem = Item(id);
	ASSERT_MSG_RET_VOID(pItem,"找不到ID对应的Item");
	if(afterId != RtUID::Null){
		GSItem* pPostItem = Item(afterId);
		ASSERT_MSG_RET_VOID(pPostItem,"找不到ID对应的Item");
		pItem->setParentItem(NULL);
		pItem->setParentItem(this);
		pItem->stackBefore(pPostItem);
		emit ItemOrdered(pItem,pPostItem);
	}else{
		//如果PostID为RtUID::Null，插入到最后
		pItem->setParentItem(NULL);
		pItem->setParentItem(this);
		emit ItemOrdered(pItem,NULL);
	}
	pItem->update();
}
GSItem* GSLayer::Item(RtUID::Type id) const{
	ItemMapConstIt it = mItemMap.find(id);
	return (it==mItemMap.end())?NULL:it.value();
}
GSItem* GSLayer::BottomItem() const{
	QList<GSItem*> list = ItemList();
	return list.isEmpty()?NULL:list[0];
}
void GSLayer::paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*){
	//只是逻辑上存在，但是不显示任何东西
}
QList<GSItem*> GSLayer::ItemList() const{
	QList<GSItem*> list;
	GSItem* pGSItem = NULL;
	foreach(QGraphicsItem* pItem,childItems()){
		pGSItem = qgraphicsitem_cast<GSItem*>(pItem);
		ASSERT_MSG_RET_X(pGSItem,"Layer子节点必须是Item",QList<GSItem*>());
		list.append(pGSItem);
	}
	return list;
}
void GSLayer::dragEnterEvent(QGraphicsSceneDragDropEvent* event){
    /*
	if(event->mimeData()->hasFormat(Asset::IMAGE)){
		event->setAccepted(true);
	}else{
		event->ignore();
	}
    */
    if (event->mimeData()->hasFormat(Asset::IMAGE)){
        if (!isObjectLayer){
            event->setAccepted(true);
        }else{
            event->ignore();
        }
    }
    else if(event->mimeData()->hasFormat(Asset::EFFECT)){
        if (isObjectLayer){
            event->setAccepted(true);
        }else{
            event->ignore();
        }
    }
    else{
        event->ignore();
    }
}
void GSLayer::dropEvent(QGraphicsSceneDragDropEvent* event){
    /*
	const QMimeData* pMime = event->mimeData();
	if(pMime->hasFormat(Asset::IMAGE)){
		//生成CmdItemInsert命令。
		GSItem::Desc desc(pMime->data(Asset::IMAGE),
			              event->pos(),RtUID::Create(),ID());
		sScene.PushCmd(new CmdItemInsert(desc));
	}
    */
    const QMimeData* pMime = event->mimeData();
    if(pMime->hasFormat(Asset::IMAGE))
    {
        //生成CmdItemInsert命令。
        GSItem::Desc desc(pMime->data(Asset::IMAGE),
                          event->pos(),RtUID::Create(),ID());
        sScene.PushCmd(new CmdItemInsert(desc));
    }
    else if(pMime->hasFormat(Asset::EFFECT))
    {
        //生成CmdItemInsert命令。
        GSItem::Desc desc(pMime->data(Asset::EFFECT),
                          event->pos(),RtUID::Create(),ID());
        sScene.PushCmd(new CmdItemInsert(desc));
    }
}
GSLayer* GSLayer::Find(RtUID::Type id){
	MapIt it = msMap.find(id);
	return (it==msMap.end())?NULL:it.value();
}
void GSLayer::Bind(){
	ASSERT_MSG_RET_VOID(msMap.contains(mId)==false,"重复绑定ID");
	msMap.insert(mId,this);
}
void GSLayer::UnBind(){
	ASSERT_MSG_RET_VOID(msMap.contains(mId),"之前没有绑定ID");
	msMap.remove(mId);
}
RtUID::Type GSLayer::AfterID() const{
	QList<QGraphicsItem*> list = parentItem()->childItems();
	int idx = 0;
	for(idx= 0; idx< list.size(); ++idx){
		if(list[idx]==this) break;
	}
	if(idx+1<list.size()){
		return qgraphicsitem_cast<GSLayer*>(list[idx+1])->ID();
	}else{
		return RtUID::Null;
	}
}

