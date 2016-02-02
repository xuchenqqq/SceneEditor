#pragma once

#include <QtGlobal>
#include <QList>

class QGraphicsItem;
class QPointF;
class GSItem;

namespace ItemQueue{
	void QueueItemByTop(QList<GSItem*>& list);
	void QueueItemByBottom(QList<GSItem*>& list);
	void QueueItemByLeft(QList<GSItem*>& list);
	void QueueItemByRight(QList<GSItem*>& list);
	QPointF ToTop(qreal dst,const QGraphicsItem* pItem,qreal& nextDst);
	QPointF ToBottom(qreal dst,const QGraphicsItem* pItem,qreal& nextDst);
	QPointF ToLeft(qreal dst,const QGraphicsItem* pItem,qreal& nextDst);
	QPointF ToRight(qreal dst,const QGraphicsItem* pItem,qreal& nextDst);
}
