#include "PreHeader.h"
#include "MsgDock.h"

MsgDock::MsgDock(QWidget * parent)
: QDockWidget(QStringLiteral("软件运行信息"),parent)
{
	ui.setupUi(this);
	ConnectSignal();
}
MsgDock::~MsgDock(){
}
void MsgDock::AddMessageLn(const QString & msg){
	ui.msg->moveCursor(QTextCursor::End);
	ui.msg->insertPlainText(msg+"\n");
	ui.msg->moveCursor(QTextCursor::End);
}
void MsgDock::AddMessage(const QString & msg){
	ui.msg->moveCursor(QTextCursor::End);
	ui.msg->insertPlainText(msg);
	ui.msg->moveCursor(QTextCursor::End);
}
void MsgDock::ClearMessage(){
	ui.msg->clear();
}
void MsgDock::ConnectSignal(){
	connect(ui.clear,SIGNAL(clicked()),this,SLOT(ClearMessage()));
}
