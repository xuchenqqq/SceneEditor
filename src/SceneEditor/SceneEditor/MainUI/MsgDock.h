#ifndef MSG_DOCK_H
#define MSG_DOCK_H

#include <QDockWidget>
#include "ui_MsgDock.h"
#include "ModDeclare.h"

class MsgDock : public QDockWidget
{
Q_OBJECT
public:
	MsgDock(QWidget* parent);
	~MsgDock();
public slots:
	void AddMessageLn(const QString & msg);
	void AddMessage(const QString & msg);
	void ClearMessage();
private:
	void ConnectSignal();
private:
	Ui::MsgDock ui;
};

M_DECL_MOD(MsgDock)

#endif
