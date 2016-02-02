#include "PreHeader.h"
#include "JsonReader.h"
#include "ScreenMng.h"
#include "GSScreen.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "Macro.h"
#include <assert.h>
#include "JsonKeyword.h"
#include "EditorScene.h"
#include "AppModAlias.h"
#include "RtUID.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "JsonConvert.h"

JsonReader::JsonReader(ScreenMng* pMng)
: mpMng(pMng)
{}
JsonReader::~JsonReader()
{}
bool JsonReader::Read(QFile& file){
	do{
		mpMng->Clear();
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		QJsonObject json = doc.object();
		FAIL_BREAK(json[JsonKW::VERSION].toInt() == JsonKW::VER_NUMBER);
		FAIL_BREAK(ParseHead(json[JsonKW::HEAD].toObject()));
		FAIL_BREAK(ParseBody(json[JsonKW::BODY].toObject()));
		return true;
	}while(false);
	//异常流程
	mpMng->Clear();
	return false;
}
bool JsonReader::ParseHead(const QJsonObject& json){
	ASSERT_MSG_RET(!json.isEmpty(),"Json文件无Header");
	mpMng->mSize = Json::ToSize(json[JsonKW::SCENE_SZ].toObject());
    mpMng->mDir = json[JsonKW::SCENE_DIR].toInt() ? json[JsonKW::SCENE_DIR].toInt() : 0;
	return true;
}
bool JsonReader::ParseBody(const QJsonObject& json){
	ASSERT_MSG_RET(!json.isEmpty(),"Json文件无Body");
	QJsonArray list = json[JsonKW::SCREENS].toArray();
	for(int i = 0; i < list.size(); ++i){
		FAIL_RET(CreateScreen(list[i].toObject(),i));
	}
	return true;
}
bool JsonReader::CreateScreen(const QJsonObject& json,int idx){
    ASSERT_MSG_RET(!json.isEmpty(),"屏幕元素无效");
    QSize sz = mpMng->mSize;
    int dir = mpMng->mDir;
    int originX,originY;
    switch (dir) {
    case 0:  //right
        originX = sz.width();
        originY = 0;
        break;
    case 1:  // top
        originX = 0;
        originY = -sz.height();
        break;
    case 2:  //bottom
        originX = 0;
        originY = sz.height();
        break;
    case 3:  // left
        originX = -sz.width();
        originY = 0;
        break;
    default:
        return false;
        break;
    }

    GSScreen* pScreen = new GSScreen(sz,QPoint(originX*idx,originY*idx),RtUID::Create());
    pScreen->setZValue(0);
    QJsonArray list = json[JsonKW::LAYERS].toArray();
    for(int i=0; i < list.count(); ++i){
        FAIL_GOTO(CreateLayer(list[i].toObject(),pScreen),ERR_TAG)
    }
    sScene.addItem(pScreen);
    mpMng->mList.push_back(pScreen);
    return true;
ERR_TAG:
    delete pScreen;
    return false;
}
bool JsonReader::CreateLayer(const QJsonObject& json,GSScreen* pScreen){
	ASSERT_MSG_RET(!json.isEmpty(),"景区元素无效");
	GSLayer::Desc desc(RtUID::Create(),
	                   pScreen->ID(),
					   RtUID::Null,
		               json[JsonKW::SPEED].toDouble(1),
                       json[JsonKW::NAME].toString(),
                       json[JsonKW::ISEFFECT].toBool());
	GSLayer* pLayer =pScreen->InsertLayer(desc);
	QJsonArray list = json[JsonKW::ITEMS].toArray();
	for(int i = 0;i < list.size(); ++i){
		FAIL_GOTO(CreateItem(list[i].toObject(),pLayer),ERR_TAG);
	}
	return true;
ERR_TAG:
	delete pLayer;
	return false;
}
bool JsonReader::CreateItem(const QJsonObject& json,GSLayer* pLayer){
    ASSERT_MSG_RET(!json.isEmpty(),"图层元素无效");
    QPointF pos =  Json::ToPoint(json[JsonKW::POS].toObject());
    Json::Coco2Qt(pos);
    GSItem::Desc desc(json[JsonKW::IMAGE].toString(),
                pos,
                RtUID::Create(),
                pLayer->ID(),
                RtUID::Null,
                json[JsonKW::SCALE].toDouble(),
                json[JsonKW::FLIP_X].toBool(),
                json[JsonKW::FLIP_Y].toBool());
    if(pLayer->IsObject()){
         QMap<QString,QString> attrMap;
         QJsonObject cusAttr = json[JsonKW::CUSTOMATTR].toObject();
         for(auto it=cusAttr.begin(); it!=cusAttr.end(); it++){
            QString key = it.key();
            QString value = it.value().toString();
            attrMap.insert(key,value);
         }
        pLayer->InsertItem(desc,attrMap);
    }else{
        pLayer->InsertItem(desc);
    }
    return true;
}
