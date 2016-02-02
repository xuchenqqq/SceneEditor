#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "AuthOp.h"

class AuthDlg;

class Auth : public QObject
{
Q_OBJECT
public:
	Auth(QObject* parent);
	~Auth();
	void Check(bool isSilent=true);
	void Request(const QString& user,const QString& sn);
	void Notify(AuthOp::Type op,const QString& desc=QString());
	void Quit();
signals:
	void SigQuit();
private slots:
	void ReplyFinished();
private:
	void Init();
	void Clear();
	void BindSignal();
	void ClearReply();
	void HandleError();
	void HandleReply(int code,const QString& desc);
	void ReadSetting(QString& user,QString& sn) const;
	void Start();
	void Stop();
	void timerEvent(QTimerEvent*);
	void PopupDialog(bool isWaitReply,const QString& desc=QString());
private:
	QNetworkAccessManager mNetMng;
	QNetworkReply* mpReply;
	AuthDlg* mpDlg;
	int mTimer;
};
