#pragma once

#include <QWidget>
#include "ModDeclare.h"

class QGraphicsScene;
class ViewScale;
class EditorView;

class GSView : public QWidget
{
Q_OBJECT
public:
	GSView(QWidget* parent);
	~GSView();
	void SetScene(QGraphicsScene* pScene);
public slots:
	void FitInView(const QRectF& rect);
	void SelRectChanged(const QRectF& rect);
private slots:
	void SceneRectChanged(const QRectF& rect);
private:
	void Init();
	void Clear();
	void BindSignal();
private:
	ViewScale*  mpXScale;
	ViewScale*  mpYScale;
	EditorView* mpView;
};
M_DECL_MOD(GSView)

