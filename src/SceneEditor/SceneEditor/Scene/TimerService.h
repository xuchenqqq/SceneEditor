#pragma once

#include <functional>
#include <QObject>
#include "ModDeclare.h"
#include <QMap>

class TimerService : QObject
{
Q_OBJECT
public:
	typedef std::function<void()> Func;
	TimerService(QObject* parent);
	~TimerService();
	int Add(int ms,const Func& func);
	void Cancel(int id);
private:
	void timerEvent(QTimerEvent * event);
private:
	typedef QMap<int,Func> Map;
	typedef Map::iterator MapIt;
	Map mMap;
};
M_DECL_MOD(TimerService)

