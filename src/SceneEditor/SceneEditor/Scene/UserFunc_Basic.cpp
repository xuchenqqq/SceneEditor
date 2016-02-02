#include "UserFunc_Baisc.h"
#include <functional>
#include "UserFuncMng.h"
#include "AppModAlias.h"

UseFunc_Basic::UseFunc_Basic(){
	InitAction();
	BindSignal();
}
UseFunc_Basic::~UseFunc_Basic(){
}
void UseFunc_Basic::OnItemChanged(){
	if(Items().isEmpty()){
		mpDelete->setEnabled(false);
		mpCopy->setEnabled(false);
	}else{
		mpDelete->setEnabled(true);
		mpCopy->setEnabled(true);
	}
}
void UseFunc_Basic::OnDelete(){
	foreach(GSItem* pItem,Items()){
		PushCmd(new CmdItemRemove(pItem->GetDesc()));
	}
}
void UseFunc_Basic::OnCopy(){
	FAIL_RET_VOID(mpInfo->Layer());
	mpClipboard->Copy(Items());
	mpPaste->setEnabled(true);
}
void UseFunc_Basic::OnPaste(){
	GSLayer* pLayer = mpInfo->Layer();
	FAIL_RET_VOID(pLayer);
	mpClipboard->Paste(mpInfo->LayerMousePos(),pLayer->ID());
	mpPaste->setEnabled(false);
}
void UseFunc_Basic::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("编辑"),true,true);
	mpDelete = actMng.AddAction(QIcon(":/Icon/delete.png"),QStringLiteral("删除"));
	mpCopy = actMng.AddAction(QIcon(":/Icon/copy.png"),QStringLiteral("复制"));
	mpPaste = actMng.AddAction(QIcon(":/Icon/paste.png"),QStringLiteral("粘贴"));
	actMng.EndPart();
	actMng.AddDefShourtcut([=](){
		//默认快捷键
		mpCopy->setShortcuts(QKeySequence::Copy);
		mpPaste->setShortcuts(QKeySequence::Paste);
		mpDelete->setShortcuts(QKeySequence::Delete);
	});
	mpPaste->setEnabled(false);
}
void UseFunc_Basic::BindSignal(){
	bool res = false;
	res = connect(mpDelete,&QAction::triggered,
	              std::bind(this,&UseFunc_Basic::OnDelete)); assert(res);
	res = connect(mpCopy,&QAction::triggered,
	              std::bind(this,&UseFunc_Basic::OnCopy)); assert(res);
	res = connect(mpPaste,&QAction::triggered,
	              std::bind(this,&UseFunc_Basic::OnPaste)); assert(res);
}