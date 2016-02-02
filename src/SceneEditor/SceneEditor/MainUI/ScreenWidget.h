#pragma once

#include <QWidget>
#include "ui_ScreenWidget.h"

class GSScreen;

class ScreenWidget : public QWidget
{
Q_OBJECT
public:
	ScreenWidget(GSScreen* pScreen,const QString& name);
	~ScreenWidget();
	GSScreen* Screen() const{return mpScreen;}
private slots:
	void OnVisibleToggle(bool state);
private:
	void InitUI(const QString& name);
	void SetVisibleBtn(bool state);
private:
	GSScreen* mpScreen;
	Ui::ScreenWidget ui;
};
