#include "PreHeader.h"
#include "GSView.h"
#include "ViewScale.h"
#include "EditorView.h"
#include <QGridLayout>
#include "AppModAlias.h"
#include "GSInfo.h"
#include "GSScreen.h"
#include "qwt_scale_engine.h"
#include <QRectF>
#include <assert.h>

GSView::GSView(QWidget* parent)
: QWidget(parent)
{
	Init();
	BindSignal();
}
GSView::~GSView(){
	Clear();
}
void GSView::SetScene(QGraphicsScene* pScene){
	mpView->setScene(pScene);
}
void GSView::FitInView(const QRectF& rect){
	mpView->FitInView(rect);
}
void GSView::SelRectChanged(const QRectF& rect){
	mpXScale->SetCursor(rect.left(),rect.right());
	mpYScale->SetCursor(-rect.bottom(),-rect.top());
}
void GSView::SceneRectChanged(const QRectF& rect){
	GSScreen* pScreen = sGSInfo.Screen();
	QRectF sRect(rect);
	if(pScreen){
		sRect = pScreen->mapRectFromScene(rect);
	}
	mpXScale->SetAxisScale(sRect.left(),sRect.right());
	mpYScale->SetAxisScale(-sRect.bottom(),-sRect.top());
}
void GSView::Init(){
	mpXScale = new ViewScale(QwtScaleDraw::BottomScale,this);
	mpYScale = new ViewScale(QwtScaleDraw::LeftScale,this);
	mpView = new EditorView(this);
	mpView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	QGridLayout* pLayout = new QGridLayout(this);
	pLayout->addWidget(mpYScale,0,0);
	pLayout->addWidget(mpView,0,1);
	pLayout->addWidget(mpXScale,1,1);
}
void GSView::Clear(){
}
void GSView::BindSignal(){
	bool res = false;
	res = connect(mpView,&EditorView::SceneRectChanged,
		               this,&GSView::SceneRectChanged); assert(res);
}


