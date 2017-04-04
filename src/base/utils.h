#ifndef _UTILS_H_
#define _UTILS_H_
#include <QString>
#include <QAbstractItemView>
#include <QStandardItemModel>
#define qstr(p_string) ((p_string).toUtf8().data())
#define PAR_UNUSED __attribute__((unused))

bool readLong(QString p_path,QString p_name,unsigned long &p_value);
bool readString(QString p_path,QString p_name,QString &p_value);
void setViewModel(QAbstractItemView *p_view,QStandardItemModel *p_model);

#endif
