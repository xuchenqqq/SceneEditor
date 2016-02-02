#ifndef SCREEN_MNG_H
#define SCREEN_MNG_H

#include <QObject>
#include <QList>
#include <QSize>
#include "ModDeclare.h"

class QFile;
class GSScreen;

class ScreenMng : public QObject
{
Q_OBJECT
public:
	ScreenMng(QObject* parent);
	~ScreenMng();
    void New(const QSize size,unsigned int cnt, int dir);
	void Save(QFile& file) const;
	bool Load(QFile& file);
	void Clear();
signals:
	void Updated(const QList<GSScreen*>& list);
public slots:
	void Preview(bool state);
private:
	QList<GSScreen*> mList;
	QSize mSize;
    int mDir;
	friend class JsonReader;
	friend class JsonWriter;
    friend class XMLWriter;
    friend class XMLReader;
};
M_DECL_MOD(ScreenMng)

#endif
