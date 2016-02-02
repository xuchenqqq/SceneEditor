#ifndef APP_MOD_H
#define APP_MOD_H

#include "ModAccess.h"

class AppMod
{
public:
	template<typename T>
	static inline void Register(T* mod){
		Access().Set(*mod);
	}
	template<typename T>
	static inline void Register(T& mod){
		Access().Set(mod);
	}
	template<typename T>
	static inline T& Get(){
		return Access().Get<T>();
	}
private:
	static ModAccess& Access();
};

#endif