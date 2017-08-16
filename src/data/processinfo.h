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
    /**
     * Proceses pid
     */
	pid_t pid;
    
    /**
     * Parent process ID
     */
	pid_t ppid;
    
    /**
     * Process session ID
     */
	pid_t sessionPId;
    
    /**
     * Owner user id of process
     */
	uint  ownerId;
    
    /**
     * Virtual size used by process
     */
	ulong vsize;
    
    /**
     * Rsident set size. Memory in RAM and not swap or on fs.
     */
	ulong rss;
    
    /**
     * Process group id
     */
	int   processGroupId; 
    
    /**
     * Start time of process sinze last boot
     */
	unsigned long long startTime;
    
    /**
     * User time used by process since start
     */
	ulong   utime;
    
    /**
     * System time used by process since start.
     */
	ulong   stime;
    
    /**
     * Difference in user time since last start
     */
	long    diffUTime=-1;
    
    /**
     * Differen in system time used by process
     */
	long    diffSTime=-1;
    
    /**
     * Path to executable
     */
	QString exe;
    
    /**
     * process name
     */
	QString comm;
    
    /**
     * Current working directory of process.
     */
	QString cwd;
    
    /**
     *  COmmand line used for starting process
     */
    QString cmdLine;
    
    /**
     * State of process
     */
	char    state=' ';
	
	/**
     * Get nice level.
     */
    int niceLevel=0;
	
    /**
     * If hasNiceLevel=true, than nice level has a valid niceLevel.
     * Reading of niceLevel can fail if the user hasn't proper rights.
     */
    
    bool hasNiceLevel=false;
    
	/**
     * Parent process object.
     */
	TProcessInfo *parent;
    
    /**
     * All sub processes of this process.
     */
	QHash<uint,TProcessInfo*> subprocess;
    
    /**
     *  Threads in this process
     */
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
	inline int   getNiceLevel(){ return niceLevel;}
	inline bool  getHasNiceLevel(){ return hasNiceLevel;}
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
    void readInfo();

};

#endif
