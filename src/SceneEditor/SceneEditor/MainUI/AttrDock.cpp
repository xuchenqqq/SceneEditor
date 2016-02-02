#include "PreHeader.h"
#include "AttrDock.h"
#include <QVBoxLayout>
#include <QLabel>
#include "Macro.h"

AttrDock::AttrDock(QWidget * parent)
: QDockWidget(QStringLiteral("属性"),parent)
, pBlank(NULL)
{
	Init();
}
AttrDock::~AttrDock(){
}
void AttrDock::SetWidget(QWidget* pWidget){
    FAIL_RET_VOID(pWidget != widget());
    DeleteWidget();
    setWidget(pWidget);
}
void AttrDock::ClearWidget(){
	DeleteWidget();
	setWidget(pBlank);
}
void AttrDock::DeleteWidget(){
	QWidget* pSrc = widget();
	if(pSrc && pSrc!=pBlank){
		pSrc->deleteLater();
	}
}
void AttrDock::Init(){
	pBlank = new QWidget(this);
	QVBoxLayout * pLayout = new QVBoxLayout(pBlank);
	QLabel* pHint = new QLabel(QStringLiteral("<没有指定节点>"),pBlank);
	pHint->setAlignment(Qt::AlignCenter);
	pLayout->addWidget(pHint);
	ClearWidget();
}
