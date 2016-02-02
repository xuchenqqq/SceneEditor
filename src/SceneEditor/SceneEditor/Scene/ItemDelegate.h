#pragma once

#include <QStyledItemDelegate>

class ItemDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
	ItemDelegate(QObject * parent);
	~ItemDelegate();
	QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& idx)const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& idx)const;
	void setEditorData(QWidget* editor,const QModelIndex& idx)const;
	void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& idx)const;
private slots:
	void CommitData();
	void CloseEditor();
private:
	void DrawBackgroud(QPainter* painter, const QStyleOptionViewItem& option) const;
};
