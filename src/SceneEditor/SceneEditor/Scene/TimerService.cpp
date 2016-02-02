#include "PreHeader.h"
#include "TimerService.h"

TimerService::TimerService(QObject* parent)
: QObject(parent)
{}
TimerService::~TimerService(){
}
int TimerService::Add(int ms,const Func& func){
	int timerId = startTimer(ms);
	if(timerId){
		mMap.insert(timerId,func);
	}
	return timerId;
}
void TimerService::Cancel(int id){
	if(mMap.contains(id)){
		killTimer(id);
		mMap.remove(id);
	}
}
void TimerService::timerEvent(QTimerEvent* event){
	MapIt it = mMap.find(event->timerId());
	if(it != mMap.end()){
		killTimer(event->timerId());
		it.value()();
		mMap.erase(it);
	}else{
		QObject::timerEvent(event);
	}
}
