#pragma once

#include <QMap>

class BoolImage;
class ImageSwitcher;
class QWidget;

class ItemFactory
{
public:
	static ItemFactory& Instance();
	BoolImage* GetBoolImage(int type);
	ImageSwitcher* CreateImageSwitcher(QWidget* parent,int type);
private:
	ItemFactory();
	~ItemFactory();
	ItemFactory(const ItemFactory&);
private:
	QMap<int,BoolImage*> mMap;
};

#define sItemFactory ItemFactory::Instance()