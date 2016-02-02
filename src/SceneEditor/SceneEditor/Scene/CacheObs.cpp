#include "PreHeader.h"
#include "CacheObs.h"
#include "Macro.h"
#include "Cache.h"
#include "AppModAlias.h"

CacheObs::CacheObs(const Func& func)
: mFunc(func)
{}
CacheObs::~CacheObs(){
	UnBind();
}
void CacheObs::Set(const Func& func){
	mFunc = func;
}
void CacheObs::Update(){
	mFunc();
}
void CacheObs::Bind(const QString& name){
	FAIL_RET_VOID(mName!=name);
	UnBind();
	mName = name;
	sCache.Bind(mName,this);
}
void CacheObs::UnBind(){
	FAIL_RET_VOID(!mName.isEmpty());
	sCache.UnBind(mName,this);
}