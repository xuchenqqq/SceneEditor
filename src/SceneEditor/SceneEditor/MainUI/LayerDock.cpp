#include "PreHeader.h"
#include "LayerDock.h"
#include "Macro.h"
#include <assert.h>
#include "GSLayer.h"
#include "RtUID.h"
#include "CmdLayer.h"
#include "GSScreen.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "LayerModel.h"
#include "ItemDelegate.h"
#include "GSInfo.h"
#include <QHeaderView>

LayerDock::LayerDock(QWidget* parent)
: QDockWidget(parent)
, mpModel(new LayerModel(this))
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
LayerDock::~LayerDock(){

}
void LayerDock::OnScreenChanged(GSScreen* pScreen){
	sGSInfo.Set(pScreen);
	SetCurrentRow(-1);
	mpModel->SetScreen(pScreen);
	if(RowCount() > 0){
		SetCurrentRow(0);
	}
}
void LayerDock::Add(){
	mpModel->Add();
	if(RowCount()==1){
		SetCurrentRow(0);
	}
}
void LayerDock::AddObject()
{
    mpModel->AddObject();
    if(RowCount()==1){
        SetCurrentRow(0);
    }
}

void LayerDock::Remove(){
	int curRow = CurrentRow();
	SelectOtherRow();
	mpModel->Remove(curRow);
}
void LayerDock::RowChanged(const QModelIndex& cur,const QModelIndex& pre){
	int curRow = cur.row();
	int preRow = pre.row();
	if(preRow >= 0){
		mpModel->Layer(preRow)->Lock();
	}
	GSLayer* pLayer = NULL;
	if(curRow >= 0){
		pLayer = mpModel->Layer(curRow);
		pLayer->UnLock();
	}
	sGSInfo.Set(pLayer);
	emit LayerChanged(pLayer);
}
void LayerDock::SelectOtherRow(){
	int cnt = RowCount();
	FAIL_RET_VOID(cnt>0);
	int curRow = RowCount();
	FAIL_RET_VOID(curRow>=0);
	if(curRow==cnt-1){
		SetCurrentRow(curRow-1);
	}else{
		SetCurrentRow(curRow+1);
	}
}
int  LayerDock::CurrentRow() const{
	QItemSelectionModel* pSelection = ui.table->selectionModel();
	return pSelection->currentIndex().row();
}
void LayerDock::SetCurrentRow(int row){
	QItemSelectionModel* pSelection = ui.table->selectionModel();
	pSelection->setCurrentIndex(mpModel->index(row,0),QItemSelectionModel::Current);
}
int  LayerDock::RowCount() const{
	return mpModel->Count();
}
void LayerDock::Init(){
	ui.table->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.table->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.table->setAutoScroll(true);
	ui.table->setDragEnabled(true);
	ui.table->setDragDropMode(QAbstractItemView::InternalMove);
	ui.table->viewport()->setAcceptDrops(true);
	ui.table->setModel(mpModel);
	ui.table->setShowGrid(false);
	ui.table->setDragDropOverwriteMode(false);
	//ui.table->setEditTriggers(QAbstractItemView::CurrentChanged);
	QHeaderView* hHeader =ui.table->horizontalHeader();
	hHeader->resizeSection(0,50);
	hHeader->resizeSection(1,100);
    hHeader->resizeSection(2,80);
    hHeader->resizeSection(3,45);
	hHeader->hide();
	ui.table->verticalHeader()->hide();
	QAbstractItemDelegate* pDefault = ui.table->itemDelegate();
	ui.table->setItemDelegate(new ItemDelegate(this));
	pDefault->deleteLater();
}
void LayerDock::BindSignal(){
	bool res = false;
	res = connect(ui.del,SIGNAL(clicked()),SLOT(Remove())); assert(res);
	res = connect(ui.add,SIGNAL(clicked()),SLOT(Add())); assert(res);
    res = connect(ui.addObject,SIGNAL(clicked()),SLOT(AddObject()));assert(res);
	res = connect(ui.table->selectionModel(),&QItemSelectionModel::currentRowChanged,
		                  this,&LayerDock::RowChanged); assert(res);
}

