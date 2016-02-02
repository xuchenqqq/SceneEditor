#include "PreHeader.h"
#include "JsonWriter.h"
#include "JsonKeyword.h"
#include "GSScreen.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "JsonConvert.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "ScreenMng.h"
#include "AppModAlias.h"

QVector<QString> JsonWriter::allUseImage = {};

JsonWriter::JsonWriter(const ScreenMng* pMng)
: mpMng(pMng)
{
}
JsonWriter::~JsonWriter(){
}
void JsonWriter::Write(QFile& file) const{
    //清空allUseImage
    allUseImage.clear();
	QJsonObject json;
	json[JsonKW::VERSION] = JsonKW::VER_NUMBER;
	json[JsonKW::HEAD] = Head();
    json[JsonKW::BODY] = Body();
    json[JsonKW::ALLUSEIMAGE] = AllUseImage();
	QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
}
QJsonObject JsonWriter::Head() const{
	QJsonObject obj;
	obj[JsonKW::SCENE_CNT] = mpMng->mList.count();
	obj[JsonKW::SCENE_SZ] = Json::From(mpMng->mSize);
    obj[JsonKW::SCENE_DIR] = mpMng->mDir;
	return obj;
}
QJsonObject JsonWriter::Body() const{
	QJsonArray list;
	foreach(GSScreen* pScreen,mpMng->mList){
		list.append(Screen(pScreen));
	}
	QJsonObject json;
	json[JsonKW::SCREENS] = list;
	return json;
}
QJsonObject JsonWriter::Screen(GSScreen* pScreen){
	QJsonArray list;
	foreach(GSLayer* pLayer,pScreen->LayerList()){
		list.append(Layer(pLayer));
	}
	QJsonObject json;
	json[JsonKW::LAYERS] = list;
	return json;
}
QJsonObject JsonWriter::Layer(GSLayer* pLayer){
	QJsonArray list;
	foreach(GSItem* pItem,pLayer->ItemList()){
		list.append(Item(pItem));
	}
	QJsonObject json;
	json[JsonKW::SZ] = Json::From(pLayer->Size());
	json[JsonKW::POS] = Json::From(QPointF(0,0));
	json[JsonKW::NAME] = pLayer->Name();
	json[JsonKW::SPEED] = pLayer->Speed();
    json[JsonKW::Z] = 0;
    json[JsonKW::ISEFFECT] = pLayer->IsObject();
	json[JsonKW::ITEMS] = list;
	return json;
}
QJsonObject JsonWriter::Item(GSItem* pItem){
	QJsonObject json;
	QPointF pos = pItem->pos();
	Json::Qt2Coco(pos);
	json[JsonKW::POS] = Json::From(pos);
	json[JsonKW::Z] = 0;
	json[JsonKW::SCALE] = pItem->scale();
    QStringList list = pItem->Name().split(".");
    if (pItem ->Layer()->IsObject()){
        json[JsonKW::IMAGE] = list[0];
    }else{
        json[JsonKW::IMAGE] = pItem->Name();
    }
    //查看是否重复
    bool isCopy = false;
    for(auto image: allUseImage){
        if(pItem->Name() == image){
            isCopy = true;
        }
    }
    if(!isCopy){
        if(pItem->Name().right(4) == ".png" ||
                pItem->Name().right(4) == ".jpg"){
            if(!pItem->Layer()->IsObject()){
                allUseImage.push_back(pItem->Name());
            }
        }
    }
	json[JsonKW::FLIP_X] = pItem->FlipX();
	json[JsonKW::FLIP_Y] = pItem->FlipY();
    if(pItem->Layer()->IsObject()){
        json[JsonKW::CUSTOMATTR] = CustomAttr(pItem);
    }
	return json;
}

QJsonObject JsonWriter::CustomAttr(GSItem* pItem){
    QJsonObject json;
    auto attrMap = pItem->customAttr;
    for(auto it=attrMap.begin(); it!=attrMap.end(); it++){
        QString key = it.key();
        QString value = it.value();
        json[key] = value;
    }
    return json;
}

QJsonArray JsonWriter::AllUseImage(){
    QJsonArray array;
    for(auto image: allUseImage){
        array.append(image);
    }
    return array;
}

