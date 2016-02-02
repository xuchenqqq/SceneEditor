#include "PreHeader.h"
#include "ItemSignal.h"
#include <QRectF>
#include "GSItem.h"

ItemSignal::ItemSignal(GSItem* pItem)
: mpItem(pItem)
{}
ItemSignal::~ItemSignal(){
	emit RectChanged(QRectF());
}
void ItemSignal::NotifyRectChange(){
	emit RectChanged(mpItem->mapRectToParent(mpItem->boundingRect()));
}