#include "os.h"
#include <sys/types.h>
#include <pwd.h>
#include <QMap>
#include <QString>
/** Get login name of user from the user id
 * 
 * \param p_uid  Userid 
 */
QString getUsernameById(uint p_uid)
{
	struct passwd *l_passwd=getpwuid(p_uid);
	if(l_passwd==nullptr){
		return "<unkown id="+QString::number(p_uid)+">";
	} else {
		return l_passwd->pw_name;
	}
}


/**  Get list of users defined on the system
 * 
 * \param p_userList  id/username map of users defined in the system
 */

void getAllUsers(QMap<uint,QString> &p_userList)
{
	passwd *l_pwd;
	setpwent();
	while((l_pwd=getpwent())!= nullptr){
		p_userList.insert(l_pwd->pw_uid,l_pwd->pw_name);
	}
	endpwent();
}
