#include "PreHeader.h"
#include "ScreenWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <assert.h>
#include "GSScreen.h"

ScreenWidget::ScreenWidget(GSScreen* pScreen,const QString& name)
: mpScreen(pScreen)
{
	ui.setupUi(this);
	InitUI(name);
}
ScreenWidget::~ScreenWidget(){
}
void ScreenWidget::OnVisibleToggle(bool state){
	SetVisibleBtn(state);
	mpScreen->setVisible(state);
}
void ScreenWidget::InitUI(const QString& name){
	ui.name->setText(name);
	SetVisibleBtn(mpScreen->isVisible());
	bool res = false;
	res = connect(ui.visible,SIGNAL(toggled(bool)),SLOT(OnVisibleToggle(bool))); assert(res);
}
void ScreenWidget::SetVisibleBtn(bool state){
	ui.visible->setChecked(state);
	ui.visible->setIcon(state?QIcon(":/Icon/visible.png"):QIcon(":/Icon/invisible.png"));
}
