#include "PreHeader.h"
#include "SimConfigDlg.h"
#include <assert.h>

SimConfigDlg::SimConfigDlg()
{
	ui.setupUi(this);
	Init();
}
SimConfigDlg::~SimConfigDlg(){
}
QString SimConfigDlg::CmdLine() const{
	return ui.input->toPlainText();
}
void SimConfigDlg::SetCmdLine(const QString& str){
	ui.input->setPlainText(str);
}
void SimConfigDlg::Init(){
	bool res = false;
	res = connect(ui.ok,SIGNAL(clicked()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(reject())); assert(res);
}