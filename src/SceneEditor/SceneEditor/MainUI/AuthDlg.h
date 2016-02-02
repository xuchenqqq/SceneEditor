#pragma once

#include "ui_AuthDlg.h"
#include "AuthOp.h"

class Auth;

class AuthDlg : public QDialog
{
Q_OBJECT
public:
	enum STATE{
		WAIT_INPUT,
		WAIT_NET_REPLY,
	};
public:
	AuthDlg(Auth* pAuth);
	~AuthDlg();
	void Init(bool isWaitReply,const QString& desc);
	void Notify(AuthOp::Type op,const QString& desc=QString());
private slots:	
	void Check();
	void Quit();
protected:
	void timerEvent(QTimerEvent*);
private:
	void StartWaiting();
	void StopWaiting();
	void PermitInput();
	void ForbidInput();
	void WaitInput();
	void WaitReply();
	bool IsInputValid() const;
	void ReadSetting();
	void WriteSetting();
	void BindSignal();
private:
	Ui::AuthDlg ui;
	int mTimerId;
	bool mIsWaitReply;
	Auth* mpAuth; 
};