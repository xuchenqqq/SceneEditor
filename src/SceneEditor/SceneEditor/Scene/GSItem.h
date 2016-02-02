#ifndef GS_ITEM_H
#define GS_ITEM_H

#include <QGraphicsItem>
#include <QString>
#include <QPointer>
#include "AttrEditor.h"
#include "RtUID.h"
#include "ItemSignal.h"
#include "CacheObs.h"
#include "MoveRecorder.h"
#include <QMap>

class EditorScene;
class GSLayer;

class GSItem : public QGraphicsItem
{
public:
	enum{Type = UserType+3};
	struct Desc{
		QString file;
		QPointF pos;
		qreal   scale;
		RtUID::Type id;
		RtUID::Type layerId;
		RtUID::Type afterId;
		bool flipX;
        bool flipY;
        Desc(const QString& _file,
			 const QPointF& _pos,
			 RtUID::Type _id,
			 RtUID::Type _layerId,
			 RtUID::Type _afterId=RtUID::Null,
			 qreal _scale=1.0,
			 bool _flipX=false,
             bool _flipY=false)
		: file(_file), pos(_pos), scale(_scale), id(_id)
        , layerId(_layerId), afterId(_afterId)
        , flipX(_flipX), flipY(_flipY)
		{}
	};
    GSItem(const Desc& desc);
	~GSItem();
	RtUID::Type ID() const{return mId;}
	int type() const{return Type;}
	QString Name() const{return mName;}
	QString File() const{return mFile;}
	bool FlipX() const{return mFlipX;}
	bool FlipY() const{return mFlipY;}
	void SetFlipX(bool stat);
	void SetFlipY(bool stat);
	void SetName(const QString& name){mName = name;}
	void SetFile(const QString& file);
	void Refresh();
	Desc GetDesc() const;
	QRectF boundingRect() const{return mRect;}
	void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget*);
	GSLayer* Layer() const;
	RtUID::Type AfterID() const;
	void SetPosNotRecord(QPointF& pos);
	AttrEditor* Editor();
	ItemSignal* Signal();
    void SetCustomAttr(const QMap<QString, QString> cusAttr);
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant & value);
private:
	void Init();
	EditorScene* Scene() const;
	void ScaleChanged();
	void Moved();
	void PosChanged();
	void UpdateAttr(const QString& name);
private:
	QString mFile;
	QString mName;
	RtUID::Type mId;
	QRectF mRect;
	MoveRecorder mMoveRecorder;
	QPointer<AttrEditor> mpEditor;
	QPointer<ItemSignal> mpSignal;
	CacheObs mObs;
	bool mFlipX;
	bool mFlipY;  
public:
     QMap<QString, QString> customAttr;
};

#endif
