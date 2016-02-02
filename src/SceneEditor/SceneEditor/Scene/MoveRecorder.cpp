#include "PreHeader.h"
#include "MoveRecorder.h"
#include "GSItem.h"
#include "TimerService.h"
#include "AppModAlias.h"
#include "EditorScene.h"
#include "Macro.h"
#include "CmdItem.h"

const int RECORD_INTERVAL = 1000;

MoveRecorder::MoveRecorder(GSItem* pItem)
: mpItem(pItem)
, mTimerId(0)
, mIsRecording(false)
{}
MoveRecorder::~MoveRecorder(){
	if(mTimerId){
		sTimer.Cancel(mTimerId);
	}
}
void MoveRecorder::Trigger(){
	FAIL_RET_VOID(!mIsRecording);
	mPrePos = mpItem->pos();
	mTimerId = sTimer.Add(RECORD_INTERVAL,[=](){Record();});
	mIsRecording = true;
}
void MoveRecorder::Record(){
	sScene.PushCmd(new CmdItemMove(mpItem,mPrePos,mpItem->pos()));
	mIsRecording = false;
	mTimerId = 0;
}

