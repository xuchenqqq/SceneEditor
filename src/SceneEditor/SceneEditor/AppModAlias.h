#ifndef APP_MOD_AlIAS_H
#define APP_MOD_AlIAS_H

#include "AppMod.h"

#define sMsgDock      AppMod::Get<MsgDock>()
#define sPath          AppMod::Get<Path>()
#define sCache       AppMod::Get<Cache>()
#define sScreenDock   AppMod::Get<ScreenDock>()
#define sActionMng    AppMod::Get<ActionMng>()
#define sScene 		  AppMod::Get<EditorScene>()
#define sScreenMng    AppMod::Get<ScreenMng>()
#define sSetting      AppMod::Get<QSettings>()
#define sAttrDock     AppMod::Get<AttrDock>()
#define sGSInfo       AppMod::Get<GSInfo>()
#define sView         AppMod::Get<GSView>()
#define sUiAccess     AppMod::Get<UIAccess>()
#define sTimer        AppMod::Get<TimerService>()

#endif
