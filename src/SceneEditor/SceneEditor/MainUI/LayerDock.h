#ifndef LAYER_DOCK_H
#define LAYER_DOCK_H

#include <QDockWidget>
#include "ui_LayerDock.h"

class GSScreen;
class GSLayer;
class QModelIndex;

class LayerDock : public QDockWidget
{
Q_OBJECT
public:
	LayerDock(QWidget* parent);
	~LayerDock();
signals:
	void LayerChanged(GSLayer* pLayer);
public slots:
	void OnScreenChanged(GSScreen* pScreen);
private slots:
	void Add();
    void AddObject();
	void Remove();
	void RowChanged(const QModelIndex& cur,const QModelIndex& pre);
private:
	void Init();
	void BindSignal();
	void SelectOtherRow();
	int  CurrentRow() const;
	void SetCurrentRow(int row);
	int  RowCount() const;
private:
	Ui::LayerDock ui;
	class LayerModel* mpModel;
};

#endif
