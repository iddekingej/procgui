#ifndef __OS_H_
#define __OS_H_

#include <QString>
#include <QMap>

QString getUsernameById(uint p_uid);
void getAllUsers(QMap<uint,QString> &p_userList);
#endif
