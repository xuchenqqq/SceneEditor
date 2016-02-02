#include "PreHeader.h"
#include "AssetModel.h"
#include <QFileInfo>
#include "Asset.h"
#include "Macro.h"
#include "Path.h"
#include "AppModAlias.h"

AssetModel::AssetModel(QObject * parent)
: QFileSystemModel(parent)
{
	Init();
}
Qt::ItemFlags AssetModel::flags(const QModelIndex &index) const{
	QFileInfo info = fileInfo(index);
	Qt::ItemFlags flags = QFileSystemModel::flags(index);
	if( info.isFile() && Asset::IsSupportDrag(info.suffix())){
		SET_BIT(flags,Qt::ItemIsDragEnabled);
	}else{
		CLR_BIT(flags,Qt::ItemIsDragEnabled); 
	}
	return flags;
}
QMimeData* AssetModel::mimeData(const QModelIndexList &indexes) const{
     QMimeData *mimeData = new QMimeData();
	 FAIL_RET_X(!indexes.isEmpty(),mimeData);
	 QFileInfo info = fileInfo(indexes[0]);
	 QString relFilePath = sPath.InnerAssetPath(info.absoluteFilePath());
	 mimeData->setData(Asset::MimeDesc(info.suffix()),
	 				   Asset::Encode(relFilePath));
     return mimeData;
}
void AssetModel::Init(){
	setNameFilterDisables(false);
}

