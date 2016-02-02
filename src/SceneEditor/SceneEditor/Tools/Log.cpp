#include "PreHeader.h"
#include "Log.h"
#include "MsgDock.h"
#include "AppModAlias.h"
#include <string.h>
#include <QString>
#include <stdio.h>

namespace Log
{
	void InfoLn(const char* msg,...){
		char buf[512];
		va_list ap;
		va_start(ap, msg);
			vsnprintf(buf,512,msg,ap);
		va_end(ap);
		sMsgDock.AddMessageLn(QString("Info:  ")+QString::fromUtf8(buf,strnlen(buf,512)));
	}
	void Info(const QString& msg){
		sMsgDock.AddMessage(msg);
	}
}