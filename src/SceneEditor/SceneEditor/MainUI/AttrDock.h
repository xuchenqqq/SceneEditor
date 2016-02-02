#pragma once

#include <QDockWidget>
#include "ModDeclare.h"

class AttrDock : public QDockWidget
{
Q_OBJECT
public:
	AttrDock(QWidget * parent);
	~AttrDock();
	void SetWidget(QWidget* pWidget);
	void ClearWidget();
private:
	void Init();
	void DeleteWidget();
private:
	QWidget* pBlank;
};

M_DECL_MOD(AttrDock)

