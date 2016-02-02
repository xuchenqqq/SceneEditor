#include "PreHeader.h"
#include "EffectModel.h"
#include <QFileInfo>
#include <QMimeData>
#include "Macro.h"
#include "Asset.h"
#include "AppModAlias.h"
#include "Path.h"

EffectModel::EffectModel(QObject * parent)
: QFileSystemModel(parent)
{
    Init();
}
Qt::ItemFlags EffectModel::flags(const QModelIndex &index) const{
    QFileInfo info = fileInfo(index);
    Qt::ItemFlags flags = QFileSystemModel::flags(index);
    if(info.isDir()){
        SET_BIT(flags,Qt::ItemIsDragEnabled);
    }else{
        CLR_BIT(flags,Qt::ItemIsDragEnabled);
    }


    //SET_BIT(flags,Qt::ItemIsDragEnabled);
    return flags;
}
//QMimeData* EffectModel::mimeData(const QModelIndexList &indexes) const{
////    QMimeData *mimeData = new QMimeData();
////    FAIL_RET_X(!indexes.isEmpty(),mimeData);
////    QFileInfo info = fileInfo(indexes[0]);
////    QString relFilePath = sPath.InnerEffectPath(info.absoluteFilePath());
////    mimeData->setData(Asset::EFFECT,Asset::Encode(relFilePath));
////    return mimeData;
//}
void EffectModel::Init(){
    //setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::Files);
    setNameFilterDisables(false);
}

