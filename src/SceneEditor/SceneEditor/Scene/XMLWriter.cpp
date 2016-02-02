#include "XMLWriter.h"
#include "PreHeader.h"
#include "GSScreen.h"
#include "GSLayer.h"
#include "GSItem.h"
#include "ScreenMng.h"
#include "AppModAlias.h"
#include "JsonConvert.h"

XMLWriter::XMLWriter(const ScreenMng* pMng)
    : mpMng(pMng)
{
}

XMLWriter::~XMLWriter()
{
}

void XMLWriter::Write(QFile& file){
    QDomDocument doc;
    auto instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("map");
    root.setAttribute("version","1.0");
    doc.appendChild(root);
    //Head
    Head(root);
    //Body
    Body(root);
    //写入到xml文件
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}

void XMLWriter::Head(QDomElement& root){
    auto head = doc.createElement("Head");
    root.appendChild(head);

    auto sceneCount = doc.createElement("SceneCount");
    auto sceneCountText = doc.createTextNode(QString::number(mpMng->mList.count()));
    sceneCount.appendChild(sceneCountText);
    head.appendChild(sceneCount);

    auto sceneDir = doc.createElement("SceneDir");
    auto sceneDirText = doc.createTextNode(QString::number(mpMng->mDir));
    sceneDir.appendChild(sceneDirText);
    head.appendChild(sceneDir);

    int width = mpMng->mSize.width();
    int height = mpMng->mSize.height();
    createSizeElement(head,width,height);
}

void XMLWriter::Body(QDomElement& root){
    auto body = doc.createElement("Body");
    root.appendChild(body);
    Scene(body);
}

void XMLWriter::Scene(QDomElement& body){
    for(int i=0; i<mpMng->mList.count(); i++){
        auto scene = doc.createElement("Scene"+QString::number(i+1));
        scene.setAttribute("id",mpMng->mList[i]->ID());
        body.appendChild(scene);
        Layer(scene,i);
    }
}

void XMLWriter::Layer(QDomElement& scene,int num){
    auto list = mpMng->mList[num]->LayerList();
    for(int i=0; i<list.count(); i++){
        QString tag;
        if(!list[i]->IsObject()){
            tag = "Layer";
        }else{
            tag = "ObjectLayer";
        }
        auto layer = doc.createElement(tag+QString::number(i+1));
        layer.setAttribute("id",list[i]->ID());
        scene.appendChild(layer);
        //name
        auto name = doc.createElement("Name");
        layer.appendChild(name);
        auto nameText = doc.createTextNode(list[i]->Name());
        name.appendChild(nameText);
        //speed
        auto speed = doc.createElement("Speed");
        layer.appendChild(speed);
        auto speedText = doc.createTextNode(QString::number(list[i]->Speed()));
        speed.appendChild(speedText);
        //zOrder
        auto zOrder = doc.createElement("ZOrder");
        layer.appendChild(zOrder);
        auto zOrderText = doc.createTextNode(QString::number(i));
        zOrder.appendChild(zOrderText);
        //size
        int width = list[i]->Size().width();
        int height = list[i]->Size().height();
        createSizeElement(layer,width,height);
        //pos
        createPosElement(layer,0,0);
        //item
        Item(layer,list[i]);
    }
}

void XMLWriter::Item(QDomElement &layer,GSLayer* l){
    auto list = l->ItemList();
    for(int i=0; i<list.count(); i++){
        auto item = doc.createElement("Sprite"+QString::number(i+1));
        item.setAttribute("id",list[i]->ID());
        layer.appendChild(item);
        //scale
        auto scale = doc.createElement("Scale");
        item.appendChild(scale);
        auto scaleText = doc.createTextNode(QString::number(list[i]->scale()));
        scale.appendChild(scaleText);
        //FilpX
        auto flipX = doc.createElement("FlipX");
        item.appendChild(flipX);
        auto flipXText = doc.createTextNode(QString::number(list[i]->FlipX()));
        flipX.appendChild(flipXText);
        //FilpY
        auto flipY = doc.createElement("FlipY");
        item.appendChild(flipY);
        auto flipYText = doc.createTextNode(QString::number(list[i]->FlipY()));
        flipY.appendChild(flipYText);
        //zOrder
        auto zOrder = doc.createElement("ZOrder");
        item.appendChild(zOrder);
        auto zOrderText = doc.createTextNode(QString::number(i));
        zOrder.appendChild(zOrderText);
        //Pos
        QPointF pos = list[i]->pos();
        Json::Qt2Coco(pos);
        createPosElement(item,pos.x(),pos.y());
        //name
        auto name = doc.createElement("Name");
        item.appendChild(name);
        auto nameText = doc.createTextNode(list[i]->Name());
        name.appendChild(nameText);
        //customAttr
        if(l->IsObject()){
            auto customAttribute = doc.createElement("CustomAttr");
            item.appendChild(customAttribute);
            createCusElement(customAttribute,list[i]);
        }
    }
}

void XMLWriter::createSizeElement(QDomElement &parent,int width,int height){
    auto Size = doc.createElement("Size");
    parent.appendChild(Size);
    auto Width = doc.createElement("Width");
    auto WidthText = doc.createTextNode(QString::number(width));
    Width.appendChild(WidthText);
    Size.appendChild(Width);
    auto Height = doc.createElement("Height");
    auto HeightText = doc.createTextNode(QString::number(height));
    Height.appendChild(HeightText);
    Size.appendChild(Height);
}

void XMLWriter::createPosElement(QDomElement &parent, qreal x, qreal y){
    auto Pos = doc.createElement("Pos");
    parent.appendChild(Pos);
    auto X = doc.createElement("X");
    auto XText = doc.createTextNode(QString::number(x));
    X.appendChild(XText);
    Pos.appendChild(X);
    auto Y = doc.createElement("Y");
    auto YText = doc.createTextNode(QString::number(y));
    Y.appendChild(YText);
    Pos.appendChild(Y);
}

void XMLWriter::createCusElement(QDomElement &parent, GSItem *item){
    auto map = item->customAttr;
    for(auto it = map.begin(); it!=map.end(); it++){
        QString k = it.key();
        QString v = it.value();
        auto key = doc.createElement(k);
        parent.appendChild(key);
        auto keyText = doc.createTextNode(v);
        key.appendChild(keyText);
    }
}
