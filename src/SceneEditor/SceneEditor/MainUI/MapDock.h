#pragma once

#include <QDockWidget>
#include "ui_MapDock.h"

class MapDock : public QDockWidget
{
Q_OBJECT
public:
	MapDock(QWidget* parent);
	~MapDock();
signals:
	void FileChanged(const QString& path);
	void StateChanged(const QString& desc);
public slots:
	void SetPath(const QString& val);
	void Modified();
	void Close();
private slots:	
	void Add(bool state);
	void New();
	void Save();
	void Load();
	void Load(const QString& file);
	void Delete();
	void Select(const QModelIndex&,const QModelIndex&);
	void Search(const QString& val);
	void NameChanged(const QString& val);
private:
	void Init();
	void InitView();
	void BindSignal();
	void NotifyMapChanged();
	void NotifyMapStateChanged();
	void InitAction();
	void UpdateAction();
	void UpdateRecent();
	QString FilePath() const;
	QString FilePath(const QString& fileName) const;
	QString FileName() const;
	void ReadScreenInfo();
	void WriteScreenInfo();
	void AddCurMaptoRecent();
private:
	Ui::MapDock ui;
	QString mPath;
	QString mCurMap;
	QString mSelMap;
	bool mIsModify;
	class QFileSystemModel* mpModel;
	//Action
	QAction* mpSave;
	QAction* mpClose;
	QMenu* mpRecent;
};