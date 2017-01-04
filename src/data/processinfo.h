#ifndef __PROCESSINFO_H_
#define __PROCESSINFO_H_
#include <sys/types.h>
#include <QString>
#include <QVector>
#include <QHash>
#include "src/base/linklist.h"

class TProcessInfo
{
private:
	pid_t pid;
	pid_t ppid;
	pid_t sessionPId;
	uint  ownerId;
	int   processGroupId; 
	unsigned long long startTime;
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
	inline void setStartTime(unsigned long long p_startTime){ startTime=p_startTime;}
	inline int getProcessGroupId(){ return processGroupId;}
	inline void setProcessGroupId(int p_processGroupId){ processGroupId=p_processGroupId;}
	inline pid_t getSessionPId(){ return sessionPId;}
	inline void setSessionPId(pid_t p_sessionPId){ sessionPId=p_sessionPId;}
	void addSubProcess(TProcessInfo *p_processInfo);
	void addThread(TProcessInfo *p_processInfo);
	QString getOwnerName();
	void getInfo(QVector<QString> & p_info);
	inline QHash<uint,TProcessInfo*> *getSubProcesses(){ return &subprocess;}
	inline TLinkList<TProcessInfo> *getThreads(){ return &threads;}
	QString stateString();
	QString timeToString(unsigned long long p_time);

};

#endif
