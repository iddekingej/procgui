#include "procinfodialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHashIterator>
#include <QMapIterator>
#include "src/base/linklist.h"

TProcInfoDialog::TProcInfoDialog(TProcessInfo *p_procInfo)
{
	ui.setupUi(this);
	info=p_procInfo;
	refreshInfo();
	refresh.setInterval(1024);
	refresh.start();
	connect(&refresh,SIGNAL(timeout()),this,SLOT(refreshInfo()));
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
	ui.command_label->setText(p_processInfo->getExe());
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
	QStandardItemModel *l_model=new QStandardItemModel(0,3);
	l_model->setHorizontalHeaderItem(0,new QStandardItem("Pid"));
	l_model->setHorizontalHeaderItem(1,new QStandardItem("Command"));
	l_model->setHorizontalHeaderItem(2,new QStandardItem("Command line"));
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
	ui.subProcessList->setModel(l_model);
	fillThreats();
	QStandardItemModel *l_fileModel=new QStandardItemModel(0,2);
	l_fileModel->setHorizontalHeaderItem(0,new QStandardItem("Fd"));
	l_fileModel->setHorizontalHeaderItem(1,new QStandardItem("File"));
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
	ui.openFiles->setModel(l_fileModel);
	ui.openFiles->resizeColumnsToContents();
	ui.openFiles->resizeRowsToContents();
}

/**
 *  Fill threads grid with information about threads in process (read from proc/pid/task)
 * 
 */

void TProcInfoDialog::fillThreats()
{
	QStandardItemModel *l_model=new QStandardItemModel(0,2);
	l_model->setHorizontalHeaderItem(0,new QStandardItem("Pid"));
	l_model->setHorizontalHeaderItem(1,new QStandardItem("Command"));
	TLinkListIterator<TProcessInfo> l_iter(info->getThreads());
	int l_rowCnt=0;
	TProcessInfo *l_pi;
	while(l_iter.hasNext()){
		l_pi=l_iter.next();		
		l_model->setItem(l_rowCnt,0,new QStandardItem(QString::number(l_pi->getPid())));
		l_model->setItem(l_rowCnt,1,new QStandardItem(l_pi->getComm()));
		l_rowCnt++;
	}
	ui.threads->setModel(l_model);
}


TProcInfoDialog::~TProcInfoDialog()
{
	if(processList != nullptr) delete processList;
}
