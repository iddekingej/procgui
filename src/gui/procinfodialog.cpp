#include "procinfodialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHashIterator>
#include "src/base/linklist.h"

TProcInfoDialog::TProcInfoDialog(TProcessInfo *p_procInfo,TProcessInfoList *p_procInfoList)
{
	ui.setupUi(this);
	info=p_procInfo;
	list=p_procInfoList;
	fillData();
}

void TProcInfoDialog::fillData()
{
	ui.pid_label->setText(QString::number(info->getPid()));
	ui.ppid_label->setText(QString::number(info->getPPid()));
	ui.command_label->setText(info->getExe());
	if(info->getParent()!=nullptr){
		ui.parentCommand_label->setText(info->getParent()->getExe());
	} else {
		ui.parentCommand_label->setText("");
	}
	ui.owner_label->setText(info->getOwnerName());
	ui.state_label->setText(info->stateString());	
	ui.cwd_label->setText(info->getCWD());
	ui.startTime_label->setText(info->timeToString(info->getStartTime()));
	ui.processGroupId_label->setText(QString::number(info->getProcessGroupId()));
	QStandardItemModel *l_model=new QStandardItemModel(0,3);
	l_model->setHorizontalHeaderItem(0,new QStandardItem("Pid"));
	l_model->setHorizontalHeaderItem(1,new QStandardItem("Command"));
	l_model->setHorizontalHeaderItem(2,new QStandardItem("Command line"));
	QHashIterator<uint,TProcessInfo *> l_iter(*(info->getSubProcesses()));
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
}

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
}
