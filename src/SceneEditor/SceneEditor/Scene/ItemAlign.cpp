#include "PreHeader.h"
#include "ItemAlign.h"
#include "GSItem.h"

namespace ItemAlign{
	QPointF ToTop(const QGraphicsItem* pItem){
		QPointF srcCenter = pItem->pos();
		qreal srcTop = pItem->mapToParent(pItem->boundingRect().topLeft()).y();
		qreal dstTop = pItem->parentItem()->boundingRect().top();
		return QPointF(srcCenter.x(),srcCenter.y()+dstTop-srcTop);
	}
	QPointF ToBottom(const QGraphicsItem* pItem){
		QPointF srcCenter = pItem->pos();
		qreal srcBottom = pItem->mapToParent(pItem->boundingRect().bottomRight()).y();
		qreal dstBottom = pItem->parentItem()->boundingRect().bottom();
		return QPointF(srcCenter.x(),srcCenter.y()+dstBottom-srcBottom);
	}
	QPointF ToLeft(const QGraphicsItem* pItem){
		QPointF srcCenter = pItem->pos();
		qreal srcLeft = pItem->mapToParent(pItem->boundingRect().topLeft()).x();
		qreal dstLeft = pItem->parentItem()->boundingRect().left();
		return QPointF(srcCenter.x()+dstLeft-srcLeft,srcCenter.y());
	}
	QPointF ToRight(const QGraphicsItem* pItem){
		QPointF srcCenter = pItem->pos();
		qreal srcRight = pItem->mapToParent(pItem->boundingRect().bottomRight()).x();
		qreal dstRight = pItem->parentItem()->boundingRect().right();
		return QPointF(srcCenter.x()+dstRight-srcRight,srcCenter.y());
	}
	void AddPoint2Rect(const QPointF& p,QRectF& rect){
		if(p.x() < rect.left()){
			rect.setLeft(p.x());
		}else if(p.x() > rect.right()){
			rect.setRight(p.x());
		}
		
		if(p.y() < rect.top()){
			rect.setTop(p.y());
		}else if(p.y() > rect.bottom()){
			rect.setBottom(p.y());
		}
	}
	QPointF Center(const QList<GSItem*>& list){
		if(list.empty()) return QPointF();
		QRectF rect;
		rect.setTopLeft(list[0]->pos());
		rect.setBottomRight(list[0]->pos());
		foreach(QGraphicsItem* pItem,list){
			AddPoint2Rect(pItem->pos(),rect);
		}
		return rect.center();
	}
}