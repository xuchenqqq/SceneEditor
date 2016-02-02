#include "PreHeader.h"
#include "ShortcutDlg.h"
#include "AppModAlias.h"
#include "ActionMng.h"
#include "KeySetDlg.h"
#include "Macro.h"

enum{
	PART_NODE = QTreeWidgetItem::UserType+1,
	ACTION_NODE
};

Q_DECLARE_METATYPE(QAction*)

ShortcutDlg::ShortcutDlg(QWidget* parent)
: QDialog(parent)
{
	setWindowFlags((Qt::WindowFlags)(~Qt::WindowCloseButtonHint & windowFlags()));
	ui.setupUi(this);
	Init();
	BindSignal();
}
ShortcutDlg::~ShortcutDlg(){

}
void ShortcutDlg::SelectItem(QTreeWidgetItem* item,int){
	FAIL_RET_VOID(item->type() == ACTION_NODE);
	QAction* pAct = item->data(0,Qt::UserRole).value<QAction*>();
	if(KeySetDlg(this,pAct).exec() == QDialog::Accepted){
		item->setText(1,pAct->shortcut().toString());
	}
}
void ShortcutDlg::Exit(){
	sActionMng.SaveShortcut();
	accept();
}
void ShortcutDlg::Init(){
	ui.tree->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tree->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tree->setAlternatingRowColors(true);
	SetupAction();
}
void ShortcutDlg::BindSignal(){
	bool res = false;
	res = connect(ui.tree,&QTreeWidget::itemDoubleClicked,
		this,&ShortcutDlg::SelectItem); assert(res);
	res = connect(ui.close,SIGNAL(clicked()),SLOT(Exit())); assert(res);
}
void ShortcutDlg::SetupAction(){
	ActionMng& actMng = sActionMng;
	foreach(const QString& part,actMng.Parts()){
		QTreeWidgetItem* pPartItem = 
			new QTreeWidgetItem(ui.tree,QStringList()<<part,PART_NODE); 
		foreach(QAction* pAct,actMng.Actions(part)){
			QTreeWidgetItem* pActItem = 
				new QTreeWidgetItem(pPartItem,
				QStringList()<<pAct->text()
				<<pAct->shortcut().toString(),
				ACTION_NODE);
			pActItem->setData(0,Qt::UserRole,QVariant::fromValue(pAct));
		}
	}
}
