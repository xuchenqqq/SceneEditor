#include "PreHeader.h"
#include "ItemFactory.h"
#include "ItemInfo.h"
#include "Macro.h"
#include <assert.h>
#include "BoolImage.h"
#include "ImageSwitcher.h"

using namespace ItemInfo;

ItemFactory& ItemFactory::Instance(){
	static ItemFactory inst;
	return inst;
}
BoolImage* ItemFactory::GetBoolImage(int type){
	if(!mMap.contains(type)){
		BoolImage* pBoolImage = NULL;
		if(type == qMetaTypeId<Visible>()){
			pBoolImage = new BoolImage;
			pBoolImage->SetImage(true,QPixmap(":/Icon/visible.png"));
			pBoolImage->SetImage(false,QPixmap(":/Icon/invisible.png"));
			mMap.insert(type,pBoolImage);
		}else if(type == qMetaTypeId<UnLock>()){
			pBoolImage = new BoolImage;
			pBoolImage->SetImage(true,QPixmap(":/Icon/unlock.png"));
			pBoolImage->SetImage(false,QPixmap(":/Icon/lock.png"));
			mMap.insert(type,pBoolImage);
        }else{
			ASSERT_MSG_RET_X(false,"",NULL);
		}
	}
	return mMap[type];
}
ImageSwitcher* ItemFactory::CreateImageSwitcher(QWidget* parent,int type){
	return new ImageSwitcher(parent,GetBoolImage(type));
}
ItemFactory::ItemFactory()
{
}
ItemFactory::~ItemFactory(){
}
