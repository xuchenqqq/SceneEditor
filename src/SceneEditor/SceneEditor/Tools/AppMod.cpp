#include "PreHeader.h"
#include "AppMod.h"

ModAccess& AppMod::Access(){
	static ModAccess _inst;
	return _inst;
}
