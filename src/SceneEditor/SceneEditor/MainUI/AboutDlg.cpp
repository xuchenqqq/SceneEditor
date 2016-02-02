#include "PreHeader.h"
#include "AboutDlg.h"
#include "Setting.h"
#include "AppModAlias.h"
#include <assert.h>

const char* const SETTING_USER = "Auth/user";
const char* const SETTING_SN   = "Auth/sn";

AboutDlg::AboutDlg()
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
AboutDlg::~AboutDlg(){
}
void AboutDlg::BindSignal(){
	bool res = false;
	res = connect(ui.ok,&QPushButton::clicked,
				this,&AboutDlg::accept); assert(res);
}
void AboutDlg::Init(){
	QString user = sSetting.value(SETTING_USER).toString();
	QString sn   = sSetting.value(SETTING_SN).toString();
	ui.user->setText(user);
	ui.sn->setText(sn);
}


