#ifndef _UTILS_H_
#define _UTILS_H_
#include <QString>

#define qstr(p_string) ((p_string).toUtf8().data())

bool readLong(QString p_path,QString p_name,unsigned long &p_value);
bool readString(QString p_path,QString p_name,QString &p_value);

#endif
