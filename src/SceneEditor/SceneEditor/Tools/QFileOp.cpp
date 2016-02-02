#include "PreHeader.h"
#include "QFileOp.h"
#include <QFileInfo>
#include <QDir>

namespace QFileOp
{

int DirFileNum(const QDir& dir){
	int sum = 0;
	QFileInfoList infoList = 
		dir.entryInfoList(QDir::Files|QDir::NoSymLinks|QDir::Dirs|QDir::NoDotAndDotDot); 

	foreach(QFileInfo info, infoList){
		if(info.isDir()){
			sum += DirFileNum(info.filePath());
		}else{
			sum++;
		}
	}
	return sum;
}

}