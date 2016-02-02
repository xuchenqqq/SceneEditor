#include "PreHeader.h"
#include "EditorView.h"
#include <assert.h>
#include "ActionMng.h"
#include "AppModAlias.h"
#include "GSInfo.h"
#include <QAction>
#include <QToolBar>

const qreal MIN_SCALE = 0.1;
const qreal MAX_SCALE = 10.0;
const qreal STEP_SCALE = 0.1;

EditorView::EditorView(QWidget* parent)
: QGraphicsView(parent)
, mScale(1.0)
, mpSelect(NULL)
, mpScrollDrag(NULL)
{
	Init();
	BindSignal();
}
void EditorView::wheelEvent(QWheelEvent* event){
	if(event->modifiers() & Qt::ControlModifier){
		if(event->delta() > 0){
			if(mScale<MAX_SCALE-STEP_SCALE) mScale+=STEP_SCALE;
		}else{
			if(mScale>MIN_SCALE+STEP_SCALE) mScale-=STEP_SCALE;
		}
		setMatrix(QMatrix().scale(mScale,mScale));
	}else{
		QGraphicsView::wheelEvent(event);
	}
	NotifySceneRect();
}
void EditorView::Init(){
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMouseTracking(true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	InitAction();
	Select();
}
void EditorView::FitInView(const QRectF& rect){
	mDefRect = rect;
	FitInView();
}
void EditorView::FitInView(){
	fitInView(mDefRect,Qt::KeepAspectRatio);
	NotifySceneRect();
}
void EditorView::Select(){
	mpSelect->setChecked(true);
	mpScrollDrag->setChecked(false);
	setInteractive(true);
	setDragMode(QGraphicsView::RubberBandDrag);
}
void EditorView::ScrollDrag(){
	mpSelect->setChecked(false);
	mpScrollDrag->setChecked(true);
	setInteractive(false);
	setDragMode(QGraphicsView::ScrollHandDrag);
}
void EditorView::InitAction(){
	ActionMng& actMng = sActionMng;
	actMng.BeginPart(QStringLiteral("视图"),false,true);
	mpSelect = actMng.AddAction(QIcon(":/Icon/select.png"),QStringLiteral("选择"));
	mpScrollDrag = actMng.AddAction(QIcon(":/Icon/scrollDrag.png"),QStringLiteral("拖动"));
	mpFitInView = actMng.AddAction(QIcon(":/Icon/fitIn.png"),QStringLiteral("适配视图"));
	actMng.EndPart();
	mpSelect->setCheckable(true);
	mpScrollDrag->setCheckable(true);
}
void EditorView::BindSignal(){
	bool res = false;
	res = connect(mpSelect,SIGNAL(triggered()),SLOT(Select())); assert(res);
	res = connect(mpScrollDrag,SIGNAL(triggered()),SLOT(ScrollDrag())); assert(res);
	res = connect(mpFitInView,SIGNAL(triggered()),SLOT(FitInView())); assert(res);
}
void EditorView::NotifySceneRect(){
	QSize sz = size();
	QPointF topLeft = mapToScene(0,0);
	QPointF bottomRight = mapToScene(sz.width(),sz.height());
	emit SceneRectChanged(QRectF(topLeft,bottomRight));
}
void EditorView::mouseMoveEvent(QMouseEvent * event){
	sGSInfo.SetSceneMousePos(mapToScene(event->pos()));
	QGraphicsView::mouseMoveEvent(event);
	if(dragMode()==QGraphicsView::ScrollHandDrag){
		NotifySceneRect();
	}
}


