#ifndef SceneEditor_H
#define SceneEditor_H

#include <QMainWindow>
#include "ui_SceneEditor.h"
#include <QTimer>

class QAction;

class SceneEditor : public QMainWindow
{
	Q_OBJECT
public:
	SceneEditor(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SceneEditor();
private:
	void Init();
	void InitUI();
	void InitMod();
	void InitMainWin();
	void Clear();
	void ClearMod();
	void ClearUI();
	void BindSignal();
	void InitAction();
	void closeEvent(QCloseEvent *event);

    void ChangeMpCoutFormat(const bool isOpen);
private slots:
	void Quit();
	void PreTask();
	void HotkeyConf();
	void About();
    void CoutFormat();
private:
	//UI
	Ui::SceneEditor ui;
	//功能模块
	class DockMng* mpDockMng;
	class AssetDock* mpAssetDock;
//	class EffectDock* mpEffectDock;
    class ObjectDock* mpObjectDock;
	class MsgDock*  mpMsgDock;
	class ScreenDock* mpScreenDock;
	class LayerDock* mpLayerDock;
	class ItemDock* mpItemDock;
	class MapDock* mpMapDock;
	class AttrDock* mpAttrDock;
	class GSView* mpView;
	class EditorScene* mpScene;
	class Project* mpProject;
	class ActionMng* mpActionMng;
	class ScreenMng* mpScreenMng;
	class Simulator* mpSimulator;
	class QSettings* mpSetting;
	class Auth* mpAuth;
	class UIAccess* mpUIAccess;
	class TimerService* mpTimer;
	//Action
	QAction* mpQuit;
	QAction* mpAbout;
	QAction* mpPreview;
	QAction* mpHotkey;
    QAction* mpCoutFormat; //json文件输出格式
	QTimer mTaskTimer;
};

#endif // SceneEditor_H
