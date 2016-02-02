#ifndef ASSET_MNG_DOCK_H
#define ASSET_MNG_DOCK_H

#include <QDockWidget>
#include "AssetModel.h"
#include "ui_AssetDock.h"

class AssetDock : public QDockWidget
{
	Q_OBJECT
public:
	AssetDock(QWidget * parent);
	~AssetDock();
public slots:
	void SetPath(const QString& path);
signals:
	void SelectFile(const QString& path);
private slots:
	void AddFile();
	void AddFolder();
	void DelAsset();
	void Filter(const QString& str);
	void ExploreAsset();
	void CurrentChanged(const QModelIndex&,const QModelIndex&);
private:
	QString CurrentPath();
	void Init();
	void InitView();
	void BindSignal();
	bool IsImageFile(const QFileInfo& info) const;
	void InitAction();
	void Preview(const QString& path);
	void LoadImage(const QSize& sz);
	void UpdateAction();
private:
	QString mPath;
	AssetModel mModel;
	Ui::AssetDock ui;
	QString mFilePath;
	//Action
	QAction* mpAddFile;
	QAction* mpAddFolder;
	QAction* mpDelAsset;
	QAction* mpExplore;
};

#endif