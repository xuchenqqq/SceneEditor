#pragma once

#include <QFileSystemModel>

class EffectModel : public QFileSystemModel
{
Q_OBJECT
public:
    EffectModel(QObject * parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
//    QMimeData* mimeData(const QModelIndexList &indexes) const;
private:
    void Init();
};
