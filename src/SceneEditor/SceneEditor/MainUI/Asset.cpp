#include "PreHeader.h"
#include "Asset.h"
#include <QString>
#include <QByteArray>
#include "Macro.h"

namespace Asset
{
	struct Suffix2MimeItem{
		const char** list;
		int   len;
		const char* mime;
	};
	#define DEF_S2M_ITEM(ls,mine) {ls,LEN_OF_ARRAY(ls),mine}
	struct SuffixPreviewItem{
		const char** list;
		int len;
	};
	#define DEF_SP_ITEM(ls) {ls,LEN_OF_ARRAY(ls)}
	
	//在下面添加新的文件后缀类型和映射表
	const char * sImgFile[] ={
		"PNG",
		"BMP",
		"JPG",
	};
	const Suffix2MimeItem sS2MList[] ={
		DEF_S2M_ITEM(sImgFile,IMAGE),
	};
	
	const SuffixPreviewItem sSPList[] ={
		DEF_SP_ITEM(sImgFile),
	};
	
	bool IsSupportPreview(const QString& suffix){
		QString sufUpper = suffix.toUpper();
        for(int i = 0; i<LEN_OF_ARRAY(sSPList); ++i){
			for(int j = 0; j<sSPList[i].len; ++j){
				if(sufUpper == sSPList[i].list[j])
					return true;
			}
		}
		return false;
	}
	QString  MimeDesc(const QString& suffix){
		QString sufUpper = suffix.toUpper();
		for(int i = 0; i<LEN_OF_ARRAY(sS2MList); ++i){
			for(int j = 0; j<sS2MList[i].len; ++j){
				if(sufUpper == sS2MList[i].list[j])
					return sS2MList[i].mime;
			}
		}
		return QString();
	}
	bool IsSupportDrag(const QString& suffix){
		return MimeDesc(suffix).isEmpty()==false;
	}
	QByteArray Encode(const QString& str){
		return str.toUtf8();
	}
	QString    Decode(const QByteArray& array){
		return QString::fromUtf8(array.data(),array.length());
	}
};
