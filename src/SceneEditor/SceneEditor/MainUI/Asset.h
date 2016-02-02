#ifndef ASSET_H
#define ASSET_H

namespace Asset
{
	//Mime描述字符串
	const char * const IMAGE = "app/image";
	const char*  const EFFECT="app/effect";

	//函数定义
	bool IsSupportPreview(const QString& suffix);
	bool IsSupportDrag(const QString& suffix);
	QString    MimeDesc(const QString& suffix);
	QByteArray Encode(const QString& str);
	QString    Decode(const QByteArray& array);
}


#endif
