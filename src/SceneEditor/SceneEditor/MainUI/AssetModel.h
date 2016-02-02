#ifndef ASSET_MODEL_H
#define ASSET_MODEL_H

#include <QFileSystemModel>

class AssetModel : public QFileSystemModel
{
Q_OBJECT
public:
	AssetModel(QObject * parent);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QMimeData* mimeData(const QModelIndexList &indexes) const;
private:
	void Init();
};

#endif