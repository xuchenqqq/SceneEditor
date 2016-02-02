#include "PreHeader.h"
#include "AuthDlg.h"
#include "Auth.h"
#include <assert.h>
#include "Macro.h"
#include "Setting.h"
#include "AppModAlias.h"

const char* const SETTING_USER = "Auth/user";
const char* const SETTING_SN   = "Auth/sn";

AuthDlg::AuthDlg(Auth* pAuth)
: mTimerId(0)
, mpAuth(pAuth)
{
	//关闭对话框入口无效
	setWindowFlags((Qt::WindowFlags)(~Qt::WindowCloseButtonHint & windowFlags()));
	ui.setupUi(this);
	ReadSetting();
	BindSignal();
}
AuthDlg::~AuthDlg(){
}
void AuthDlg::Init(bool isWaitReply,const QString& desc){
	ui.hint->setText(desc);
	if(isWaitReply){
		WaitReply();
	}else{
		WaitInput();
	}
}
void AuthDlg::Notify(AuthOp::Type op,const QString& desc){
	ui.hint->setText(desc);
	if(mIsWaitReply){
		switch(op){
			case AuthOp::INPUT: WaitInput(); break;
			case AuthOp::ACCEPT: accept(); break;
			default: break;
		}
	}
}
void AuthDlg::Check(){
	WriteSetting();
	mpAuth->Request(ui.user->text(),ui.sn->text());
	WaitReply();
}
void AuthDlg::Quit(){
	reject();
}
void AuthDlg::timerEvent(QTimerEvent*){
	QString sign = ui.workSign->text();
	if(sign=="-"){
		ui.workSign->setText("/");
	}else if(sign=="/"){
		ui.workSign->setText("|");
	}else if(sign=="|"){
		ui.workSign->setText("\\");
	}else{
		ui.workSign->setText("-");
	}
}
void AuthDlg::StartWaiting(){
	if(!mTimerId){
		mTimerId = startTimer(100); //0.1s
		ASSERT_MSG_RET_VOID(mTimerId,"分配不到定时器了");
	}
}
void AuthDlg::StopWaiting(){
	if(mTimerId){
		killTimer(mTimerId);
		mTimerId = 0;
	}
}
void AuthDlg::PermitInput(){
	ui.user->setEnabled(true);
	ui.sn->setEnabled(true);
	ui.check->setEnabled(true);
	ui.quit->setEnabled(true);
}
void AuthDlg::ForbidInput(){
	ui.user->setEnabled(false);
	ui.sn->setEnabled(false);
	ui.check->setEnabled(false);
	ui.quit->setEnabled(false);
}
void AuthDlg::WaitInput(){
	mIsWaitReply = false;
	PermitInput();
	StopWaiting();
}
void AuthDlg::WaitReply(){
	mIsWaitReply = true;
	ForbidInput();
	ui.hint->setText(QStringLiteral("等待服务器响应"));
	StartWaiting();
}
bool AuthDlg::IsInputValid() const{
	return !(ui.user->text().isEmpty() || ui.sn->text().isEmpty());
}
void AuthDlg::ReadSetting(){
	QString user = sSetting.value(SETTING_USER).toString();
	QString sn   = sSetting.value(SETTING_SN).toString();
	ui.user->setText(user);
	ui.sn->setText(sn);
}
void AuthDlg::WriteSetting(){
	sSetting.setValue(SETTING_USER,ui.user->text());
	sSetting.setValue(SETTING_SN,ui.sn->text());
}
void AuthDlg::BindSignal(){
	bool res = false;
	res = connect(ui.check,&QPushButton::clicked,
		   		  this,&AuthDlg::Check); assert(res);
	res = connect(ui.quit,&QPushButton::clicked,
				  this,&AuthDlg::Quit); assert(res);
}


