#pragma once

#include <functional>
#include <QString>

class CacheObs
{
public:
	typedef std::function<void()> Func;
public:
	CacheObs(const Func& func);
	~CacheObs();
	void Set(const Func& func);
	void Update();
	void Bind(const QString& name);
	void UnBind();
private:
	Func mFunc;
	QString mName;
};
