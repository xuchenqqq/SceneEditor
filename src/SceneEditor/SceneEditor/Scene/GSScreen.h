#ifndef GS_SCREEN_H
#define GS_SCREEN_H

#include <QGraphicsObject>
#include <QMap>
#include "RtUID.h"
#include <QList>
#include "GSLayer.h"

class EditorScene;

class GSScreen : public QGraphicsObject
{
Q_OBJECT
	enum{Type = UserType+1};
public:
	GSScreen(const QSize& size,const QPoint& pos,RtUID::Type id);
	~GSScreen();
	void Preview();
	RtUID::Type ID() const{return mId;}
	QRectF boundingRect() const;
	int type() const{return Type;}
	void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget*);
	QList<GSLayer*> LayerList() const;
    GSLayer* InsertLayer(const GSLayer::Desc& desc);
	void RemoveLayer(RtUID::Type id);
	void OrderLayer(RtUID::Type id,RtUID::Type postId);
	static GSScreen* Find(RtUID::Type id);
	QRectF RectBaseScene() const;
signals:
	void LayerInserted(GSLayer* pLayer,GSLayer* pPostLayer);
	void LayerRemoved(GSLayer* layer);
	void LayerOrdered(GSLayer* pLayer,GSLayer* pPostLayer);
private:
	void Bind();
	void UnBind();
	struct SelItem{
		QGraphicsItem* pItem;
		bool isSelect;
		SelItem():pItem(0),isSelect(false)
		{}
	};
	bool Clear(SelItem& item) const;
	void Restore(SelItem& item) const;
private:
	QSize mSize;
	RtUID::Type mId;
private:
	typedef QMap<RtUID::Type,GSScreen*> Map;
	typedef Map::iterator MapIt;
	static Map msMap;
};

#endif
