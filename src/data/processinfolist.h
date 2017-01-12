#ifndef _PROCESSINFOLIST_H_
#define _PROCESSINFOLIST_H_

#include "src/base/linklist.h"
#include "processinfo.h"
#include <QHash>

class TProcessInfoList:public TLinkList<TProcessInfo>
{
private:
	QHash<int,TProcessInfo *> pidIndex;
	
	void readProcess(QString p_path,TProcessInfo *p_info);
	void reReadProcess(TProcessInfo *p_info);
	void processStat(QString p_path,TProcessInfo *p_info);
	void readThreads(QString p_path,TProcessInfo *p_parent);
	TProcessInfo *readInfoFromPid(pid_t p_pid);
	void toHyr();
	
public:	
	
	void readInfo();
	inline TProcessInfo *getByPid(uint p_pid){ return pidIndex.value(p_pid,nullptr);}
	void diff(TProcessInfoList *p_list);
	void uidWithProcess(QSet<int> &p_uids);
};

#endif
