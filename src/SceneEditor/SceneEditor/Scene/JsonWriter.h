#pragma once

class ScreenMng;
class QJsonObject;
class GSScreen;
class GSLayer;
class GSItem;

#include<QAction>
#include <QVector>

class JsonWriter
{
public:
	JsonWriter(const ScreenMng* pMng);
	~JsonWriter();
	void Write(QFile& file) const;
private:
	QJsonObject Head() const;
	QJsonObject Body() const;
    static QJsonObject Screen(GSScreen* pScreen);
    static QJsonObject Layer(GSLayer* pLayer);
    static QJsonObject Item(GSItem* pItem);
    static QJsonObject CustomAttr(GSItem* pItem);
    static QJsonArray AllUseImage();
private:
    const ScreenMng* mpMng;
    static QVector<QString> allUseImage;
};
