#ifndef MOD_ACCESS_H
#define MOD_ACCESS_H

#include <string>
#include <assert.h>
#include <utility>
#include <vector>
#include "ModDeclare.h"

class ModAccess
{
public:
	ModAccess();
	template<typename T>
	inline void Set(T& mod)
	{
		Set(__ModName__<T>(),&mod);
	}
	template<typename T>
	inline T& Get() const
	{
		return *((T*)Get(__ModName__<T>()));
	}
private:
	void* Get(const std::string & name) const;
	void  Set(const std::string & name,void* ptr);
	void* Find(const std::string & name) const;
private:
	ModAccess(const ModAccess&);
	ModAccess& operator=(const ModAccess&);
private:
	typedef std::pair<std::string,void*> ModItem;
	typedef std::vector<ModItem> ModList;
	ModList mModList;
};

#endif