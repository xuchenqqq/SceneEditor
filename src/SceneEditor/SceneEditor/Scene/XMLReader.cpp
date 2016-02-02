#include "XMLReader.h"
#include "PreHeader.h"
#include "ScreenMng.h"
#include "GSScreen.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "Macro.h"
#include <assert.h>
#include "EditorScene.h"
#include "AppModAlias.h"
#include "RtUID.h"
#include "JsonConvert.h"

XMLReader::XMLReader(ScreenMng* pMng)
    :mpMng(pMng)
{
}

XMLReader::~XMLReader()
{
}

bool XMLReader::Read(QFile &file){
    do{
        mpMng->Clear();
        if(!doc.setContent(&file)){
          file.close();
          return false;
        }
        file.close();
        //解析结点
        //root(map)
        QDomElement root = doc.documentElement();
        bool res = false;
        res = ParseHead(root); assert(res);
        res = ParseBody(root); assert(res);

        return true;
    }while(false);
    //异常流程
    mpMng->Clear();
    return false;
}

bool XMLReader::ParseHead(const QDomElement &root){
   auto head = root.firstChild();
   //size
   auto sizeNode = head.namedItem("Size");
   int width = sizeNode.namedItem("Width").firstChild().nodeValue().toInt();
   int height = sizeNode.namedItem("Height").firstChild().nodeValue().toInt();
   QSize size(width,height);
   mpMng->mSize = size;
   //dir
   auto dirNode = head.namedItem("SceneDir");
   int dir = dirNode.firstChild().nodeValue().toInt();
   mpMng->mDir = dir;
   return true;
}

bool XMLReader::ParseBody(const QDomElement &root){
    auto body = root.lastChild();
    auto sceneList = NodeList(body);
    for(int i=0; i<sceneList.count(); i++){
        auto scene = sceneList.at(i);
        CreateScrren(scene,i);
    }
    return true;
}

QDomNodeList XMLReader::NodeList(const QDomNode& parent){
    auto list = parent.childNodes();
    return list;
}

bool XMLReader::CreateScrren(const QDomNode &scene,int idx){
    ASSERT_MSG_RET(!scene.isNull(),"屏幕元素无效");
    QSize sz = mpMng->mSize;
    int dir = mpMng->mDir;
    int originX,originY;
    switch (dir) {
    case 0:
        originX = sz.width();
        originY = 0;
        break;
    case 1:
        originX = 0;
        originY = -sz.height();
    case 2:
        originX = 0;
        originY = sz.height();
    case 3:
        originX = -sz.width();
        originY = 0;
    default:
        return false;
        break;
    }
    GSScreen* pScreen = new GSScreen(sz,QPoint(originX*idx,originY*idx),RtUID::Create());
    pScreen->setZValue(0);
    //场景层
    auto layerList = scene.childNodes();
    for(int i=0; i<layerList.count(); i++){
        auto layer = layerList.at(i);
        FAIL_GOTO(CreateLayer(layer,pScreen),ERR_TAG)
    }
    sScene.addItem(pScreen);
    mpMng->mList.push_back(pScreen);
    return true;
ERR_TAG:
    delete pScreen;
    return false;
}

bool XMLReader::CreateLayer(const QDomNode &layer, GSScreen *pScreen){
    ASSERT_MSG_RET(!layer.isNull(),"景区元素无效");
    auto speedNode = layer.namedItem("Speed");
    int speed = speedNode.firstChild().nodeValue().toInt();
    auto nameNode = layer.namedItem("Name");
    QString name = nameNode.firstChild().nodeValue();
    GSLayer* pLayer;
    if(layer.nodeName().left(11) == "ObjectLayer"){
         GSLayer::Desc desc(RtUID::Create(),pScreen->ID(),RtUID::Null,speed,name,true);
         pLayer = pScreen->InsertLayer(desc);
    }else{
        GSLayer::Desc desc(RtUID::Create(),pScreen->ID(),RtUID::Null,speed,name,false);
        pLayer = pScreen->InsertLayer(desc);
    }
    //精灵层
    auto itemList = layer.childNodes();
    QList<QDomNode> spriteList;
    for(int i=0; i<itemList.count(); i++){
        if(itemList.at(i).nodeName().left(6) == "Sprite"){
            spriteList.push_back(itemList.at(i));
        }
    }
    for(int i=0; i<spriteList.count(); i++){
        auto sprite = spriteList.at(i);
        FAIL_GOTO(CreateItem(sprite, pLayer),ERR_TAG)
    }
    return true;
ERR_TAG:
    delete pLayer;
    return false;
}

bool XMLReader::CreateItem(const QDomNode &item, GSLayer *pLayer){
    ASSERT_MSG_RET(!item.isNull(),"图层元素无效");
    //Pos
    auto posNode = item.namedItem("Pos");
    qreal x = posNode.namedItem("X").firstChild().nodeValue().toDouble();
    qreal y = posNode.namedItem("Y").firstChild().nodeValue().toDouble();
    QPointF pos(x,y);
    Json::Coco2Qt(pos);
    //Name
    auto nameNode = item.namedItem("Name");
    QString name = nameNode.firstChild().nodeValue();
    //Scale
    auto scaleNode = item.namedItem("Scale");
    qreal scale = scaleNode.firstChild().nodeValue().toDouble();
    //FlipX
    auto flipXNode = item.namedItem("FlipX");
    bool flipX = flipXNode.firstChild().nodeValue().toInt();
    //FlipY
    auto flipYNode = item.namedItem("FlipY");
    bool flipY = flipYNode.firstChild().nodeValue().toInt();
    //customAttr
    if(pLayer->IsObject()){
        QMap<QString,QString> customAttrMap;
        auto customAttrNode = item.namedItem("CustomAttr");
        auto attrNodeList = customAttrNode.childNodes();
        for(int i=0; i<attrNodeList.count(); i++){
            QString key = attrNodeList.at(i).nodeName();
            QString value = attrNodeList.at(i).firstChild().nodeValue();
            customAttrMap.insert(key,value);
        }
        GSItem::Desc desc(name, pos,RtUID::Create(),pLayer->ID(),RtUID::Null,scale,flipX,flipY);
        pLayer->InsertItem(desc,customAttrMap);
    }else{
        GSItem::Desc desc(name, pos,RtUID::Create(),pLayer->ID(),RtUID::Null,scale,flipX,flipY);
        pLayer->InsertItem(desc);
    }
    return true;
}



