#ifndef ITEM_DOCK_H
#define ITEM_DOCK_H

#include <QDockWidget>
#include "ui_ItemDock.h"
#include <QMap>

class GSLayer;
class GSItem;
class ItemWidget;

class ItemDock : public QDockWidget
{
Q_OBJECT
public:
	ItemDock(QWidget* parent);
	~ItemDock();
signals:
	void ClearSelection();
public slots:
	void OnLayerChanged(GSLayer* pLayer);
private slots:
	void SelectionChanged(const QItemSelection&,const QItemSelection&);
private:
	void Init();
	void BindSignal();
private:
	Ui::ItemDock ui;
	class ItemModel* mpModel;
};

#endif