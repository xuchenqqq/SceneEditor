#include "PreHeader.h"
#include "Cache.h"
#include "Path.h"
#include <assert.h>
#include "Macro.h"
#include "Log.h"
#include "CacheObs.h"

const QPixmap& Cache::Pixmap(const QString& key){
	ASSERT_MSG_RET_X(mpPath->IsValid(),"路径无效",*((QPixmap*)NULL));
	if(!mPixmapCache.contains(key)){
		ASSERT_MSG_RET_X(Refresh(key),"Cache插入Pixmap失败",*((QPixmap*)NULL));
	}
	return *(mPixmapCache[key]);
}
bool Cache::Refresh(const QString& key){
    mPixmapCache.remove(key);
    QString fullAPath = mpPath->FullAssetPath(key);
    QString fullOPath = mpPath->FullObjectPath(key);
    QFile file(fullAPath);
    if (file.exists()){
        return mPixmapCache.insert(key,new QPixmap(fullAPath));
    }else{
        return mPixmapCache.insert(key,new QPixmap(fullOPath));
    }
}
Cache::Cache(Path* pPath)
: mpPath(pPath)
{
	assert(mpPath);
	BindSignal();
}
Cache::~Cache(){
	Clear();
}
void Cache::Clear(){
	mFSMonitor.removePaths(mFSMonitor.files());
	mPixmapCache.clear();
	mObsMap.clear();
}
void Cache::Bind(const QString& key,CacheObs* pObs)
{
    ASSERT_MSG_RET_VOID(pObs,"无效指针");
    QString fullAPath = mpPath->FullAssetPath(key);
    QString fullOPath = mpPath->FullObjectPath(key);
    QFile file(fullAPath);
    if(!mObsMap.contains(key)){
        if(file.exists()){
            if(mFSMonitor.addPath(fullAPath)){
                mObsMap.insert(key,pObs);
            }else{
                Log::Info(key+" 资源文件监控失败\n");
            }
        }else{
            if(mFSMonitor.addPath(fullOPath)){
                mObsMap.insert(key,pObs);
            }else{
                Log::Info(key+" 资源文件监控失败\n");
            }
        }
    }else{
        ASSERT_MSG_RET_VOID(!mObsMap.contains(key,pObs),"重复添加");
        mObsMap.insert(key,pObs);
    }
}
void Cache::BindSignal(){
	bool res = false;
	res = connect(&mFSMonitor,SIGNAL(fileChanged(const QString&)),
	              SLOT(FileModified(const QString&))); assert(res);
}
void Cache::FileModified(const QString& path){
	QString key = mpPath->InnerAssetPath(path);
	FAIL_RET_VOID(!key.isEmpty());
	QList<CacheObs*> obsList = mObsMap.values(key);
	ASSERT_MSG_RET_VOID(!obsList.isEmpty(),"无效文件关注");
	Refresh(key);
	foreach(CacheObs* pObs,obsList){
		pObs->Update();
    }
}
void Cache::UnBind(const QString& key,CacheObs* pObs){
    ASSERT_MSG_RET_VOID(pObs,"无效指针");
    mObsMap.remove(key,pObs);
    QString fullAPath = mpPath->FullAssetPath(key);
    QString fullOPath = mpPath->FullObjectPath(key);
    QFile file(fullAPath);
    if(!mObsMap.contains(key)){
        if (file.exists()){
            if(!mFSMonitor.removePath(fullAPath)){
                Log::Info(key+" 资源文件取消监控失败\n");
            }
        }else{
            if(!mFSMonitor.removePath(fullOPath)){
                Log::Info(key+" 资源文件取消监控失败\n");
            }
        }
    }
}
