#pragma once

#include <QMetaType>

namespace ItemInfo{
	struct Visible{
		Visible():mState(false){}
		Visible(bool state):mState(state){}
		operator bool(){return mState;}
	private:
		bool mState;
	};
	
	struct UnLock{
		UnLock():mState(false){}
		UnLock(bool state):mState(state){}
		operator bool(){return mState;}
	private:
		bool mState;
	};
}

Q_DECLARE_METATYPE(ItemInfo::Visible)
Q_DECLARE_METATYPE(ItemInfo::UnLock)
