#include "PreHeader.h"
#include "GSInfo.h"
#include "GSScreen.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "Macro.h"

void GSInfo::SetSceneMousePos(const QPointF& pos){
	mSceneMousePos=pos;
}
QPointF  GSInfo::ScreenMousePos() const{
	FAIL_RET_X(mpScreen,QPointF());
	return mpScreen->mapFromScene(mSceneMousePos);
}
QPointF  GSInfo::LayerMousePos() const{
	FAIL_RET_X(mpLayer,QPointF());
	return mpLayer->mapFromScene(mSceneMousePos);
}
QPointF  GSInfo::ItemMousePos() const{
	FAIL_RET_X(mpItem,QPointF());
	return mpItem->mapFromScene(mSceneMousePos);
}
