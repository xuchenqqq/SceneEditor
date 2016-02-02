#include "PreHeader.h"
#include "NewProjectDlg.h"
#include <QFileDialog>
#include <QDir>
#include <assert.h>

NewProjectDlg::NewProjectDlg(QWidget* parent)
: QDialog(parent)
{
	ui.setupUi(this);
	Init();
	BindSignal();
}
NewProjectDlg::~NewProjectDlg(){
}
void NewProjectDlg::SelectDir(){
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目录"),ui.dir->text(),
	                                                QFileDialog::ShowDirsOnly  |QFileDialog::DontResolveSymlinks);
	if(!dir.isEmpty()){
		ui.dir->setText(dir);
		CheckInputValid();
	}
}
void NewProjectDlg::BindSignal(){
	bool res = false;
	res = connect(ui.ok,SIGNAL(clicked()),SLOT(accept())); assert(res);
	res = connect(ui.cancel,SIGNAL(clicked()),SLOT(reject())); assert(res);
	res = connect(ui.selectDir,SIGNAL(clicked()),SLOT(SelectDir())); assert(res);
	res = connect(ui.name,SIGNAL(textChanged(const QString&)),
		     SLOT(NameChanged(const QString&))); assert(res);
}
QString NewProjectDlg::Dir() const{
	return ui.dir->text();
}
void NewProjectDlg::Init(){
	ui.dir->setText(QDir::currentPath());
	ui.name->setText("NewProject");
	ui.hint->setText("");
	CheckInputValid();
}
QString NewProjectDlg::Name() const{
	return ui.name->text();
}
void NewProjectDlg::NameChanged(const QString&){
	CheckInputValid();
}
void NewProjectDlg::CheckInputValid(){
	QString name = Name();
	if(name.isEmpty()){
		ui.hint->setText(QStringLiteral("名称不允许为空"));
		ui.ok->setEnabled(false);
		return;
	}
	if(QDir(ui.dir->text()).exists(name)){
		ui.hint->setText(QStringLiteral("已经存在该文件夹"));
		ui.ok->setEnabled(false);
		return;
	}
	ui.ok->setEnabled(true);
	ui.hint->setText("");
}

