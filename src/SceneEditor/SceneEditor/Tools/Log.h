#ifndef LOG_H
#define LOG_H

class QString;

namespace Log
{
	void InfoLn(const char* msg,...);
	void Info(const QString& msg);
};


#endif