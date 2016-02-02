#include "PreHeader.h"
#include "ItemDelegate.h"
#include "ItemInfo.h"
#include "ImageSwitcher.h"
#include "ItemFactory.h"
#include "BoolImage.h"
#include <assert.h>

using namespace ItemInfo;

ItemDelegate::ItemDelegate(QObject * parent)
: QStyledItemDelegate(parent)
{

}
ItemDelegate::~ItemDelegate(){

}
QWidget* ItemDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem& option,
									const QModelIndex& idx)const{
	QVariant var = idx.model()->data(idx);
	if( var.canConvert<Visible>() || var.canConvert<UnLock>() ) {
		ImageSwitcher* pSwitcher = sItemFactory.CreateImageSwitcher(parent,var.userType());
		bool res = false;
		res = connect(pSwitcher,&ImageSwitcher::ValueChanged,this,&ItemDelegate::CommitData);  assert(res);
		res = connect(pSwitcher,&ImageSwitcher::EditFinished,this,&ItemDelegate::CloseEditor); assert(res);
		return pSwitcher;
	}else{
		return QStyledItemDelegate::createEditor(parent,option,idx);
	}
}
void ItemDelegate::paint(QPainter* painter,
                         const QStyleOptionViewItem& option,
						 const QModelIndex& idx)const{
	QVariant var = idx.model()->data(idx);
	if(var.canConvert<Visible>()){
		DrawBackgroud(painter,option);
		sItemFactory.GetBoolImage(var.userType())->Paint(painter,option.rect,var.value<Visible>());
	}else if(var.canConvert<UnLock>()){
		DrawBackgroud(painter,option);
		sItemFactory.GetBoolImage(var.userType())->Paint(painter,option.rect,var.value<UnLock>());
	}else{
		QStyledItemDelegate::paint(painter,option,idx);
	}
}
void ItemDelegate::setEditorData(QWidget* editor,const QModelIndex& idx)const{
	QVariant var = idx.model()->data(idx);
	if(var.canConvert<Visible>()){
		ImageSwitcher* pWidget = qobject_cast<ImageSwitcher*>(editor);
		pWidget->SetValue(var.value<Visible>());
	}else if(var.canConvert<UnLock>()){
		ImageSwitcher* pWidget = qobject_cast<ImageSwitcher*>(editor);
		pWidget->SetValue(var.value<UnLock>());
	}else{
		QStyledItemDelegate::setEditorData(editor,idx);
	}
}
void ItemDelegate::setModelData(QWidget* editor,
                                QAbstractItemModel* model,
								const QModelIndex& idx)const{
	ImageSwitcher* pWidget = qobject_cast<ImageSwitcher*>(editor);
	if(pWidget){
		model->setData(idx,pWidget->Value());
	}else{
		QStyledItemDelegate::setModelData(editor,model,idx);
	}
}
void ItemDelegate::DrawBackgroud(QPainter* painter,const QStyleOptionViewItem& option) const{
	painter->save();
	painter->setPen(Qt::NoPen);
	if(option.state & QStyle::State_HasFocus){
		painter->setBrush(option.palette.highlight());
		painter->drawRect(option.rect);
	}else if(option.state & QStyle::State_Selected){
		painter->setBrush(option.palette.midlight());
		painter->drawRect(option.rect);
	}else{
	}
	painter->restore();
}
void ItemDelegate::CommitData(){
	QWidget *editor = qobject_cast<QWidget *>(sender());
	if(editor){
		emit commitData(editor);
	}
}
void ItemDelegate::CloseEditor(){
	QWidget *editor = qobject_cast<QWidget *>(sender());
	if(editor){
		emit closeEditor(editor);
	}
}