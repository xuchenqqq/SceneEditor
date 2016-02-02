#include "PreHeader.h"
#include "KeySetDlg.h"
#include <assert.h>
#include "KeyTable.h"
#include "Macro.h"

KeySetDlg::KeySetDlg(QWidget* parent,QAction* pAct)
: QDialog(parent)
, mpAct(pAct)
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
KeySetDlg::~KeySetDlg(){

}
void KeySetDlg::Save(){
	QString keySeq;
	if(ui.ctrl->checkState() == Qt::Checked){
		keySeq.append("Ctrl");
	}
	if(ui.alt->checkState() == Qt::Checked){
		if(!keySeq.isEmpty())
			keySeq.append("+");
		keySeq.append("Alt");
	}
	if(ui.shift->checkState() == Qt::Checked){
		if(!keySeq.isEmpty())
			keySeq.append("+");
		keySeq.append("Shift");
	}
	if(ui.key->currentIndex()!=-1 && 
	   ui.key->currentText() != sKeyTable[0]){
		   if(!keySeq.isEmpty())
			   keySeq.append("+");
		keySeq.append(ui.key->currentText());
	}
	if(!keySeq.isEmpty()){
		mpAct->setShortcut(QKeySequence(keySeq,QKeySequence::PortableText));
	}
	accept();
}
void KeySetDlg::BindSignal(){
	bool res = false;
	res = connect(ui.save,SIGNAL(clicked()),SLOT(Save())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(reject())); assert(res);
}
void KeySetDlg::Init(){
	SetupKeyTable();
	ui.name->setText(mpAct->text());
	QStringList keys = mpAct->shortcut().toString().split("+");
	int fixKeyNum = 0;
	if(keys.contains("Ctrl")){
		ui.ctrl->setCheckState(Qt::Checked);
		++fixKeyNum;
	}
	if(keys.contains("Alt")){
		ui.alt->setCheckState(Qt::Checked);
		++fixKeyNum;
	}
	if(keys.contains("Shift")){
		ui.shift->setCheckState(Qt::Checked);
		++fixKeyNum;
	}
	if(fixKeyNum<keys.size()){
		ui.key->setCurrentText(keys.last());
	}else{
		ui.key->setCurrentText(sKeyTable[0]);
	}
}
void KeySetDlg::SetupKeyTable(){
	QStringList strList;
	for(int i = 0; i < LEN_OF_ARRAY(sKeyTable); ++i){
		strList<<sKeyTable[i];
	}
	ui.key->addItems(strList);
}
