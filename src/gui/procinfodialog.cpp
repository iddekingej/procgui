#include "procinfodialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHashIterator>
#include <QMapIterator>
#include "src/base/linklist.h"
#include "src/base/utils.h"
#include <klocalizedstring.h>


TProcInfoDialog::TProcInfoDialog(TProcessInfo *p_procInfo)
{
	ui.setupUi(this);
	info=p_procInfo;
	refreshInfo();
	refresh.setInterval(1024);
	refresh.start();
	connect(&refresh,SIGNAL(timeout()),this,SLOT(refreshInfo()));
	connect(ui.close_button,SIGNAL(pressed()),this,SLOT(close()));
}

/***
 *  Periodically the process data is read and the data is updated in the dialog
 *  If process doesn't exists anymore a warning is displayed.
 */

void TProcInfoDialog::refreshInfo()
{
	TProcessInfoList *l_list=new TProcessInfoList();
	l_list->readInfo();
	if(processList != nullptr) l_list->diff(processList);
	TProcessInfo *l_pi=l_list->getByPid(info->getPid());
	
	if(l_pi != nullptr){
		ui.deadWarning->setVisible(false);
		fillData(l_pi);
	} else {
		ui.deadWarning->setVisible(true);
	}
	if(processList != nullptr) delete processList;
	processList=l_list;
}


/**
 *  Fills dialog with data from process (info object var)
 * 
 *  \param p_processInfo Fill dialog with information about this process
 */

void TProcInfoDialog::fillData(TProcessInfo *p_processInfo)
{
	ui.pid_label->setText(QString::number(p_processInfo->getPid()));
	ui.ppid_label->setText(QString::number(p_processInfo->getPPid()));
	ui.command_label->setText(p_processInfo->getComm());
	if(p_processInfo->getParent()!=nullptr){
		ui.parentCommand_label->setText(p_processInfo->getParent()->getExe());
	} else {
		ui.parentCommand_label->setText("");
	}
	ui.owner_label->setText(p_processInfo->getOwnerName());
	ui.state_label->setText(p_processInfo->stateString());	
	ui.cwd_label->setText(p_processInfo->getCWD());
	ui.startTime_label->setText(p_processInfo->timeToString(p_processInfo->getStartTime()));
	ui.processGroupId_label->setText(QString::number(p_processInfo->getProcessGroupId()));
	ui.sessionPId_label->setText(QString::number(p_processInfo->getSessionPId()));
	ui.vsize_label->setText(QString::number(p_processInfo->getVSize()));
	ui.rss_label->setText(QString::number(p_processInfo->getRSS()));
	ui.utime_label->setText(p_processInfo->getUTimeStr());
	ui.stime_label->setText(p_processInfo->getSTimeStr());
	ui.diffUTime_label->setText(QString::number(p_processInfo->getDiffUTime()));
	ui.diffSTime_label->setText(QString::number(p_processInfo->getDiffSTime()));
	QStandardItemModel *l_model=new QStandardItemModel(0,3,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Pid")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Command")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Command line")));
	QHashIterator<uint,TProcessInfo *> l_iter(*(p_processInfo->getSubProcesses()));
	int l_rowCnt=0;
	TProcessInfo *l_pi;
	while(l_iter.hasNext()){
		l_iter.next();
		l_pi=l_iter.value();
		l_model->setItem(l_rowCnt,0,new QStandardItem(QString::number(l_pi->getPid())));
		l_model->setItem(l_rowCnt,1,new QStandardItem(l_pi->getComm()));
		l_model->setItem(l_rowCnt,2,new QStandardItem(l_pi->getCmdLine()));
		l_rowCnt++;
	}
	setViewModel(ui.subProcessList,l_model);	
	fillThreats(p_processInfo);
	fillControlGroups(p_processInfo);
	QStandardItemModel *l_fileModel=new QStandardItemModel(0,2,this);
	l_fileModel->setHorizontalHeaderItem(0,new QStandardItem(i18n("Fd")));
	l_fileModel->setHorizontalHeaderItem(1,new QStandardItem(i18n("File")));
	QHash<int,QString> l_openFiles;
	p_processInfo->getOpenFiles(l_openFiles);
	QHashIterator<int,QString> l_oi(l_openFiles);
	l_rowCnt=0;
	while(l_oi.hasNext()){
		l_oi.next();
		l_fileModel->setItem(l_rowCnt,0,new QStandardItem(QString::number(l_oi.key())));
		l_fileModel->setItem(l_rowCnt,1,new QStandardItem(l_oi.value()));
		l_rowCnt++;
	}
	setViewModel(ui.openFiles,l_fileModel);	
	ui.openFiles->resizeColumnsToContents();
	ui.openFiles->resizeRowsToContents();
}

/**
 *  Fill threads grid with information about threads in process (read from proc/pid/task)
 * 
 */

void TProcInfoDialog::fillThreats(TProcessInfo *p_processInfo)
{
	QStandardItemModel *l_model=new QStandardItemModel(0,2,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Pid")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Command")));
	TLinkListIterator<TProcessInfo> l_iter(p_processInfo->getThreads());
	int l_rowCnt=0;
	TProcessInfo *l_pi;
	while(l_iter.hasNext()){
		l_pi=l_iter.next();		
		l_model->setItem(l_rowCnt,0,new QStandardItem(QString::number(l_pi->getPid())));
		l_model->setItem(l_rowCnt,1,new QStandardItem(l_pi->getComm()));
		l_rowCnt++;
	}
	
	setViewModel(ui.threads,l_model);	
	ui.threads->resizeRowsToContents();
	ui.threads->resizeColumnsToContents();
}

/**
 *  List of control group used in process
 *  This information is read from /proc/#pid/cgroups 
 *  This is only available when cgroups are compiled into the kernel
 */

void TProcInfoDialog::fillControlGroups(TProcessInfo *p_processInfo)
{
	QStandardItemModel *l_model=new QStandardItemModel(0,3,this);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("Hierarchy Id")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Controlled subsystem")));
	l_model->setHorizontalHeaderItem(2,new QStandardItem(i18n("Control group")));
	TLinkList<TCGroupInfo> l_cgroups;
	p_processInfo->getCGroups(l_cgroups);
	TLinkListIterator<TCGroupInfo> l_iter(l_cgroups);
	int l_rowCnt=0;
	TCGroupInfo *l_cgi;
	while(l_iter.hasNext()){
		l_cgi=l_iter.next();		
		l_model->setItem(l_rowCnt,0,new QStandardItem(QString::number(l_cgi->getHierarchyId())));
		l_model->setItem(l_rowCnt,1,new QStandardItem(l_cgi->getSubsystems()));
		l_model->setItem(l_rowCnt,2,new QStandardItem(l_cgi->getCGroup()));
		l_rowCnt++;
	}
	setViewModel(ui.controlGroups,l_model);	
	ui.controlGroups->resizeRowsToContents();
	ui.controlGroups->resizeColumnsToContents();
}


TProcInfoDialog::~TProcInfoDialog()
{
	if(processList != nullptr) delete processList;
}
