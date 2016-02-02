#include "PreHeader.h"
#include "ModAccess.h"
#include "Macro.h"

ModAccess::ModAccess()
{
	mModList.reserve(10);
}
void* ModAccess::Get(const std::string & name) const
{
	void* ptr = Find(name);
	assert(ptr);
	return ptr;
}
void ModAccess::Set(const std::string & name,void* ptr)
{
	//输入参数无效
	assert(ptr);
	FAIL_RET_VOID(ptr);
	void* isExisted = Find(name);
	//防止重复添加
	assert(isExisted == NULL);
	FAIL_RET_VOID(isExisted == NULL);
	//增加数据项
	mModList.push_back(ModItem(name,ptr));
}
void* ModAccess::Find(const std::string & name) const
{
	for(int i = 0;i < (int)mModList.size(); ++i)
	{
		if(mModList[i].first == name)
		{
			return mModList[i].second;
		}
	}
	return NULL;
}
