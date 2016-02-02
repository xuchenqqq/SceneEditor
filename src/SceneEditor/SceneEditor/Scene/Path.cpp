#include "PreHeader.h"
#include "Path.h"
#include <assert.h>
#include "Macro.h"
#include "Setting.h"
#include "AppModAlias.h"
#include <QMessageBox>

const char* ASSET_INNER_PATH ="scenes/image";
const char* EFFECT_INNER_PATH ="scenes/effect";
const char* MAP_INNER_PATH ="scenes/xml";
const char* MAP_INNER_PATH_JSON = "scenes/json";
const char* WORK_INNER_PATH = "scenes";
const char* ZIP_INNER_PATH ="zip";
const char* OBJECT_INNER_PATH = "scenes/object";

const char* const RECENT_PROJECT_CONFIG="recent/project";
const char* const RECENT_MAP_CONFIG="recent/map";
const int MAX_RECENT_PROJECT = 4;
const int MAX_RECENT_MAP     = 8;

typedef QMap<QString,QVariant> RecentMapType;

extern bool coutFormat;

Path::Path()
{
	ReadSettings();
}
Path::~Path()
{}
bool Path::NewPath(const QString& path,const QString& folderName){
	mPath.clear();
	QDir rootDir(path);
	FAIL_RET(rootDir.exists());
	FAIL_RET(rootDir.mkdir(folderName));
	FAIL_RET(rootDir.cd(folderName));
	FAIL_RET(rootDir.mkpath(ASSET_INNER_PATH));
    FAIL_RET(rootDir.mkpath(OBJECT_INNER_PATH));
	FAIL_RET(rootDir.mkpath(MAP_INNER_PATH));
    FAIL_RET(rootDir.mkpath(MAP_INNER_PATH_JSON));
	FAIL_RET(rootDir.mkpath(ZIP_INNER_PATH));
	mAssetDir.setPath(rootDir.absoluteFilePath(ASSET_INNER_PATH));
	FAIL_RET(mAssetDir.exists());
    mObjectDir.setPath(rootDir.absoluteFilePath(OBJECT_INNER_PATH));
    FAIL_RET(mObjectDir.exists());
	mPath = rootDir.absolutePath();
	AddRecentProject(mPath);
	return true;
}
bool Path::LoadPath(const QString& path){
	mPath.clear();
	QDir rootDir(path);
	FAIL_RET(rootDir.exists());
	if(!rootDir.exists(ASSET_INNER_PATH)){
		FAIL_RET(rootDir.mkpath(ASSET_INNER_PATH));
	}
    if(!rootDir.exists(OBJECT_INNER_PATH)){
        FAIL_RET(rootDir.mkpath(OBJECT_INNER_PATH));
    }
	if(!rootDir.exists(MAP_INNER_PATH)){
		FAIL_RET(rootDir.mkpath(MAP_INNER_PATH));
	}
    if(!rootDir.exists(MAP_INNER_PATH_JSON)){
        FAIL_RET(rootDir.mkpath(MAP_INNER_PATH_JSON));
    }
	if(!rootDir.exists(ZIP_INNER_PATH)){
		FAIL_RET(rootDir.mkpath(ZIP_INNER_PATH));
	}
	mAssetDir.setPath(rootDir.absoluteFilePath(ASSET_INNER_PATH));
	FAIL_RET(mAssetDir.exists());
    mObjectDir.setPath(rootDir.absoluteFilePath(OBJECT_INNER_PATH));
    FAIL_RET(mObjectDir.exists());
	mPath = rootDir.absolutePath();
	AddRecentProject(mPath);
	return true;
}
QString Path::AssetPath() const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return QDir(mPath).absoluteFilePath(ASSET_INNER_PATH);
}

QString Path::MapPath() const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
    QString path;
    if(coutFormat){
        path = QDir(mPath).absoluteFilePath(MAP_INNER_PATH);
    }else{
        path = QDir(mPath).absoluteFilePath(MAP_INNER_PATH_JSON);
    }
    return path;
}
QString Path::ObjectPath() const
{
    FAIL_RET_X(!mPath.isEmpty(),QString());
    return QDir(mPath).absoluteFilePath(OBJECT_INNER_PATH);
}
QString Path::ZipFilePath(const QString& fileName) const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return QDir(ZipPath()).absoluteFilePath(fileName);
}
QString Path::WorkPath() const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return QDir(mPath).absoluteFilePath(WORK_INNER_PATH);
}
QString Path::ZipPath() const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return QDir(mPath).absoluteFilePath(ZIP_INNER_PATH);
}
QString Path::InnerAssetPath(const QString fullPath) const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return mAssetDir.relativeFilePath(fullPath);
}
QString Path::FullAssetPath(const QString innerPath) const{
	FAIL_RET_X(!mPath.isEmpty(),QString());
	return mAssetDir.absoluteFilePath(innerPath);
}
QString Path::InnerObjectPath(const QString fullPath) const{
    FAIL_RET_X(!mPath.isEmpty(),QString());
    return mObjectDir.relativeFilePath(fullPath);
}
QString Path::FullObjectPath(const QString innerPath) const{
    FAIL_RET_X(!mPath.isEmpty(),QString());
    return mObjectDir.absoluteFilePath(innerPath);
}
QString Path::LastProject() const{
	return mRectProject.empty()?".":mRectProject.first();
}
QStringList Path::RecentProject() const{
	return mRectProject;
}
QStringList Path::RecentMap() const{
	return mRectMap.value(mPath);
}
void Path::AddRecentMap(const QString& map){
	QStringList& mapList = mRectMap[mPath];
	mapList.removeAll(map);
	mapList.prepend(map);
	while(mapList.size()>MAX_RECENT_MAP)
		mapList.removeLast();
	WriteSettings();
}
void Path::ReadSettings(){
	QVariant var = sSetting.value(RECENT_PROJECT_CONFIG);
	if(!var.canConvert<QStringList>()){
		mRectProject.clear();
		mRectMap.clear();
		return;
	}
	mRectProject = var.toStringList();
	while(mRectProject.size()>MAX_RECENT_PROJECT)
		mRectProject.removeLast();
	var = sSetting.value(RECENT_MAP_CONFIG);
	if(var.canConvert<RecentMapType>()){
		RecentMapType recentMap = var.toMap();
		foreach(const QString& proj,mRectProject){
			if(!recentMap.contains(proj)) continue;
			QVariant& mapVar = recentMap[proj];
			if(!mapVar.canConvert<QStringList>()) continue;
			mRectMap[proj] = mapVar.toStringList();
		}
	}
}
void Path::WriteSettings(){
	sSetting.setValue(RECENT_PROJECT_CONFIG,mRectProject);
	RecentMapType recentMap;
	foreach(const QString& proj,mRectMap.uniqueKeys()){
		recentMap.insert(proj,mRectMap[proj]);
	}
	sSetting.setValue(RECENT_MAP_CONFIG,recentMap);
}
void Path::AddRecentProject(const QString& project){
	mRectProject.removeAll(project);
	mRectProject.prepend(project);
	while(mRectProject.size()>MAX_RECENT_PROJECT){
		mRectMap.remove(mRectProject.last());
		mRectProject.removeLast();
	}
	WriteSettings();
}

