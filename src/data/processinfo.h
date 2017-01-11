#ifndef _PROCESSINFO_H_
#define _PROCESSINFO_H_
#include <sys/types.h>
#include <QString>
#include <QVector>
#include <QHash>
#include "src/base/linklist.h"
#include "cgroupinfo.h"

/**
 * Class containing all the information about a process
 * This information is read from /proc/#pidnummer/xxx
 * See man proc
 */

class TProcessInfo
{
private:
	pid_t pid;
	pid_t ppid;
	pid_t sessionPId;
	uint  ownerId;
	ulong  vsize;
	ulong   rss;
	int   processGroupId; 
	unsigned long long startTime;
	ulong utime;
	ulong stime;
	long diffUTime=-1;
	long diffSTime=-1;
	QString exe;
	QString comm;
	QString cwd;
	QString cmdLine;
	char    state=' ';
	TProcessInfo *parent;
	QHash<uint,TProcessInfo*> subprocess;
	TLinkList<TProcessInfo> threads;
public:
	inline pid_t getPid(){ return pid;}
	inline void setPid(pid_t p_pid){pid=p_pid;}
	inline pid_t getPPid(){ return ppid;}
	inline void setPPid(pid_t p_ppid){ppid=p_ppid;}
	inline QString &getExe(){ return exe;}
	inline void setExec(QString &p_exe){exe=p_exe;}
	inline QString &getComm(){ return comm;}
	inline void setComm(QString &p_comm){ comm=p_comm;}
	inline char getState(){ return state;}
	inline void setState(char p_state){ state=p_state;}
	inline void setOwnerId(uint p_ownerId){ ownerId=p_ownerId;}
	inline uint getOwnerId(){ return ownerId;}
	inline void setParent(TProcessInfo *p_parent){ parent=p_parent;}
	inline void setCWD(QString &p_cwd){ cwd=p_cwd;}
	inline QString &getCWD(){ return cwd;}
	inline void setCmdLine(QString &p_cmdLine){cmdLine=p_cmdLine; }
	inline QString &getCmdLine(){return cmdLine;}
	inline TProcessInfo *getParent(){ return parent;}
	inline unsigned long long getStartTime(){ return startTime;}
	inline void  setStartTime(unsigned long long p_startTime){ startTime=p_startTime;}
	inline ulong getUTime(){ return utime;}
	inline void  setUTime(ulong p_utime){ utime=p_utime;}
	inline ulong getSTime(){ return stime;}
	inline void  setSTime(ulong p_stime){ stime=p_stime;}
	inline const QString getSTimeStr(){ return timeToString(getSTime());}
	inline const QString getUTimeStr(){ return timeToString(getUTime());}
	inline const QString getTotalTimeStr(){ return timeToString(getUTime()+getSTime());}
	inline long getDiffUTime(){ return diffUTime;}
	inline void  setDiffUTime(long p_diffUTime){ diffUTime=p_diffUTime;}
	inline long getDiffSTime(){ return diffSTime;}
	inline void  setDiffSTime(long p_diffSTime){ diffSTime=p_diffSTime;}
	inline long getDiffTotalTime(){ return diffUTime+diffSTime;}
	inline int   getProcessGroupId(){ return processGroupId;}
	inline void  setProcessGroupId(int p_processGroupId){ processGroupId=p_processGroupId;}
	inline pid_t getSessionPId(){ return sessionPId;}
	inline void  setSessionPId(pid_t p_sessionPId){ sessionPId=p_sessionPId;}
	inline void  setVSize(ulong p_vsize){ vsize=p_vsize;}
	inline ulong getVSize(){return vsize;}
	inline void  setRSS(ulong p_rss){rss=p_rss;}
	inline ulong getRSS(){ return rss;}
	inline QHash<uint,TProcessInfo*> *getSubProcesses(){ return &subprocess;}
	inline TLinkList<TProcessInfo> *getThreads(){ return &threads;}	
	void addSubProcess(TProcessInfo *p_processInfo);
	void addThread(TProcessInfo *p_threadInfo);
	QString getOwnerName();
	void getInfo(QVector<QString> & p_info);

	QString stateString();
	const QString timeToString(unsigned long long p_time);
	void getOpenFiles(QHash<int,QString> &p_map);	
	void getCGroups(TLinkList<TCGroupInfo> &p_info);

};

#endif
