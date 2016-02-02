#ifndef EDITOR_VIEW_H
#define EDITOR_VIEW_H

#include <QGraphicsView>

class QAction;

class EditorView : public QGraphicsView
{
Q_OBJECT
public:
	EditorView(QWidget* parent);
signals:
	void SceneRectChanged(const QRectF& rect);
public slots:
	void FitInView(const QRectF& rect);
	void FitInView();
private slots:
	void Select();
	void ScrollDrag();
private:
	void Init();
	void InitAction();
	void BindSignal();
	void NotifySceneRect();
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
private:
	QRectF mDefRect;
	qreal mScale;
	QAction* mpSelect;
	QAction* mpScrollDrag;
	QAction* mpFitInView;
};


#endif