#include "processinfo.h"
#include "src/base/os.h"
#include <unistd.h>

/**
 * Stores process data in a QVector (for flexible displaying information)
 * \param p_info List to fill with data (entry 0:pid 1:exec name)
 */
void TProcessInfo::getInfo(QVector<QString>& p_info)
{
	p_info.clear();
	p_info << QString::number(pid);
	p_info << QString::number(ppid);
	p_info << exe;
	p_info << comm;
	p_info << stateString();
	p_info << QString::number(ownerId);
	p_info << getOwnerName();
	p_info << cwd;
	p_info << cmdLine;
	p_info << timeToString(startTime);
	p_info << QString::number(processGroupId);
	
}

QString TProcessInfo::stateString()
{
	switch(getState()){
		case 'R': return "Running";
		case 'S': return "Sleeping";
		case 'D': return "Waiting in uninterruptible disk sleep";
		case 'Z': return "Zombie";
		case 'T': return "Stopped(on a signal)";
		case 't': return "Tracing stop";
		case 'W': return "Paging/Waking";
		case 'X': return "Dead";
		case 'x': return "Dead";
		case 'K': return "Wake kill";
		case 'P': return "Parked";
		default:
			return QString("Unknown ")+getState();
	}
}

QString TProcessInfo::getOwnerName()
{
	return getUsernameById(ownerId);
}

void TProcessInfo::addSubProcess(TProcessInfo* p_processInfo)
{
	subprocess.insert(p_processInfo->getPid(),p_processInfo);
}

void TProcessInfo::addThread(TProcessInfo* p_processInfo)
{
	threads.append(p_processInfo);
}

QString TProcessInfo::timeToString(unsigned long long p_time)
{
	QString l_return ="";
	unsigned long long l_time=p_time;
	long l_clkTck=sysconf(_SC_CLK_TCK);
	l_return=QString::number(l_time % l_clkTck);
	l_time=l_time/l_clkTck;
	l_return=QString::number(l_time % 60)+"."+l_return;
	l_time=l_time/60;
	l_return=QString::number(l_time % 60)+":"+l_return;
	l_time=l_time/60;
	l_return=QString::number(l_time % 24)+":"+l_return;
	l_time=l_time/24;
	if(l_time>0){
		l_return=QString::number(l_time)+"d "+l_return;
	}
	return l_return;
	
}
