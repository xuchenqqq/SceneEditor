#ifndef JSON_CONVERT_H
#define JSON_CONVERT_H

namespace Json
{
	QJsonObject From(const QSize& size);
	QSize       ToSize(const QJsonObject& json);
	QJsonObject From(const QPointF& pos);
	QPointF     ToPoint(const QJsonObject& json);
	void        Qt2Coco(QPointF& pos);
	void        Coco2Qt(QPointF& pos);
}


#endif