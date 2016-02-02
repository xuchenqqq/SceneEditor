#pragma once

class QJsonObject;
class ScreenMng;
class GSLayer;
class GSScreen;

class JsonReader
{
public:
	JsonReader(ScreenMng* pMng);
	~JsonReader();
	bool Read(QFile& file);
private:
	bool ParseHead(const QJsonObject& json);
	bool ParseBody(const QJsonObject& json);
	bool CreateScreen(const QJsonObject& json,int idx);
    static bool CreateLayer(const QJsonObject& json,GSScreen* pScreen);
	static bool CreateItem(const QJsonObject& json,GSLayer* pLayer);
private:
	ScreenMng* mpMng;
};