#include "PreHeader.h"
#include "ItemQueue.h"
#include "GSItem.h"

namespace ItemQueue{
	QPointF ToTop(qreal dst,const QGraphicsItem* pItem,qreal& nextDst){
		QRectF rect = pItem->mapToParent(pItem->boundingRect()).boundingRect();
		rect.moveTop(dst);
		nextDst = rect.bottom();
		return rect.center();
	}
	QPointF ToBottom(qreal dst,const QGraphicsItem* pItem,qreal& nextDst){
		QRectF rect = pItem->mapToParent(pItem->boundingRect()).boundingRect();
		rect.moveBottom(dst);
		nextDst = rect.top();
		return rect.center();
	}
	QPointF ToLeft(qreal dst,const QGraphicsItem* pItem,qreal& nextDst){
		QRectF rect = pItem->mapToParent(pItem->boundingRect()).boundingRect();
		rect.moveLeft(dst);
		nextDst = rect.right();
		return rect.center();
	}
	QPointF ToRight(qreal dst,const QGraphicsItem* pItem,qreal& nextDst){
		QRectF rect = pItem->mapToParent(pItem->boundingRect()).boundingRect();
		rect.moveRight(dst);
		nextDst = rect.left();
		return rect.center();
	}
	bool TopThan(const GSItem* p1,const GSItem* p2){
		return p1->y() < p2->y();
	}
	void QueueItemByTop(QList<GSItem*>& list){
		qSort(list.begin(),list.end(),TopThan);
	}
	bool BottomThan(const GSItem* p1,const GSItem* p2){
		return p1->y() > p2->y();
	}
	void QueueItemByBottom(QList<GSItem*>& list){
		qSort(list.begin(),list.end(),BottomThan);
	}
	bool LeftThan(const GSItem* p1,const GSItem* p2){
		return p1->x() < p2->x();
	}
	void QueueItemByLeft(QList<GSItem*>& list){
		qSort(list.begin(),list.end(),LeftThan);
	}
	bool RightThan(const GSItem* p1,const GSItem* p2){
		return p1->x() > p2->x();
	}
	void QueueItemByRight(QList<GSItem*>& list){
		qSort(list.begin(),list.end(),RightThan);
	}
}