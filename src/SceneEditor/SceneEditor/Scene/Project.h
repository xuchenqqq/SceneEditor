#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QDir>

class QAction;
class QMenu;

class Project : public QObject
{
Q_OBJECT
public:
	Project(QObject* parent);
	~Project();
signals:
    void OpenProject(const bool isOpen);
    void AssetDirChanged(const QString& path);
    void ObjectDirChanged(const QString& path);
	void MapDirChanged(const QString& path);
public slots:
	void New();
	void Load();
	void Zip();
private:
	void Init();
	void InitAction();
	void BindSignal();
	void UpdateState();
	void UpdateRecent();
	void Load(const QString& path);
private:
	class Path* mpPath;
	class Cache* mpCache;
	//Action
	QAction* mpNew;
	QAction* mpLoad;
	QAction* mpZip;
	QMenu*  mpRecent;
};

#endif
