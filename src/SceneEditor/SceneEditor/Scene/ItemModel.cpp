#include "PreHeader.h"
#include "ItemModel.h"
#include "Macro.h"
#include <assert.h>
#include "CmdItem.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "RtUID.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "ItemInfo.h"

using namespace ItemInfo;

enum{
	COL_VISIBLE=0,
	COL_UNLOCK,
	COL_NAME,
	COL_NUM,
};

ItemModel::ItemModel(QObject* parent)
: QAbstractTableModel(parent)
, mpLayer(NULL)
{}
ItemModel::~ItemModel(){
}
int ItemModel::rowCount(const QModelIndex& )const{
	return Count();
}
int ItemModel::columnCount(const QModelIndex& )const{
	return COL_NUM;
}
QVariant ItemModel::data(const QModelIndex& idx,int role)const{
	//只处理Qt::DisplayRole 和 Qt::EditRole
	if(role == Qt::DisplayRole || role == Qt::EditRole){
		GSItem* pItem = mList[idx.row()];
		switch(idx.column()){
			case COL_VISIBLE: return QVariant::fromValue(Visible(pItem->isVisible()));
			case COL_UNLOCK:  return QVariant::fromValue(UnLock(pItem->isEnabled()));
			case COL_NAME:    return pItem->Name();
		default: break;
		}
	}
	return QVariant();
}
bool ItemModel::setData(const QModelIndex& idx, const QVariant& var,int role){
	//只处理Qt::DisplayRole 和 Qt::EditRole
	if(role==Qt::DisplayRole || role==Qt::EditRole){
		GSItem* pItem = mList[idx.row()];
		switch(idx.column()){
			case COL_VISIBLE:  pItem->setVisible(var.toBool()); return true;
			case COL_UNLOCK:  pItem->setEnabled(var.toBool()); return true;
			case COL_NAME:    pItem->SetName(var.toString()); return true;
		default: break;
		}
	}
	return QAbstractTableModel::setData(idx,var,role);
}
Qt::ItemFlags ItemModel::flags(const QModelIndex& idx) const{
    Qt::ItemFlags oldFlags = QAbstractTableModel::flags(idx);
    return Qt::ItemFlags(oldFlags|Qt::EditRole|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}
QMimeData* ItemModel::mimeData(const QModelIndexList& indexes) const{
	FAIL_RET_X(indexes.count()>0,0);
    QStringList types = mimeTypes();
	FAIL_RET_X(!types.isEmpty(),0);
    QMimeData *data = new QMimeData();
    QString format = types.at(0);
    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
	QSet<int> rowSet;
	foreach(const QModelIndex& idx,indexes){
		rowSet.insert(idx.row());
	}
	stream<<rowSet.toList();
	data->setData(format,encoded);
	return data;
}
QStringList	ItemModel::mimeTypes() const{
	QStringList types;
    types << "application/ItemRowNum";
    return types;
}
bool ItemModel::dropMimeData(const QMimeData* data,Qt::DropAction action,int,int,const QModelIndex& parent){
	FAIL_RET(data);
	FAIL_RET(action==Qt::MoveAction);
    QStringList types = mimeTypes();
    FAIL_RET(!types.isEmpty());
    QString format = types.at(0);
    FAIL_RET(data->hasFormat(format));
	GSItem* pPostItem = NULL;
	int dstRow = parent.row();
	FAIL_RET(dstRow>=0 && dstRow < Count())
	pPostItem = mList[dstRow];
    QByteArray byteArray = data->data(format);
    QDataStream stream(&byteArray,QIODevice::ReadOnly);
	QList<int> rowList;
	stream>>rowList;
	FAIL_RET(!rowList.contains(dstRow));
	foreach(int srcRow,rowList){
		if(srcRow+1==dstRow) continue;
		sScene.PushCmd(new CmdItemOrder(mList[srcRow],pPostItem));
	}
	return true;
}
GSItem* ItemModel::Item(int idx) const{
	FAIL_RET_X(idx>=0 || idx<mList.size(),NULL);
	return mList[idx];
}
Qt::DropActions ItemModel::supportedDropActions() const{
	return Qt::MoveAction;
}
void ItemModel::SetLayer(GSLayer* pLayer){
	FAIL_RET_VOID(mpLayer!=pLayer);
	beginResetModel();
	if(mpLayer){
		mpLayer->disconnect(this);
		mList.clear();
	}
	mpLayer = pLayer;
	if(mpLayer){
		mList = mpLayer->ItemList();
		bool res = false;
		res = connect(mpLayer,SIGNAL(ItemInserted(GSItem*,GSItem*)),
					  SLOT(Inserted(GSItem*,GSItem*))); assert(res);
		res = connect(mpLayer,SIGNAL(ItemRemoved(GSItem*)),
					  SLOT(Removed(GSItem*))); assert(res);
		res = connect(mpLayer,SIGNAL(ItemOrdered(GSItem*,GSItem*)),
					  SLOT(Moved(GSItem*,GSItem*))); assert(res);
	}
	endResetModel();
}
void ItemModel::Inserted(GSItem* pItem,GSItem* pPostItem){
	int dstRow = pPostItem?mList.indexOf(pPostItem):mList.size(); 
	beginInsertRows(QModelIndex(),dstRow,dstRow);
		mList.insert(dstRow,pItem);
	endInsertRows();
	CheckSameWithScene();
}
void ItemModel::Removed(GSItem* pItem){
	int dstRow = mList.indexOf(pItem);
	beginRemoveRows(QModelIndex(),dstRow,dstRow);
		mList.removeAt(dstRow);
	endInsertRows();
	CheckSameWithScene();
}
void ItemModel::Moved(GSItem* pItem,GSItem* pPostItem){
	int srcRow = mList.indexOf(pItem);
	int dstRow = pPostItem?mList.indexOf(pPostItem):mList.size();
	beginMoveRows(QModelIndex(),srcRow,srcRow,QModelIndex(),dstRow);
		if(dstRow<srcRow){
			mList.move(srcRow,dstRow);
		}else{
			mList.move(srcRow,dstRow-1);
		}
	endMoveRows();
	CheckSameWithScene();
}
void ItemModel::CheckSameWithScene(){
	ASSERT_MSG(mpLayer->ItemList()==mList,"Scene中的次序必须和Dock中次序一致");
}


