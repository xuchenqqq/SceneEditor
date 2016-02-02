#include "PreHeader.h"
#include "RtUID.h"

namespace RtUID
{
	static Type sNextUID = 1;
	Type Create(){
		return sNextUID++;
	}
};
