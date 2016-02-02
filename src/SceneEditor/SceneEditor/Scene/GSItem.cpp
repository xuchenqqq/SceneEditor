#include "PreHeader.h"
#include "GSItem.h"
#include "Cache.h"
#include "AppModAlias.h"
#include "GSLayer.h"
#include "EditorScene.h"
#include <QJsonObject>
#include "JsonConvert.h"
#include "Macro.h"
#include <QtDebug>
#include "Log.h"

GSItem::GSItem(const Desc& desc)
: mName(desc.file)
, mId(desc.id)
, mMoveRecorder(this)
, mObs(std::bind(&GSItem::Refresh,this))
, mFlipX(desc.flipX)
, mFlipY(desc.flipY)
{
	//严格按照这个顺序初始化
	setScale(desc.scale);
	setPos(desc.pos);
	SetFile(desc.file);
	Init();
}
GSItem::~GSItem(){
}
GSItem::Desc GSItem::GetDesc() const{
    return Desc(mFile,pos(),mId,Layer()->ID(),AfterID(),scale(),mFlipX,mFlipY);
}
void GSItem::SetFlipX(bool stat){
	mFlipX = stat;
	update();
}
void GSItem::SetFlipY(bool stat){
	mFlipY = stat;
	update();
}
void GSItem::SetFile(const QString& file){
	FAIL_RET_VOID(file!=mFile);
    mFile = file;
	mObs.Bind(mFile);
    UpdateAttr("image");
	Refresh();
}
void GSItem::Refresh(){
	QSize size = sCache.Pixmap(mFile).size();
	mRect = QRectF(-size.width()/2,-size.height()/2,
		           size.width(),size.height());
	prepareGeometryChange();
}

void GSItem::paint(QPainter* painter,const QStyleOptionGraphicsItem*,QWidget*){
	painter->save();
	painter->setRenderHint(QPainter::SmoothPixmapTransform,true);
	if(mFlipX || mFlipY){
		painter->setTransform(
			QTransform::fromScale(mFlipX?(-1.0):1.0,
			                  mFlipY?(-1.0):1.0),true);
	}
    const QPixmap& pixmap = sCache.Pixmap(mFile);
    painter->drawPixmap(boundingRect(),pixmap,pixmap.rect());
	if(isSelected()){
        painter->setPen(QPen(Qt::green,3,Qt::DotLine));
		painter->drawRect(boundingRect());
	}
	painter->restore();
}
void GSItem::Init(){
	setFlag(ItemIsMovable, true);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges,true);
}
GSLayer* GSItem::Layer() const{
	return qgraphicsitem_cast<GSLayer*>(parentItem());
}
RtUID::Type GSItem::AfterID() const{
	QList<QGraphicsItem*> list = parentItem()->childItems();
	int idx = 0;
	for(idx= 0; idx< list.size(); ++idx){
		if(list[idx]==this) break;
	}
	if(idx+1<list.size()){
		return qgraphicsitem_cast<GSItem*>(list[idx+1])->ID();
	}else{
		return RtUID::Null;
	}
}
QVariant GSItem::itemChange(GraphicsItemChange change, const QVariant & value){
	switch(change){
		case ItemPositionChange: Moved();break;
		case ItemScaleHasChanged: ScaleChanged(); break;
		default: break;
	}
	return QGraphicsItem::itemChange(change,value);
}
EditorScene* GSItem::Scene() const{
	return qobject_cast<EditorScene*>(scene());
}
void GSItem::ScaleChanged(){
	UpdateAttr("scale");
	if(mpSignal){
		mpSignal->NotifyRectChange();
	}
}
void GSItem::PosChanged(){
	UpdateAttr("pos");
	if(mpSignal){
		mpSignal->NotifyRectChange();
	}
}
void GSItem::UpdateAttr(const QString& name){
	if(mpEditor){
		mpEditor->UpdateUI(name);
	}
}
void GSItem::SetPosNotRecord(QPointF& pos){
	setFlag(ItemSendsGeometryChanges,false);
	setPos(pos);
	setFlag(ItemSendsGeometryChanges,true);
	PosChanged();
}
void GSItem::Moved(){
	mMoveRecorder.Trigger();
	PosChanged();
}
AttrEditor* GSItem::Editor(){
	if(!mpEditor){
        mpEditor = new AttrEditor(this);
	}
	return mpEditor.data();
}
ItemSignal* GSItem::Signal(){
	if(!mpSignal){
		mpSignal = new ItemSignal(this);
	}
	return mpSignal.data();
}
void GSItem::SetCustomAttr(const QMap<QString, QString> cusAttr)
{
    for(auto it=customAttr.begin(); it!=customAttr.end();){
        customAttr.erase(it++);
    }
    for(auto it=cusAttr.begin(); it!=cusAttr.end(); it++){
        QString key = it.key();
        QString value = it.value();
        if(!key.isEmpty()){
            customAttr.insert(key,value);
        }
    }
}


