#include "PreHeader.h"
#include "JsonConvert.h"
#include <QJsonObject>
#include <QSize>
#include <QPointF>

namespace Json
{
	QJsonObject From(const QSize& size){
		QJsonObject json;
		json["height"] = size.height();
		json["width"] = size.width();
		return json;
	}
	QSize       ToSize(const QJsonObject& json){
		return QSize(json["width"].toInt(),
		             json["height"].toInt());
	}
	QJsonObject From(const QPointF& pos){
		QJsonObject json;
		json["x"] = pos.x();
		json["y"] = pos.y();
		return json;
	}
	QPointF     ToPoint(const QJsonObject& json){
		return QPointF(json["x"].toDouble(),
		               json["y"].toDouble());
	}
	void Qt2Coco(QPointF& pos){
		pos.setY(-pos.y());
	}
	void Coco2Qt(QPointF& pos){
		pos.setY(-pos.y());
	}
}