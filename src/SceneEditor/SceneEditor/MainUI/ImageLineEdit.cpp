#include "PreHeader.h"
#include "ImageLineEdit.h"
#include "Asset.h"

ImageLineEdit::ImageLineEdit(QWidget* parent)
: QLineEdit(parent)
{
	setAcceptDrops(true);
	setReadOnly(true);
}
ImageLineEdit::~ImageLineEdit(){
}
void ImageLineEdit::dragEnterEvent(QDragEnterEvent* event){
	if(event->mimeData()->hasFormat(Asset::IMAGE)){
		event->setAccepted(true);
	}else{
		event->ignore();
	}
}
void ImageLineEdit::dropEvent(QDropEvent* event){
	const QMimeData* pMime = event->mimeData();
	if(pMime->hasFormat(Asset::IMAGE)){
		QString image = pMime->data(Asset::IMAGE);
		if(image != text()){
			setText(image);
			emit ImageDroped();
		}
	}
}