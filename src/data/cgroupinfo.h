#ifndef _CGROUPINFO_H_
#define _CGROUPINFO_H_
#include <QString>

/**
 *  class containing information about cgroups
 *  This data is read from proc/#id/cgroups
 *  This information is only available when cgroups are compiled into the kernel (CONFIG_CGROUPS)
 */

class TCGroupInfo
{
private:
	int hierarchyId ;
	QString subsystems;
	QString cgroup;
public:
    
    /**
     * Setup CGroup information object
     * 
     * \param p_hierarchyId  CGroup HyarchyID
     * \param p_subsystem    Subsystem name
     * \param p_cgroup       CGroup name
     */
	
    TCGroupInfo(int p_hierarchyId,const QString &p_subsystem,const QString &p_cgroup){
		hierarchyId=p_hierarchyId;
		subsystems=p_subsystem;
		cgroup=p_cgroup;
	}
	
	/**
     * Get Hyrarchy ID
     */
	
    inline int getHierarchyId(){ return hierarchyId;}
	
	/**
     * Get subsystem ID
     */
	
    inline const QString &getSubsystems(){ return subsystems;}
	
	/**
     * Get CGroup name towhich this item belongs
     */
    
	inline const QString &getCGroup(){ return cgroup;}
};

#endif
