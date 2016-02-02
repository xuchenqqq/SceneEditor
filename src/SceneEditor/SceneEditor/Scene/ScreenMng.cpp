#include "PreHeader.h"
#include "ScreenMng.h"
#include <assert.h>
#include "Macro.h"
#include "GSScreen.h"
#include "JsonReader.h"
#include "JsonWriter.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "XMLWriter.h"
#include "XMLReader.h"

extern bool coutFormat;

enum{
	MAX_SCREEN_NUM = 10,
	MIN_SCREEN_WIDTH = 10,
	MAN_SCREEN_WIDTH = 8000,
	MIN_SCREEN_HEIGHT = 10,
	MAN_SCREEN_HEIGHT = 8000,
};

ScreenMng::ScreenMng(QObject* parent)
: QObject(parent)
{}
ScreenMng::~ScreenMng(){
	Clear();
}
void ScreenMng::New(const QSize size,unsigned int cnt, int dir){
	ASSERT_MSG_RET_VOID(cnt>0 && cnt<=MAX_SCREEN_NUM,"屏幕数量太多");
	ASSERT_MSG_RET_VOID(size.width()>=MIN_SCREEN_WIDTH && 
	                    size.width()<=MAN_SCREEN_WIDTH ,"屏幕尺寸错误");
	ASSERT_MSG_RET_VOID(size.height()>=MIN_SCREEN_HEIGHT && 
	                    size.height()<=MAN_SCREEN_HEIGHT ,"屏幕尺寸错误");
	Clear();
	mSize = size;
    mDir = dir;

    int originX,originY;
    switch (dir) {
    case 0:  //right
        originX = size.width();
        originY = 0;
        break;
    case 1:  // top
        originX = 0;
        originY = -size.height();
        break;
    case 2:  //bottom
        originX = 0;
        originY = size.height();
        break;
    case 3:  // left
        originX = -size.width();
        originY = 0;
        break;
    default:
        return;
        break;
    }

    for(unsigned int i = 0; i < cnt; ++i){
        GSScreen* pScreen= new GSScreen(mSize,QPoint(originX*i,originY*i),RtUID::Create());
        pScreen->setZValue(0);
		sScene.addItem(pScreen);
		mList.push_back(pScreen);
	}
	emit Updated(mList);
}
void ScreenMng::Save(QFile& file) const{
    if(coutFormat){
        XMLWriter(this).Write(file);
    }else{
        JsonWriter(this).Write(file);
    }
}
bool ScreenMng::Load(QFile& file){
    bool res = false;
    if(coutFormat){
        res = XMLReader(this).Read(file);
    }else{
        res = JsonReader(this).Read(file);
    }
	if(res){
		emit Updated(mList);
	}
	return res;
}
void ScreenMng::Clear(){
	emit Updated(QList<GSScreen*>());
	for(int i = 0; i<mList.size(); ++i){
		sScene.removeItem(mList[i]);
	}
	qDeleteAll(mList);
	mList.clear();
}
void ScreenMng::Preview(bool state){
	if(state){
		emit Updated(QList<GSScreen*>());
		foreach(GSScreen* pScreen,mList){
			pScreen->Preview();
		}
	}else{
		emit Updated(mList);
	}
}
