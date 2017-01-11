#ifndef _CGROUPINFO_H_
#define _CGROUPINFO_H_
#include <QString>

/**
 *  class containing information about cgroups
 *  This data is read from proc/#id/cgroups
 *  This file is only available when cgroups are compiled into the kernel (CONFIG_CGROUPS)
 */

class TCGroupInfo
{
private:
	int hierarchyId ;
	QString subsystems;
	QString cgroup;
public:
	TCGroupInfo(int p_hierarchyId,const QString &p_subsystem,const QString &p_cgroup){
		hierarchyId=p_hierarchyId;
		subsystems=p_subsystem;
		cgroup=p_cgroup;
	}
	
	inline int getHierarchyId(){ return hierarchyId;}
	inline const QString &getSubsystems(){ return subsystems;}
	inline const QString &getCGroup(){ return cgroup;}
};

#endif
