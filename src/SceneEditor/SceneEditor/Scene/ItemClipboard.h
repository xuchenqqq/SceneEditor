#pragma once

#include <QList>
#include <QRectF>
#include "RtUID.h"
#include "GSItem.h"

class ItemClipboard
{
public:
	ItemClipboard();
	~ItemClipboard();
	void Copy(QList<GSItem*> itemList);
	void Paste(const QPointF& center,RtUID::Type layerId);
private:	
	void RectAddPoint(const QPointF& pos);
private:
	QList<GSItem::Desc> mItemDescs;
	QRectF mRect;
};
