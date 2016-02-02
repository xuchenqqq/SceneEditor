#include "ObjectModel.h"
#include "PreHeader.h"
#include "EffectModel.h"
#include <QFileInfo>
#include <QMimeData>
#include "Macro.h"
#include "Asset.h"
#include "AppModAlias.h"
#include "Path.h"

ObjectModel::ObjectModel(QObject* parent)
    :QFileSystemModel(parent)
{
    Init();
}

Qt::ItemFlags ObjectModel::flags(const QModelIndex &index) const
{
    QFileInfo info = fileInfo(index);
    Qt::ItemFlags flags = QFileSystemModel::flags(index);
    if(info.isDir())
    {
        CLR_BIT(flags,Qt::ItemIsDragEnabled);
    }
    else
    {
        SET_BIT(flags,Qt::ItemIsDragEnabled);
    }
    return flags;
}

QMimeData* ObjectModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = new QMimeData();
    FAIL_RET_X(!indexes.isEmpty(),mimeData);
    QFileInfo info = fileInfo(indexes[0]);
    QString relFilePath = sPath.InnerObjectPath(info.absoluteFilePath());
    mimeData->setData(Asset::EFFECT,Asset::Encode(relFilePath));
    return mimeData;
}

void ObjectModel::Init(){
//    setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::Files);
    setNameFilterDisables(false);
}
