#pragma once

#include "ModDeclare.h"
#include <QObject>
#include <QMap>
#include <QString>

class QWidget;

class UIAccess
{
public:
	template<typename T>
	T* TypePtr(const QString& name) const;
	QWidget* Ptr(const QString& name) const;
	void Set(const QString& name,QWidget* pWidget);
private:
	QMap<QString,QWidget*> mMap;
};
M_DECL_MOD(UIAccess)


template<typename T>
T* UIAccess::TypePtr(const QString& name) const{
	QWidget* pWidget = Ptr(name);
	if(pWidget){
		return qobject_cast<T*>(pWidget);
	}else{
		return NULL;
	}
}

