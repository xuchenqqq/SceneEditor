#include "PreHeader.h"
#include "ScreenDock.h"
#include <assert.h>
#include <QListWidgetItem>
#include "Macro.h"
#include "ScreenWidget.h"
#include "GSScreen.h"
#include "RtUID.h"
#include <QGraphicsScene>

enum{
	MAX_SCREEN_NUM = 10,
	MIN_SCREEN_WIDTH = 10,
	MAN_SCREEN_WIDTH = 8000,
	MIN_SCREEN_HEIGHT = 10,
	MAN_SCREEN_HEIGHT = 8000,
};

ScreenDock::ScreenDock(QWidget* parent)
: QDockWidget(parent)
, mpPreScreen(NULL)
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
ScreenDock::~ScreenDock(){

}
void ScreenDock::ScreenUpdated(const QList<GSScreen*>& list){
	Clear();
	foreach(GSScreen* pScreen,list){
		AddScreen(pScreen);
	}
	if(ui.list->count() > 0){
		ui.list->setCurrentRow(0);
	}
}
void ScreenDock::OnRowChanged(int row){
	if(mpPreScreen){
		mpPreScreen->setEnabled(false);
	}
	if(row >= 0){
		GSScreen* pScreen = Screen(row);
		pScreen->setEnabled(true);
		mpPreScreen = pScreen;
		emit ScreenChanged(pScreen);
		emit ScreenRectChanged(pScreen->RectBaseScene());
	}else{
		mpPreScreen = NULL;
		emit ScreenChanged(NULL);
	}
}
ScreenWidget* ScreenDock::Widget(int idx){
	return qobject_cast<ScreenWidget*>(ui.list->itemWidget(ui.list->item(idx)));
}
GSScreen* ScreenDock::Screen(int idx){
	return Widget(idx)->Screen();
}
void ScreenDock::AddScreen(GSScreen* pScreen){
	pScreen->setEnabled(false);
	//2. ScreenWidget
	int idx = ui.list->count();
	QString title = QString(QStringLiteral("屏幕 "))+QString::number(idx+1);
	ScreenWidget* pWidget= new ScreenWidget(pScreen,title);
	//3. WidgetItem
	QListWidgetItem* pItem = new QListWidgetItem(ui.list);
	pItem->setSizeHint(pWidget->sizeHint());
	ui.list->addItem(pItem);
	ui.list->setItemWidget(pItem,pWidget);
}
void ScreenDock::RemoveScreen(int idx){
	//2. ScreenWidget
	QListWidgetItem* pItem = ui.list->item(idx);
	ui.list->removeItemWidget(pItem);
	//3. WidgetItem
	ui.list->takeItem(idx);
	delete pItem;
}
void ScreenDock::Clear(){
	ui.list->setCurrentRow(-1);
	while(ui.list->count() > 0){
		RemoveScreen(0);
	}
}
void ScreenDock::Init(){
	ui.list->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.list->setResizeMode(QListView::Fixed);
	ui.list->setAutoScroll(true);
	ui.list->setUniformItemSizes(true);
}
void ScreenDock::BindSignal(){
	bool res = false;
	res = connect(ui.list,SIGNAL(currentRowChanged(int)),
	              SLOT(OnRowChanged(int))); assert(res);
}

