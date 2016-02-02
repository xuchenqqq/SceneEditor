#pragma once

#include <QAbstractTableModel>
#include <QList>

class GSLayer;
class GSScreen;

class LayerModel : public QAbstractTableModel
{
Q_OBJECT
public:
	LayerModel(QObject* parent);
	~LayerModel();
	int rowCount(const QModelIndex& parent=QModelIndex())const;
	int columnCount(const QModelIndex& parent=QModelIndex())const;
	QVariant data(const QModelIndex& idx,int role=Qt::DisplayRole)const;
	bool setData(const QModelIndex& idx, const QVariant& value,int role=Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex& idx) const;
	QMimeData* mimeData(const QModelIndexList& indexes) const;
	QStringList	mimeTypes() const;
	bool dropMimeData(const QMimeData* data,Qt::DropAction action,int row,int column,const QModelIndex& parent);
	GSLayer* Layer(int idx) const;
	int Count() const{return mList.size();}
	Qt::DropActions supportedDropActions() const;
public slots:
	void SetScreen(GSScreen* pScreen);
	void Remove(int idx);
	void Insert(int idx);
	void Add();
    void AddObject();
private slots:
	void Inserted(GSLayer* pLayer,GSLayer* pPostLayer);
	void Removed(GSLayer* pLayer);
	void Moved(GSLayer* pLayer,GSLayer* pPostLayer);
private:
	void CheckSameWithScene();
private:
	GSScreen* mpScreen;
	QList<GSLayer*> mList;
};
