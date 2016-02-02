#include "PreHeader.h"
#include "Auth.h"
#include "AuthCode.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "AppModAlias.h"
#include "Setting.h"
#include "AuthDlg.h"

const char* const SETTING_USER = "Auth/user";
const char* const SETTING_SN   = "Auth/sn";

const int SECOND = 1000;
const int MINUTE = 60*SECOND;
const int CHECK_INTERVAL_MIN = 10*MINUTE;

Auth::Auth(QObject* parent)
: QObject(parent)
, mNetMng(this)
, mpReply(NULL)
, mTimer(0)
{
	Init();
	BindSignal();
}
Auth::~Auth(){
	Clear();
}
void Auth::Check(bool isSilent){
    return;
	QString user;
	QString sn;
	ReadSetting(user,sn);
	assert(mpDlg->isVisible() == false);
	if(user.isEmpty() || sn.isEmpty()){
		PopupDialog(false,QStringLiteral("请输入用户名和注册码"));
	}else{
		Request(user,sn);
		if(!isSilent){
			PopupDialog(true);
		}
	}
}
void Auth::Notify(AuthOp::Type op,const QString& desc){
	if(mpDlg->isVisible()){
		mpDlg->Notify(op,desc);
	}else{
		switch(op){
			case AuthOp::ACCEPT: break; //啥事没有
			case AuthOp::INPUT: PopupDialog(false,desc);break;
			default: break; //不处理
		}
	}
}
void Auth::Quit(){
	emit SigQuit();
}
void Auth::ReplyFinished(){
	if(mpReply->error() == QNetworkReply::NoError){
		QByteArray data = mpReply->readAll();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject obj = doc.object();
		HandleReply(obj.value("code").toInt(-1),obj.value("desc").toString());
	}else{
		HandleError();
	}
	ClearReply();
}
void Auth::Request(const QString& user,const QString& sn){
	ClearReply();
	QString req = QString("http://www.ccplaying.com:8099/tools/verify?user=%1&sn=%2")
	              .arg(user).arg(sn);
	mpReply =  mNetMng.get(QNetworkRequest(req));
	bool res = false;
	res = connect(mpReply,&QNetworkReply::finished,
				  this,&Auth::ReplyFinished); assert(res);
}
void Auth::Init(){
	Start();
	mpDlg = new AuthDlg(this);
}
void Auth::Clear(){
	ClearReply();
	delete mpDlg;
}
void Auth::BindSignal(){
}
void Auth::ReadSetting(QString& user,QString& sn) const{
	user = sSetting.value(SETTING_USER).toString();
	sn   = sSetting.value(SETTING_SN).toString();
}
void Auth::ClearReply(){
	if(mpReply){
		mpReply->deleteLater();
		mpReply = NULL;
	}
}
void Auth::HandleError(){
	#define CASE(err,str) case QNetworkReply::err : desc=QStringLiteral(str);break;
	QString desc;
	switch(mpReply->error()){
		CASE(ConnectionRefusedError,"服务器拒绝建立连接");
		CASE(RemoteHostClosedError,"服务器响应前以关闭连接");
		CASE(HostNotFoundError,"无效服务器路径");
		CASE(TimeoutError,"服务器响应超时");
		default:
			desc = QStringLiteral("未知响应错误，错误码：")+QString::number(mpReply->error());
	}
	Notify(AuthOp::INPUT,desc);
}
void Auth::HandleReply(int code,const QString& desc){
	using namespace AuthCode;
	switch(code){
		case PASS:              Notify(AuthOp::ACCEPT,desc);break;
		case INPUT_WRONG:
		case IP_CHANGED:
		case USER_DISALBE:
		case DATA_FORMAT_WRONG: Notify(AuthOp::INPUT,desc); break;
		default:
			Notify(AuthOp::INPUT,QStringLiteral("验证服务器返回数据格式错误"));
			break;
	}
}
void Auth::Start(){
	if(!mTimer){
		mTimer = startTimer(CHECK_INTERVAL_MIN,Qt::VeryCoarseTimer); //10分钟
	}
}
void Auth::Stop(){
	if(mTimer){
		killTimer(mTimer);
		mTimer = 0;
	}
}
void Auth::timerEvent(QTimerEvent*){
	if(mpDlg->isVisible() == false){
		Check();
	}
}
void Auth::PopupDialog(bool isWaitReply,const QString& desc){
	mpDlg->Init(isWaitReply,desc);
	if(mpDlg->exec() == QDialog::Rejected){
		Quit();
	}
}



