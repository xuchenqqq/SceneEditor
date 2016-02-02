#include "PreHeader.h"
#include "ItemDock.h"
#include "Macro.h"
#include <assert.h>
#include "ItemModel.h"
#include "ItemDelegate.h"
#include "GSItem.h"

ItemDock::ItemDock(QWidget* parent)
: QDockWidget(parent)
, mpModel(new ItemModel(this))
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
ItemDock::~ItemDock(){
}
void ItemDock::OnLayerChanged(GSLayer* pLayer){
	mpModel->SetLayer(pLayer);
}
void ItemDock::SelectionChanged(const QItemSelection& ,const QItemSelection&){
	emit ClearSelection();
	QVector<int> rows;
	QItemSelectionModel* pSelection = ui.table->selectionModel();
	foreach(const QModelIndex& idx,pSelection->selectedRows()){
		rows.append(idx.row());
	}
	foreach(int row,rows){
		mpModel->Item(row)->setSelected(true);
	}
}
void ItemDock::Init(){
	ui.table->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui.table->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.table->setAutoScroll(true);
	ui.table->setDragEnabled(true);
	ui.table->setDragDropMode(QAbstractItemView::InternalMove);
	ui.table->viewport()->setAcceptDrops(true);
	ui.table->setModel(mpModel);
	ui.table->setShowGrid(false);
	ui.table->setDragDropOverwriteMode(false);
	QHeaderView* hHeader =ui.table->horizontalHeader();
	hHeader->resizeSection(0,50);
	hHeader->resizeSection(1,50);
	hHeader->resizeSection(2,150);
	hHeader->hide();
	ui.table->verticalHeader()->hide();
	QAbstractItemDelegate* pDefault = ui.table->itemDelegate();
	ui.table->setItemDelegate(new ItemDelegate(this));
	pDefault->deleteLater();
}
void ItemDock::BindSignal(){
	bool res = false;
	res = connect(ui.table->selectionModel(),&QItemSelectionModel::selectionChanged,
		          this,&ItemDock::SelectionChanged); assert(res);
}



