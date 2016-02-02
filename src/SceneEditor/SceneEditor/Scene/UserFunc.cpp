#include "UserFunc.h"
#include "UserFuncMng.h"
#include "AppModAlias.h"

UserFunc::UserFunc(){
}
UserFunc::~UserFunc(){
}
void UserFunc::PushCmd(QUndoCommand* pCmd){
	sFuncMng.PushCmd(pCmd);
}
const QList<GSItem*>& UserFunc::Items() const{
	return sFuncMng.mFuncList;
}
