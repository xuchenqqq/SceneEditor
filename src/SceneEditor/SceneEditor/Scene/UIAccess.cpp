#include "PreHeader.h"
#include "UIAccess.h"
#include "Macro.h"
#include <assert.h>

QWidget* UIAccess::Ptr(const QString& name) const{
	QWidget* pWidget = mMap.value(name,NULL);
	ASSERT_MSG(pWidget,"不存在这个名字的控件");
	return pWidget;
}
void UIAccess::Set(const QString& name,QWidget* pWidget){
	ASSERT_MSG_RET_VOID(!mMap.contains(name),"添加相同名字的控件");
	mMap.insert(name,pWidget);
}
