#pragma once

#include "ModDeclare.h"
#include <QCache>
#include <QPixmap>
#include <QString>
#include <QMultiMap>
#include <QFileSystemWatcher>

class Path;
class CacheObs;

class Cache : public QObject
{
	Q_OBJECT
public:
	const QPixmap& Pixmap(const QString& key);
private:
	Cache(Path* pPath);
	~Cache();
	void Clear();
	void Bind(const QString& key,CacheObs* pObs);
	void UnBind(const QString& key,CacheObs* pObs);
	void BindSignal();
	bool Refresh(const QString& key);
private slots:
	void FileModified(const QString& path);
private:
	QCache<QString,QPixmap> mPixmapCache;
	QMultiMap<QString,CacheObs*> mObsMap;
	QFileSystemWatcher mFSMonitor;
	Path* mpPath;
	friend class Project;
	friend class CacheObs;
};
M_DECL_MOD(Cache)
