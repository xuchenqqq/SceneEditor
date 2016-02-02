#pragma once

#include <QPointF>

class GSItem;

class MoveRecorder
{
public:
	MoveRecorder(GSItem* pItem);
	~MoveRecorder();
	void Trigger();
	void Record();
private:
	GSItem* mpItem;
	QPointF mPrePos;
	int mTimerId;
	bool mIsRecording;
};

