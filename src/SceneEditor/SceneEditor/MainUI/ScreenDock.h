#ifndef SCREEN_DOCK_H
#define SCREEN_DOCK_H

#include <QDockWidget>
#include "ui_ScreenDock.h"
#include <QList>

class ScreenWidget;
class GSScreen;
class QGraphicsScene;

class ScreenDock : public QDockWidget
{
Q_OBJECT
public:
	ScreenDock(QWidget* parent);
	~ScreenDock();
public slots:
	void ScreenUpdated(const QList<GSScreen*>& list);
signals:
	void ScreenChanged(GSScreen* pScreen);
	void ScreenRectChanged(const QRectF& rect);
private slots:	
	void OnRowChanged(int row);
private:
	ScreenWidget* Widget(int idx);
	GSScreen* Screen(int idx);
	void AddScreen(GSScreen* pScreen);
	void RemoveScreen(int idx);
	void Init();
	void Clear();
	void BindSignal();
private:
	Ui::ScreenDock ui;
	QGraphicsScene* mpScene;
	GSScreen* mpPreScreen;
	QSize mSize;
};

#endif
