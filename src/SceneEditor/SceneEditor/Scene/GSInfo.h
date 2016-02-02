#pragma once

#include "ModDeclare.h"
#include <QPointF>

class GSScreen;
class GSLayer;
class GSItem;

class GSInfo
{
public:
	GSInfo()
	: mpScreen(NULL)
	, mpLayer(NULL)
	, mpItem(NULL)
	{}
	void Set(GSScreen* pScreen){mpScreen=pScreen;}
	void Set(GSLayer* pLayer){mpLayer=pLayer;}
	void Set(GSItem* pItem){mpItem=pItem;}
	void SetSceneMousePos(const QPointF& pos);
	GSScreen* Screen() const{return mpScreen;}
	GSLayer*  Layer() const{return mpLayer;}
	GSItem*   Item() const{return mpItem;}
	QPointF   ScreenMousePos() const;
	QPointF   LayerMousePos() const;
	QPointF   ItemMousePos() const;
private:
	GSScreen* mpScreen;
	GSLayer*  mpLayer;
	GSItem*   mpItem;
	QPointF   mSceneMousePos;
};

M_DECL_MOD(GSInfo)
