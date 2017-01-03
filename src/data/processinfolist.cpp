#include "processinfolist.h"
#include "processinfo.h"
#include <QDirIterator>
#include "src/base/utils.h"
#include <iostream>
#include "src/base/utils.h"


void TProcessInfoList::readInfo()
{
	QDirIterator l_iter("/proc");
	pid_t l_id;
	bool l_ok;
	TProcessInfo *l_info;
	while(l_iter.hasNext()){
		l_id=l_iter.fileName().toLong(&l_ok);
		if(l_ok){
			l_info=new TProcessInfo();
			append(l_info);	
			l_info->setPid(l_id);
			pidIndex.insert(l_id,l_info);
			l_info->setOwnerId(l_iter.fileInfo().ownerId());
			readProcess(l_iter.filePath(),l_info);
			readThreads(l_iter.filePath(),l_info);

		}		
		l_iter.next();
	}
	toHyr();
	
}

/** Read process information from /proc file system for one process
 * 
 * \param p_path - path to folder with information about process (/proc/#pid#)
 * \param p_info - process info object which will be filled with information
 */

void TProcessInfoList::readProcess(QString p_path, TProcessInfo *p_info)
{
	
	QString l_exe=QFile::symLinkTarget(p_path+"/exe");
	p_info->setExec(l_exe);
	QString l_str;
	readString(p_path,"comm",l_str);
	p_info->setComm(l_str);	
	l_str=QFile::symLinkTarget(p_path+"/cwd");
	p_info->setCWD(l_str);
	processStat(p_path,p_info);
	readString(p_path,"cmdline",l_str);
	p_info->setCmdLine(l_str);
}


/** Reread proces info.
 * 
 *  \param p_info - proces to reread.
 */

void TProcessInfoList::reReadProcess(TProcessInfo* p_info)
{
	readProcess(QString("/proc/")+QString::number(p_info->getPid()),p_info);
}


/**
 * Process data in /proc/xxx/stat file which contains some information about a process
 * 
 * \param p_path - path to process folder /proc/pid
 * \param p_info - process info in which the information is set.
 */
void TProcessInfoList::processStat(QString p_path, TProcessInfo* p_info)
{
	QString l_stat;
	
	readString(p_path,"stat",l_stat);	
	if(l_stat.length()==0) return;
	int l_end=l_stat.indexOf(")");
	if(l_end<0) return;
	while(l_end < l_stat.size() && l_stat[l_end]!=' ') l_end++;
	QStringList l_list=l_stat.mid(l_end+1).split(' ');
	if(l_list.length()>2){
		p_info->setState(l_list[0].at(0).toLatin1());
		p_info->setPPid(l_list[1].toLong());
	}
}

/** Read information of threats inside proces
 * 
 * \param p_path path where information about the parent is stored /proc/#pid#
 * \param p_parent parent proces of threats 
 */

void TProcessInfoList::readThreads(QString p_path, TProcessInfo* p_parent)
{
	QDirIterator l_iter(p_path+"/task");
	pid_t l_id;
	bool l_ok;
	TProcessInfo *l_info;	
	while(l_iter.hasNext()){
		l_iter.next();
		l_id=l_iter.fileName().toLong(&l_ok);
		if(l_ok){
			l_info=new TProcessInfo();			
			l_info->setPid(l_id);
			readProcess(l_iter.filePath(),l_info);
			p_parent->addThread(l_info);
		}
		
	}
	
}



/** Add process to parent process
 * 
 */
void TProcessInfoList::toHyr()
{
	TLinkListIterator<TProcessInfo> l_iter(this);
	TProcessInfo *l_pi;
	TProcessInfo *l_parent;
	while(l_iter.hasNext()){
		l_pi=l_iter.next();
		l_parent=pidIndex.value(l_pi->getPPid(),nullptr);
		l_pi->setParent(l_parent);
		if(l_parent != nullptr) l_parent->addSubProcess(l_pi);	
	}
}
