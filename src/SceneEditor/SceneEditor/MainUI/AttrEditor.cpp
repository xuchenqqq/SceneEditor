#include "PreHeader.h"
#include "AttrEditor.h"
#include "Macro.h"
#include "GSItem.h"
#include <assert.h>
#include "CmdItem.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "Guard.h"
#include "SimConfigDlg.h"
#include "GSLayer.h"

#define FORBID_SIGNAL() M_SETUP_GUARD(mIsPermitSignal,false)
#define CHECK_PERMIT() FAIL_RET_VOID(mIsPermitSignal)

AttrEditor::AttrEditor(GSItem* pItem)
: mpItem(pItem),row(0)
{
	ui.setupUi(this);
	BindSignal();
	UpdateUI();
    Init();
}
AttrEditor::~AttrEditor(){
}
void AttrEditor::Init()
{
   if(mpItem->Layer()->IsObject()){
        ui.table->setSelectionMode(QAbstractItemView::SingleSelection);
   }else{
        ui.add->setEnabled(false);
        ui.del->setEnabled(false);
        ui.refresh->setEnabled(false);
   }
}

void AttrEditor::UpdateUI(const QString& item){
	FORBID_SIGNAL();
	#define MACTCH_ITEM(str) (item.isEmpty() || item==str)
	
	if(MACTCH_ITEM("pos")){
		QPointF pos = mpItem->pos();
		ui.xPos->setValue(pos.x());
		ui.yPos->setValue(-pos.y());
	}
	if(MACTCH_ITEM("scale")){
		ui.scale->setValue(mpItem->scale());
	}
	if(MACTCH_ITEM("image")){
        ui.image->setText(mpItem->Name());
	}
	if(MACTCH_ITEM("flipX")){
		ui.flipX->setCheckState(mpItem->FlipX()?Qt::Checked:Qt::Unchecked);
	}
	if(MACTCH_ITEM("flipY")){
		ui.flipY->setCheckState(mpItem->FlipY()?Qt::Checked:Qt::Unchecked);
	}
    if(MACTCH_ITEM("customAttr")){
        int r = 0;
        for(auto it=mpItem->customAttr.begin();it!=mpItem->customAttr.end();it++){
            QString key = it.key();
            QString value = it.value();
            if(!key.isEmpty()){
                ui.table->insertRow(r);
                ui.table->setItem(r,0,new QTableWidgetItem(key));
                ui.table->setItem(r,1,new QTableWidgetItem(value));
                r++;
            }
        }
        IsEdit();
    }
}
void AttrEditor::OnPos(){
	QPointF pos(ui.xPos->value(),-ui.yPos->value());
	sScene.PushCmd(new CmdItemMove(mpItem,pos));
}
void AttrEditor::OnScale(){
	sScene.PushCmd(new CmdItemScale(mpItem,ui.scale->value()));
}
void AttrEditor::OnImage(){
	sScene.PushCmd(new CmdItemImage(mpItem,ui.image->text()));
}
void AttrEditor::OnFlipX(int stat){
	sScene.PushCmd(CmdItemFlip::NewX(mpItem,stat==Qt::Checked));
}
void AttrEditor::OnFlipY(int stat){
	sScene.PushCmd(CmdItemFlip::NewY(mpItem,stat==Qt::Checked));
}
void AttrEditor::BindSignal(){
	bool res = false;
	res = connect(ui.posAct,SIGNAL(clicked()),SLOT(OnPos())); assert(res);
	res = connect(ui.scaleAct,SIGNAL(clicked()),SLOT(OnScale())); assert(res);
	res = connect(ui.image,SIGNAL(ImageDroped()),SLOT(OnImage())); assert(res);
	res = connect(ui.flipX,SIGNAL(stateChanged(int)),SLOT(OnFlipX(int))); assert(res);
	res = connect(ui.flipY,SIGNAL(stateChanged(int)),SLOT(OnFlipY(int))); assert(res);

    res = connect(ui.add,SIGNAL(clicked()),SLOT(AddCustomAttr())); assert(res);
    res = connect(ui.del,SIGNAL(clicked()),SLOT(DelCustomAttr())); assert(res);
    res = connect(ui.refresh,SIGNAL(clicked()),SLOT(RefreshCustomAttr())); assert(res);
}

void AttrEditor::AddCustomAttr()
{
    SimConfigDlg dlg;
    QString mCmdLine;
    if(dlg.exec() == QDialog::Accepted){
        mCmdLine = dlg.CmdLine();
        if(!mCmdLine.isEmpty()){
            ui.table->insertRow(row);
            ui.table->setItem(row,0,new QTableWidgetItem(mCmdLine));
            auto item = new QTableWidgetItem();
            ui.table->setItem(row,1,item);
            IsEdit();
            row ++;
        }
    }
}

void AttrEditor::DelCustomAttr()
{
   QList<QTableWidgetItem*> items = ui.table->selectedItems();
   int count = items.count();
   for(int i=0; i<count;i++)
   {
       int delrow = ui.table->row(items.at(i));
       ui.table->removeRow(delrow);
       row--;
   }
}

void AttrEditor::IsEdit()
{
    int col = 0;
    for(int i=0; i<ui.table->rowCount(); i++)
    {
        auto item = ui.table->item(i,col);
        if(item){
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
    }
}

void AttrEditor::RefreshCustomAttr()
{
    QMap<QString, QString> customAttr;
    for(int r=0; r<row; r++){
        auto keyItem = ui.table->item(r,0);
        auto  valueItem = ui.table->item(r,1);
        if(!keyItem->text().isEmpty())
        {
           customAttr.insert(keyItem->text(),valueItem->text());
        }
    }
    mpItem->SetCustomAttr(customAttr);
}
