#ifndef EDITOR_SCENE_H
#define EDITOR_SCENE_H

#include <QGraphicsScene>
#include "ModDeclare.h"
#include <QList>

class QAction;
class QUndoStack;
class GSItem;
class QUndoCommand;
class ItemSignal;

class EditorScene : public QGraphicsScene
{
Q_OBJECT
public:
	EditorScene(QObject* parent);
	~EditorScene();
	void PushCmd(QUndoCommand* pCmd);
signals:
	void Modify();
private slots:
	void SelectionChanged();
	void AlignTop();
	void AlignDown();
	void AlignLeft();
	void AlignRight();
	void AlignCenter();
	void QueueTop();
	void QueueDown();
	void QueueLeft();
	void QueueRight();
	void Scale();
	void OrderTop();
	void OrderBottom();
	void Delete();
	void Redo();
	void Undo();
	void Copy();
	void Paste();
private:
	void Init();
	void InitAction();
	void BindSignal();
	QList<GSItem*> SelectItems() const;
	void Clear();
	void SetItemSignal(ItemSignal*);
	void ClearItemSignal();
private:
	//Action
	QAction* mpAlignTop;
	QAction* mpAlignDown;
	QAction* mpAlignRight;
	QAction* mpAlignLeft;
	QAction* mpAlignCenter;
	QAction* mpQueueTop;
	QAction* mpQueueDown;
	QAction* mpQueueRight;
	QAction* mpQueueLeft;
	QAction* mpScale;
	QAction* mpOrderTop;
	QAction* mpOrderBottom;
	QAction* mpDelete;
	QAction* mpRedo;
	QAction* mpUndo;
	QAction* mpCopy;
	QAction* mpPaste;
	//Undo Stack
	QUndoStack* mpUndoStack;
	class ItemClipboard* mpClipboard;
    class GSInfo* mpInfo;
	ItemSignal* mpItemSignal;
};

M_DECL_MOD(EditorScene)

#endif
