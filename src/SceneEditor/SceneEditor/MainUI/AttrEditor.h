#pragma once

#include <QWidget>
#include "ui_AttrEditor.h"
#include <QMap>

class GSItem;

class AttrEditor : public QWidget
{
Q_OBJECT
public:
	AttrEditor(GSItem* pItem);
	~AttrEditor();
	void UpdateUI(const QString& item=QString());
private slots:
	void OnPos();
	void OnScale();
	void OnImage();
	void OnFlipX(int stat);
	void OnFlipY(int stat);
    void AddCustomAttr();
    void DelCustomAttr();
    void RefreshCustomAttr();
private:
	void BindSignal();
    void Init();
    void IsEdit();
private:
	Ui::AttrEditor ui;
	bool mIsPermitSignal;
	GSItem* mpItem;
    int row;
};
