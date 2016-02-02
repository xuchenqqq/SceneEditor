#pragma once

#include <QObject>

class GSItem;
class QRectF;

class ItemSignal : public QObject
{
Q_OBJECT
public:
	ItemSignal(GSItem* pItem);
	~ItemSignal();
	void NotifyRectChange();
signals:
	void RectChanged(const QRectF& rect);
private:
	GSItem* mpItem;
};
