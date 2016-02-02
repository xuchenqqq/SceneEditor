#pragma once

#include <QObject>
#include <QList>
#include <QDockWidget>
#include <QMap>

class QMainWindow;
class QDockWidget;

struct DockInfo{
	Qt::DockWidgetArea area;
	Qt::DockWidgetAreas allowAreas;
	bool visible;
	DockInfo(Qt::DockWidgetArea _area=Qt::LeftDockWidgetArea)
	: area(_area)
	, allowAreas(_area)
	, visible(true)
	{}
	DockInfo(Qt::DockWidgetArea _area,bool _visible)
	: area(_area)
	, allowAreas(_area)
	, visible(_visible)
	{}
	DockInfo(Qt::DockWidgetArea _area,Qt::DockWidgetAreas _allowArea,bool _visible=true)
	: area(_area)
	, allowAreas(_allowArea)
	, visible(_visible)
	{}
};

class DockMng : public QObject
{
Q_OBJECT
public:
	DockMng(QMainWindow* pMainWindow);
	~DockMng();
	void Add(QDockWidget* pDock,const DockInfo& info);
	void Add(QDockWidget* preDock,QDockWidget* pDock,bool visible=true);
public slots:
	void Reset();
private:
	struct Pos{
		int row;
		int col;
		Pos(): row(-1),col(-1){}
		Pos(int _row,int _col): row(_row),col(_col){}
		bool IsValid() const{return (row>=0 && col>=0);}
	};
	Pos Find(QDockWidget* pDock) const;
	void Insert(const Pos& pos,QDockWidget* pDock,const DockInfo& info);
	void AddToWindow();
	void BindAction(QDockWidget* pDock);
	void Init();
private:
	QMainWindow* mpMainWindow;
	typedef QList<QDockWidget*> ROW;
	QList<ROW> mDocks;
	QMap<QDockWidget*,DockInfo> mDockInfos;
};
