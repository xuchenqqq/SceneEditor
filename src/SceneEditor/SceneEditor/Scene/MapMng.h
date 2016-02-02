#pragma once

#include <QObject>
#include <QString>

class QAction;

class MapMng : public QObject
{
Q_OBJEECT
public:
	MapMng(QObject* parent);
	~MapMng();
signals:
	void FileChanged(const QString& path);
	void StateChanged(const QString& desc);
public slots:
	void SetPath(const QString& val);
	void Modified();
private slots:
	void New();
	void Load();
	void Save();
	void Close();
private:
	void NotifyState();
	void Init();
	void InitAction();
	void BindSignal();
	void PathInvalid();
	void FileValid();
	void FileInvalid();
	QString FilePath() const;
private:
	QString mPath;
	QString mCurMap;
	bool mIsModify;
	//Action
	QAction* mpNew;
	QAction* mpSave;
	QAction* mpLoad;
	QAction* mpClose;
};
