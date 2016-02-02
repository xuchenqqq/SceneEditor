#include "PreHeader.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "ActionMng.h"
#include "CmdItem.h"
#include <QUndoStack>
#include "Macro.h"
#include <assert.h>
#include "GSLayer.h"
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include "ScaleDlg.h"
#include "Macro.h"
#include "ItemAlign.h"
#include "AttrDock.h"
#include "ItemClipboard.h"
#include "GSInfo.h"
#include "GSView.h"
#include "ItemQueue.h"
#include "Log.h"
#include "qdebug.h"

EditorScene::EditorScene(QObject* parent)
: QGraphicsScene(parent)
, mpAlignTop(NULL)
, mpAlignDown(NULL)
, mpScale(NULL)
, mpOrderTop(NULL)
, mpOrderBottom(NULL)
, mpDelete(NULL)
, mpRedo(NULL)
, mpUndo(NULL)
, mpUndoStack(NULL)
, mpItemSignal(NULL)
{
	Init();
	BindSignal();
}
EditorScene::~EditorScene(){
	
}
void EditorScene::PushCmd(QUndoCommand* pCmd){
	mpUndoStack->push(pCmd);
	emit Modify();
}
void EditorScene::Redo(){
	mpUndoStack->redo();
	emit Modify();
}
void EditorScene::Undo(){
	mpUndoStack->undo();
	emit Modify();
}
void EditorScene::Copy(){
	FAIL_RET_VOID(mpInfo->Layer());
	QList<QGraphicsItem*> selItems = selectedItems();
	FAIL_RET_VOID(selItems.size()>0);
	QList<GSItem*> itemList;
	GSItem* pGSItem = NULL;
	foreach(QGraphicsItem* pItem,selItems){
        pGSItem = qgraphicsitem_cast<GSItem*>(pItem);
		ASSERT_MSG_RET_VOID(pGSItem,"选中非GSItem类型");
        itemList.append(pGSItem);
    }
	mpClipboard->Copy(itemList);
	mpPaste->setEnabled(true);
}
void EditorScene::Paste(){
	GSLayer* pLayer = mpInfo->Layer();
	FAIL_RET_VOID(pLayer);
	mpClipboard->Paste(mpInfo->LayerMousePos(),pLayer->ID());
	mpPaste->setEnabled(false);
}
void EditorScene::SelectionChanged(){
	QList<QGraphicsItem*> list =selectedItems();
	bool isOnlyOneItem = (list.count() == 1);
	bool isItemSelected = !list.isEmpty();
	bool isMultiItemSelected = list.count()>1;
	mpAlignTop->setEnabled(isItemSelected);
	mpAlignDown->setEnabled(isItemSelected);
	mpAlignLeft->setEnabled(isItemSelected);
	mpAlignRight->setEnabled(isItemSelected);
	mpAlignCenter->setEnabled(isItemSelected);
	mpQueueTop->setEnabled(isMultiItemSelected);
	mpQueueDown->setEnabled(isMultiItemSelected);
	mpQueueLeft->setEnabled(isMultiItemSelected);
	mpQueueRight->setEnabled(isMultiItemSelected);
	mpScale->setEnabled(isItemSelected);
	mpDelete->setEnabled(isItemSelected);
	mpCopy->setEnabled(isItemSelected);
	mpOrderTop->setEnabled(isOnlyOneItem);
	mpOrderBottom->setEnabled(isOnlyOneItem);
	if(isOnlyOneItem){
		GSItem* pItem = qgraphicsitem_cast<GSItem*>(list[0]);
        sAttrDock.ClearWidget();
        ClearItemSignal();
        sAttrDock.SetWidget(pItem->Editor());
        SetItemSignal(pItem->Signal());
	}else{
		sAttrDock.ClearWidget();
		ClearItemSignal();
	}
}
void EditorScene::SetItemSignal(ItemSignal* pSignal){
	FAIL_RET_VOID(pSignal != mpItemSignal);
	Log::InfoLn("SetItemSignal pSignal=0x%x",pSignal);
	ClearItemSignal();
	mpItemSignal = pSignal;
	bool res = false;
	res = connect(mpItemSignal,&ItemSignal::RectChanged,
		          &sView,&GSView::SelRectChanged); assert(res);
	mpItemSignal->NotifyRectChange();
}
void EditorScene::ClearItemSignal(){
	Log::InfoLn("ClearItemSignal mpItemSignal=0x%x",mpItemSignal);
	if(mpItemSignal){
		mpItemSignal->deleteLater();
		mpItemSignal = NULL;
	}
}
void EditorScene::AlignTop(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemMove(pItem,pItem->pos(),ItemAlign::ToTop(pItem)));
	}
}
void EditorScene::AlignDown(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemMove(pItem,pItem->pos(),ItemAlign::ToBottom(pItem)));
	}
}
void EditorScene::AlignLeft(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemMove(pItem,pItem->pos(),ItemAlign::ToLeft(pItem)));
	}
}
void EditorScene::AlignRight(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemMove(pItem,pItem->pos(),ItemAlign::ToRight(pItem)));
	}
}
void EditorScene::AlignCenter(){
	QList<GSItem*> list = SelectItems();
	QPointF srcCenter = mpInfo->Layer()->boundingRect().center();
	QPointF offset = srcCenter-ItemAlign::Center(list);
	foreach(GSItem* pItem,list){
		PushCmd(new CmdItemMove(pItem,pItem->pos()+offset));
	}
}
void EditorScene::QueueTop(){
	QList<GSItem*> list = SelectItems();
	ItemQueue::QueueItemByTop(list);
	qreal dst = mpInfo->Layer()->boundingRect().top();
	foreach(GSItem* pItem,list){
		PushCmd(new CmdItemMove(pItem,ItemQueue::ToTop(dst,pItem,dst)));
	}
}
void EditorScene::QueueDown(){
	QList<GSItem*> list = SelectItems();
	ItemQueue::QueueItemByBottom(list);
	qreal dst = mpInfo->Layer()->boundingRect().bottom();
	foreach(GSItem* pItem,list){
		PushCmd(new CmdItemMove(pItem,ItemQueue::ToBottom(dst,pItem,dst)));
	}
}
void EditorScene::QueueLeft(){
	QList<GSItem*> list = SelectItems();
	ItemQueue::QueueItemByLeft(list);
	qreal dst = mpInfo->Layer()->boundingRect().left();
	foreach(GSItem* pItem,list){
		PushCmd(new CmdItemMove(pItem,ItemQueue::ToLeft(dst,pItem,dst)));
	}
}
void EditorScene::QueueRight(){
	QList<GSItem*> list = SelectItems();
	ItemQueue::QueueItemByRight(list);
	qreal dst = mpInfo->Layer()->boundingRect().right();
	foreach(GSItem* pItem,list){
		PushCmd(new CmdItemMove(pItem,ItemQueue::ToRight(dst,pItem,dst)));
	}
}
void EditorScene::Scale(){
	ScaleDlg dlg;
	FAIL_RET_VOID(dlg.exec() == QDialog::Accepted);
	qreal scale = dlg.Scale();
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemScale(pItem,scale));
	}
}
void EditorScene::OrderTop(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemOrder(pItem));
		break;
	}
}
void EditorScene::OrderBottom(){
	foreach(GSItem* pItem,SelectItems()){
		GSLayer* pLayer= pItem->Layer();
		PushCmd(new CmdItemOrder(pItem,pLayer->BottomItem()));
		break;
	}
}
void EditorScene::Delete(){
	foreach(GSItem* pItem,SelectItems()){
		PushCmd(new CmdItemRemove(pItem->GetDesc()));
	}
}
void EditorScene::Init(){
	setBackgroundBrush(Qt::gray);
	setSceneRect(-10000,-10000,20000,20000);
    mpUndoStack = new QUndoStack(this);
	mpUndoStack->setUndoLimit(200);
	mpClipboard = new ItemClipboard;
	mpInfo = new GSInfo; AppMod::Register(mpInfo);
	InitAction();
}
QList<GSItem*> EditorScene::SelectItems() const{
	QList<GSItem*> list;
	GSItem* pGSItem = NULL;
	foreach(QGraphicsItem* pItem,selectedItems()){
		pGSItem = qgraphicsitem_cast<GSItem*>(pItem);
		ASSERT_MSG_RET_X(pGSItem,"",QList<GSItem*>());
		list<<pGSItem;
	}
	return list;
}
void EditorScene::Clear(){
	delete mpClipboard;
	delete mpInfo;
}
void EditorScene::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("编辑"),true,true);
	mpAlignTop = actMng.AddAction(QIcon(":/Icon/alignUp.png"),QStringLiteral("对齐上边框"));
	mpAlignDown = actMng.AddAction(QIcon(":/Icon/alignDown.png"),QStringLiteral("对齐下边框"));
	mpAlignLeft = actMng.AddAction(QIcon(":/Icon/alignLeft.png"),QStringLiteral("对齐左边框"));
	mpAlignRight = actMng.AddAction(QIcon(":/Icon/alignRight.png"),QStringLiteral("对齐右边框"));
	mpAlignCenter = actMng.AddAction(QIcon(":/Icon/alignCenter.png"),QStringLiteral("居中对齐"));
	actMng.AddSeparator();
	mpQueueTop = actMng.AddAction(QIcon(":/Icon/queueTop.png"),QStringLiteral("向上排队"));
	mpQueueDown = actMng.AddAction(QIcon(":/Icon/queueDown.png"),QStringLiteral("向下排队"));
	mpQueueLeft = actMng.AddAction(QIcon(":/Icon/queueLeft.png"),QStringLiteral("向左排队"));
	mpQueueRight = actMng.AddAction(QIcon(":/Icon/queueRight.png"),QStringLiteral("向右排队"));
	actMng.AddSeparator();
	mpOrderTop = actMng.AddAction(QIcon(":/Icon/moveTop.png"),QStringLiteral("移到顶层"));
	mpOrderBottom = actMng.AddAction(QIcon(":/Icon/moveBottom.png"),QStringLiteral("移到底层"));
	actMng.AddSeparator();
	mpRedo = actMng.AddAction(QIcon(":/Icon/redo.png"),QStringLiteral("重做"));
	mpUndo = actMng.AddAction(QIcon(":/Icon/undo.png"),QStringLiteral("撤销"));
	actMng.AddSeparator();
	mpScale = actMng.AddAction(QIcon(":/Icon/zoomOut.png"),QStringLiteral("缩放"));
	mpDelete = actMng.AddAction(QIcon(":/Icon/delete.png"),QStringLiteral("删除"));
	actMng.AddSeparator();
	mpCopy = actMng.AddAction(QIcon(":/Icon/copy.png"),QStringLiteral("复制"));
	mpPaste = actMng.AddAction(QIcon(":/Icon/paste.png"),QStringLiteral("黏贴"));
	actMng.EndPart();
	actMng.AddDefShourtcut([=](){
		//默认快捷键
		mpRedo->setShortcuts(QKeySequence::Redo);
		mpUndo->setShortcuts(QKeySequence::Undo);
		mpCopy->setShortcuts(QKeySequence::Copy);
		mpPaste->setShortcuts(QKeySequence::Paste);
		mpDelete->setShortcuts(QKeySequence::Delete);
	});

	mpPaste->setEnabled(false);
}
void EditorScene::BindSignal(){
	bool res = false;
	res = connect(this,SIGNAL(selectionChanged()),SLOT(SelectionChanged())); assert(res);
	res = connect(mpAlignTop,SIGNAL(triggered()),SLOT(AlignTop())); assert(res);
	res = connect(mpAlignDown,SIGNAL(triggered()),SLOT(AlignDown())); assert(res);
	res = connect(mpAlignLeft,SIGNAL(triggered()),SLOT(AlignLeft())); assert(res);
	res = connect(mpAlignRight,SIGNAL(triggered()),SLOT(AlignRight())); assert(res);
	res = connect(mpAlignCenter,SIGNAL(triggered()),SLOT(AlignCenter())); assert(res);
	res = connect(mpQueueTop,SIGNAL(triggered()),SLOT(QueueTop())); assert(res);
	res = connect(mpQueueDown,SIGNAL(triggered()),SLOT(QueueDown())); assert(res);
	res = connect(mpQueueLeft,SIGNAL(triggered()),SLOT(QueueLeft())); assert(res);
	res = connect(mpQueueRight,SIGNAL(triggered()),SLOT(QueueRight())); assert(res);
	res = connect(mpScale,SIGNAL(triggered()),SLOT(Scale())); assert(res);
	res = connect(mpOrderTop,SIGNAL(triggered()),SLOT(OrderTop())); assert(res);
	res = connect(mpOrderBottom,SIGNAL(triggered()),SLOT(OrderBottom())); assert(res);
	res = connect(mpDelete,SIGNAL(triggered()),SLOT(Delete())); assert(res);
	res = connect(mpRedo,SIGNAL(triggered()),SLOT(Redo())); assert(res);
	res = connect(mpUndo,SIGNAL(triggered()),SLOT(Undo())); assert(res);
	res = connect(mpCopy,SIGNAL(triggered()),SLOT(Copy())); assert(res);
	res = connect(mpPaste,SIGNAL(triggered()),SLOT(Paste())); assert(res);
	res = connect(mpUndoStack,SIGNAL(canRedoChanged(bool)),mpRedo,SLOT(setEnabled(bool))); assert(res);
	res = connect(mpUndoStack,SIGNAL(canUndoChanged(bool)),mpUndo,SLOT(setEnabled(bool))); assert(res);
}

