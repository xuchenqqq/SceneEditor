#include "PreHeader.h"
#include "LayerModel.h"
#include "ItemInfo.h"
#include "GSLayer.h"
#include "GSScreen.h"
#include "Macro.h"
#include <assert.h>
#include "CmdLayer.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "RtUID.h"

using namespace ItemInfo;

enum{
	COL_VISIBLE=0,
	COL_NAME,
	COL_SPEED,
    COL_OBJECTLAYER,
	COL_NUM,
};

LayerModel::LayerModel(QObject* parent)
: QAbstractTableModel(parent)
, mpScreen(NULL)
{}
LayerModel::~LayerModel(){
}
int LayerModel::rowCount(const QModelIndex&)const{
	return mList.size();
}
int LayerModel::columnCount(const QModelIndex&)const{
	return COL_NUM;
}
QVariant LayerModel::data(const QModelIndex& idx,int role)const{
	//只处理Qt::DisplayRole 和 Qt::EditRole
	if(role == Qt::DisplayRole || role == Qt::EditRole){
		GSLayer* pLayer = mList[idx.row()];
		switch(idx.column()){
			case COL_VISIBLE: return QVariant::fromValue(Visible(pLayer->isVisible()));
			case COL_NAME:    return pLayer->Name();
			case COL_SPEED:   return pLayer->Speed();
            case COL_OBJECTLAYER:  return pLayer->IsObjectLayer();
		default: break;
		}
	}
	return QVariant();
}
bool LayerModel::setData(const QModelIndex& idx,const QVariant& var,int role){
	//只处理Qt::DisplayRole 和 Qt::EditRole
	if(role==Qt::DisplayRole || role==Qt::EditRole){
		GSLayer* pLayer = mList[idx.row()];
		switch(idx.column()){
			case COL_VISIBLE:  pLayer->setVisible(var.toBool()); return true;
			case COL_NAME:    pLayer->SetName(var.toString()); return true;
			case COL_SPEED:    pLayer->SetSpeed(var.toDouble()); return true;
		default: break;
		}
	}
	return QAbstractTableModel::setData(idx,var,role);
}
Qt::ItemFlags LayerModel::flags(const QModelIndex& idx) const{
    Qt::ItemFlags oldFlags = QAbstractTableModel::flags(idx);
    return Qt::ItemFlags(oldFlags|Qt::EditRole|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}
void LayerModel::SetScreen(GSScreen* pScreen){
	FAIL_RET_VOID(mpScreen!=pScreen);
	beginResetModel();
	if(mpScreen){
		mpScreen->disconnect(this);
		mList.clear();
	}
	mpScreen = pScreen;
	if(mpScreen){
		mList = mpScreen->LayerList();
		foreach(GSLayer* pLayer,mList){
			pLayer->Lock();
		}
		bool res = false;
		res = connect(mpScreen,SIGNAL(LayerInserted(GSLayer*,GSLayer*)),
					  SLOT(Inserted(GSLayer*,GSLayer*))); assert(res);
		res = connect(mpScreen,SIGNAL(LayerRemoved(GSLayer*)),
					  SLOT(Removed(GSLayer*))); assert(res);
		res = connect(mpScreen,SIGNAL(LayerOrdered(GSLayer*,GSLayer*)),
					  SLOT(Moved(GSLayer*,GSLayer*))); assert(res);
	}
	endResetModel();
}
QMimeData* LayerModel::mimeData(const QModelIndexList& indexes) const{
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
QStringList	LayerModel::mimeTypes() const{
	QStringList types;
    types << "application/LayerRowNum";
    return types;
}
Qt::DropActions LayerModel::supportedDropActions() const{
	return Qt::MoveAction;
}
bool LayerModel::dropMimeData(const QMimeData* data,Qt::DropAction action,int,int,const QModelIndex& parent){
	FAIL_RET(data);
	FAIL_RET(action==Qt::MoveAction);
    QStringList types = mimeTypes();
    FAIL_RET(!types.isEmpty());
    QString format = types.at(0);
    FAIL_RET(data->hasFormat(format));
	GSLayer* pPostLayer = NULL;
	int dstRow = parent.row();
	FAIL_RET(dstRow>=0 && dstRow < Count())
	pPostLayer = mList[dstRow];
    QByteArray byteArray =data->data(format);
    QDataStream stream(&byteArray,QIODevice::ReadOnly);
	QList<int> rowList;
	stream>>rowList;
	FAIL_RET(!rowList.contains(dstRow));
	foreach(int srcRow,rowList){
		if(srcRow+1==dstRow) continue;
		sScene.PushCmd(new CmdLayerOrder(mList[srcRow],pPostLayer));
	}
	return true;
}
GSLayer* LayerModel::Layer(int idx) const{
	FAIL_RET_X(idx>=0 || idx<mList.size(),NULL);
	return mList[idx];
}
void LayerModel::Inserted(GSLayer* pLayer,GSLayer* pPostLayer){
	int dstRow = pPostLayer?mList.indexOf(pPostLayer):mList.size(); 
	beginInsertRows(QModelIndex(),dstRow,dstRow);
		mList.insert(dstRow,pLayer);
	endInsertRows();
	CheckSameWithScene();
}
void LayerModel::Removed(GSLayer* pLayer){
	int dstRow = mList.indexOf(pLayer);
	beginRemoveRows(QModelIndex(),dstRow,dstRow);
		mList.removeAt(dstRow);
	endInsertRows();
	CheckSameWithScene();
}
void LayerModel::Moved(GSLayer* pLayer,GSLayer* pPostLayer){
	int srcRow = mList.indexOf(pLayer);
	int dstRow = pPostLayer?mList.indexOf(pPostLayer):mList.size();
	beginMoveRows(QModelIndex(),srcRow,srcRow,QModelIndex(),dstRow);
	if(dstRow<srcRow){
		mList.move(srcRow,dstRow);
	}else{
		mList.move(srcRow,dstRow-1);
	}
	endMoveRows();
	CheckSameWithScene();
}
void LayerModel::Remove(int idx){
	FAIL_RET_VOID(mpScreen);
	FAIL_RET_VOID(idx>=0 && idx <mList.size());
	GSLayer* pLayer = mList[idx];
	sScene.PushCmd(new CmdLayerRemove(pLayer->GetDesc()));
}
void LayerModel::Insert(int idx){
	FAIL_RET_VOID(mpScreen);
	FAIL_RET_VOID(idx>=0 && idx <mList.size());
    GSLayer::Desc desc(RtUID::Create(),mpScreen->ID(),mList[idx]->ID());
    sScene.PushCmd(new CmdLayerInsert(desc));
}
void LayerModel::Add(){
	FAIL_RET_VOID(mpScreen);
    GSLayer::Desc desc(RtUID::Create(),mpScreen->ID(),RtUID::Null,1,"No Name",false);
    sScene.PushCmd(new CmdLayerInsert(desc));
}
void LayerModel::AddObject(){
    FAIL_RET_VOID(mpScreen);
    GSLayer::Desc desc(RtUID::Create(),mpScreen->ID(),RtUID::Null,1,"object",true);
    sScene.PushCmd(new CmdLayerInsert(desc));
}

void LayerModel::CheckSameWithScene(){
	ASSERT_MSG(mpScreen->LayerList()==mList,"Scene中的次序必须和Dock中次序一致");
}


