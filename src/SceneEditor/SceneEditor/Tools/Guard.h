#pragma once

template<typename T>
class Guard
{
public:
	Guard(T& ref,T val)
	: mRef(ref)
	, mOldVal(ref)
	{
		mRef = val;
	}
	~Guard(){
		mRef = mOldVal;
	}
private:
	T& mRef;
	T  mOldVal;
};

#define M_SETUP_GUARD(var,val) Guard<decltype(var)> __Guard__(var,val);
