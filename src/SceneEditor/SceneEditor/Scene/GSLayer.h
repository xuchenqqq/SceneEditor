#ifndef GS_LAYER_H
#define GS_LAYER_H

#include <QGraphicsObject>
#include <QString>
#include "RtUID.h"
#include <QMap>
#include "GSItem.h"

class GSScreen;

class GSLayer : public QGraphicsObject
{
Q_OBJECT
public:
	enum {Type = UserType+2};
	struct Desc{
		RtUID::Type id;
		RtUID::Type afterId;
		RtUID::Type screenId;
		double   speed;
		QString name;
        bool isObjectLayer;
        Desc(RtUID::Type _id,
			 RtUID::Type _screenId,
			 RtUID::Type _afterId=RtUID::Null,
			 double _speed=1,
             const QString& _name="No Name",
             bool _isObjectLayer = false)
        : id(_id), afterId(_afterId)
        , screenId(_screenId),speed(_speed)
        , name(_name), isObjectLayer(_isObjectLayer)
		{}
	};
public:
    GSLayer(const Desc& desc);
	~GSLayer();
	void Preview();
	void Lock();
	void UnLock();
	RtUID::Type ID() const{return mId;}
	int type() const{return Type;}
	double Speed() const{return mSpeed;}
	void SetSpeed(double val){mSpeed = val;}
	QString Name() const{return mName;}
	void SetName(const QString& name){mName =name;}
	Desc GetDesc() const;
    bool IsObject() const{return isObjectLayer;}
    QString IsObjectLayer() const;
	QSize Size() const;
	QRectF boundingRect() const;
	void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget*);
	QList<GSItem*> ItemList() const;
	GSScreen* Screen() const;
	void InsertItem(const GSItem::Desc& desc);
    void InsertItem(const GSItem::Desc &desc,QMap<QString,QString> attr);
	void RemoveItem(RtUID::Type id);
	void OrderItem(RtUID::Type id,RtUID::Type afterId);
	GSItem* Item(RtUID::Type id) const;
	GSItem* BottomItem() const;
	RtUID::Type AfterID() const;
	static GSLayer* Find(RtUID::Type id);
signals:
	void ItemInserted(GSItem*,GSItem*);
	void ItemRemoved(GSItem*);
	void ItemOrdered(GSItem*,GSItem*);
protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
	void dropEvent(QGraphicsSceneDragDropEvent* event);
private:
	void Bind();
	void UnBind();
private:
	RtUID::Type mId;
	RtUID::Type mScreenId;
	double mSpeed;
    QString mName;
	QRectF mRect;
	typedef QMap<RtUID::Type,GSItem*> ItemMap;
	typedef ItemMap::const_iterator ItemMapConstIt;
	ItemMap mItemMap;
    bool isObjectLayer;
private:
	typedef QMap<RtUID::Type,GSLayer*> Map;
	typedef Map::iterator MapIt;
	static Map msMap;
};

#endif
