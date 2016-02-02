#include "PreHeader.h"
#include "ScaleDlg.h"
#include <assert.h>

ScaleDlg::ScaleDlg(){
	ui.setupUi(this);
	BindSignal();
	ui.scaleVal->setValue(100);
}
ScaleDlg::~ScaleDlg(){

}
qreal ScaleDlg::Scale() const{
	return qreal(ui.scaleVal->value())/100.0;
}
void ScaleDlg::SliderChanged(int val){
	if(val != ui.scaleVal->value()){
		ui.scaleVal->setValue(val);
	}
}
void ScaleDlg::SpinBoxChanged(int val){
	if(val != ui.scaleBar->value()){
		ui.scaleBar->setValue(val);
	}
}
void ScaleDlg::BindSignal(){
	bool res = false;
	res = connect(ui.scaleBar,SIGNAL(valueChanged(int)),SLOT(SliderChanged(int))); assert(res);
	res = connect(ui.scaleVal,SIGNAL(valueChanged(int)),SLOT(SpinBoxChanged(int))); assert(res);
	res = connect(ui.ok,SIGNAL(clicked()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(reject())); assert(res);
}