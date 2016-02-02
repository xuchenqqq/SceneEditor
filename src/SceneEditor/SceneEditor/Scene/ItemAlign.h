#pragma once

#include <QList>

class QPointF;
class QGraphicsItem;
class GSItem;

namespace ItemAlign{
	QPointF ToTop(const QGraphicsItem* pItem);
	QPointF ToBottom(const QGraphicsItem* pItem);
	QPointF ToLeft(const QGraphicsItem* pItem);
	QPointF ToRight(const QGraphicsItem* pItem);
	QPointF Center(const QList<GSItem*>& list);
}