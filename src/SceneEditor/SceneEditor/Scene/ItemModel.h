#pragma once

#include <QAbstractTableModel>
#include <QList>

class GSLayer;
class GSItem;

class ItemModel : public QAbstractTableModel
{
Q_OBJECT
public:
	ItemModel(QObject* parent);
	~ItemModel();
	int rowCount(const QModelIndex& parent=QModelIndex())const;
	int columnCount(const QModelIndex& parent=QModelIndex())const;
	QVariant data(const QModelIndex& idx,int role=Qt::DisplayRole)const;
	bool setData(const QModelIndex& idx, const QVariant& value,int role=Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex& idx) const;
	QMimeData* mimeData(const QModelIndexList& indexes) const;
	QStringList mimeTypes() const;
	bool dropMimeData(const QMimeData* data,Qt::DropAction action,int,int,const QModelIndex& parent);
	GSItem* Item(int idx) const;
	int Count() const{return mList.size();}
	Qt::DropActions supportedDropActions() const;
public slots:
	void SetLayer(GSLayer* pLayer);
private slots:
	void Inserted(GSItem* pItem,GSItem* pPostItem);
	void Removed(GSItem* pItem);
	void Moved(GSItem* pItem,GSItem* pPostItem);
private:
	void CheckSameWithScene();
private:
	GSLayer* mpLayer;
	QList<GSItem*> mList;
};