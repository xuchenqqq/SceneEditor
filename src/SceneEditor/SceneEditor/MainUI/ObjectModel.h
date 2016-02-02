#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QFileSystemModel>

class ObjectModel : public QFileSystemModel
{
public:
    ObjectModel(QObject* parent);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
private:
    void Init();
};

#endif // OBJECTMODEL_H
