#pragma once

#include "ModDeclare.h"
#include <QString>
#include <QDir>

class Path
{
public:
	bool IsValid() const{return !mPath.isEmpty();}
	QString AssetPath() const;
	QString MapPath() const;
    QString ObjectPath() const;
	QString Root() const{return mPath;}
	QString ZipFilePath(const QString& fileName) const;
	QString WorkPath() const;
	QString ZipPath() const;
	QString InnerAssetPath(const QString fullPath) const;
	QString FullAssetPath(const QString innerPath) const;
    QString InnerObjectPath(const QString fullPath) const;
    QString FullObjectPath(const QString innerPath) const;
	QString LastProject() const;
	QStringList RecentProject() const;
	QStringList RecentMap() const;
	void AddRecentMap(const QString& map);
private:
	Path();
	~Path();
	bool NewPath(const QString& path,const QString& folderName);
	bool LoadPath(const QString& path);
	void ReadSettings();
	void WriteSettings();
	void AddRecentProject(const QString& project);
private:
	QString mPath;
	QDir mAssetDir;
//	QDir mEffectDir;
    QDir mObjectDir;
	QStringList mRectProject;
	QMap<QString,QStringList> mRectMap;
	friend class Project;
};
M_DECL_MOD(Path)
