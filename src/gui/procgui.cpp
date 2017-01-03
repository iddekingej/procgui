#include <QStandardItemModel>
#include <QVectorIterator>
#include "procgui.h"
#include "ui_procgui.h"
#include "src/data/processinfolist.h"
#include "src/base/linklist.h"
#include "src/gui/fieldconfig.h"
#include "src/base/config.h"
#include "src/base/os.h"
#include <QString>
#include <QMapIterator>
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <QModelIndex>
#include "procinfodialog.h"
#include <QListIterator>
#include <sys/types.h>
#include <signal.h>
#include <QMessageBox>
       
TProcGui::TProcGui(QWidget *parent) : QMainWindow(parent), ui(new Ui::procgui)
{
	ui->setupUi(this);
	int l_width= g_config.getMainWindowWidth();
	int l_height=g_config.getMainWindowHeight();
	if((l_width>0) && (l_height>0)){
		resize(l_width,l_height);
	}
	refresh.setInterval(1024);
	refresh.start();
	connect(ui->action_Exit,SIGNAL(triggered()),qApp,SLOT(quit()));
	connect(ui->actionFields,SIGNAL(triggered()),this,SLOT(fieldsDialog()));
	connect(&refresh,SIGNAL(timeout()),this,SLOT(refreshTimeout()));
	connect(ui->userFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(userFilterChange(int)));
	connect(ui->processList,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(doubleClickedGrid(const QModelIndex &)));
	connect(ui->killButton,SIGNAL(clicked()),this,SLOT(killProcess()));
	connect(ui->detailsButton,SIGNAL(clicked()),this,SLOT(editDetails()));
	fillProcessList();
	
}

TProcGui::~TProcGui()
{
    delete ui;
}




void TProcGui::fillUserFilter()
{
	QMap<uint,QString> l_list;
	getAllUsers(l_list);	
	QMapIterator<uint,QString> l_iter(l_list);
	uint l_selectedUid;
	if(ui->userFilter->currentIndex() ==-1){		
		l_selectedUid=getuid();
		if(l_selectedUid==0)l_selectedUid=UINT_MAX;//Root user? display all
	} else {
		l_selectedUid=ui->userFilter->itemData(ui->userFilter->currentIndex()).toUInt();
	}
	ui->userFilter->blockSignals(true);
	ui->userFilter->clear();
	int l_index=1;
	int l_selectedIndex=0;
	ui->userFilter->addItem("All users",UINT_MAX);
	while(l_iter.hasNext()){
		l_iter.next();
		if(l_iter.key()==l_selectedUid) l_selectedIndex=l_index;
		ui->userFilter->addItem(l_iter.value(),l_iter.key());
		l_index++;
	}
	ui->userFilter->setCurrentIndex(l_selectedIndex);
	ui->userFilter->model()->sort(0);
	ui->userFilter->blockSignals(false);
}


QSet<int> TProcGui::getSelectedProcess()
{
	QSet<int> l_set;
	if(ui->processList->model() !=nullptr){
		if(ui->processList->model()->rowCount()>0){
			QModelIndexList l_selected=ui->processList->selectionModel()->selectedRows(0);
			QListIterator<QModelIndex> l_iter(l_selected);
			QModelIndex l_index;			
			while(l_iter.hasNext()){
				l_index=l_iter.next();
				if(l_index.isValid()){
					int  l_pid=l_index.data(Qt::UserRole + 1).toInt();				
					l_set += l_pid;
				}
			}
		}
	}
	return l_set;
}

void TProcGui::selectProcesses(QModelIndexList& p_list)
{
	QListIterator<QModelIndex> l_selectIter(p_list);
	QModelIndex  l_index;
	QItemSelectionModel *l_selectionModel=ui->processList->selectionModel();
	while(l_selectIter.hasNext()){
		l_index=l_selectIter.next();
		l_selectionModel->select(l_index,QItemSelectionModel::Rows|QItemSelectionModel::Select);
	}	
}


void TProcGui::fillProcessList()
{
	TProcessInfoList *l_info=new TProcessInfoList();
	l_info->readInfo();
	
	QSet<int> l_selected=getSelectedProcess();
	
	QStandardItemModel *l_model=new QStandardItemModel(0,3,this);
	TLinkListIterator<TProcessInfo> l_iter(l_info);
	TProcessInfo *l_pi;
	QVector<QString> l_row;
	int l_cnt=0;
	int l_col;
	int l_fieldId;
	
	fillUserFilter();
	QVector<int> *l_fields=g_config.getFields();
	QVectorIterator<int> l_hi(*l_fields);
	l_col=0;
	QVariant l_selectedUidV=ui->userFilter->itemData(ui->userFilter->currentIndex());
	uint l_selectedUid=l_selectedUidV.toUInt();
	QString l_exeFilter=ui->exeFilter->text();
	while(l_hi.hasNext()){
		l_fieldId=l_hi.next();
		l_model->setHorizontalHeaderItem(l_col,new QStandardItem(g_fields[l_fieldId]));
		l_col++;
	}
	l_col=0;
	QModelIndexList l_newSelection;
	while(l_iter.hasNext()){
		l_pi=l_iter.next();
		
		if(l_exeFilter.length()>0){
			if(l_pi->getExe().indexOf(l_exeFilter)==-1) continue;
		}
		if((l_selectedUid != UINT_MAX) && (l_pi->getOwnerId() != l_selectedUid)) continue;
		l_pi->getInfo(l_row);
		QVectorIterator<int> l_fi(*l_fields);
		l_col=0;		
		while(l_fi.hasNext()){
			l_fieldId=l_fi.next();
			QStandardItem *l_item=new QStandardItem(l_row[l_fieldId]);
			if(l_col==0){
				l_item->setData(l_pi->getPid());
			}
			l_model->setItem(l_cnt,l_col,l_item);
			l_col++;
		
			
		}
		if(l_selected.contains(l_pi->getPid()))l_newSelection << l_model->index(l_cnt,0);
		l_cnt++;		
	}
	ui->processList->setModel(l_model);
	ui->processList->resizeRowsToContents();
	ui->processList->resizeColumnsToContents();
	selectProcesses(l_newSelection);
	if(processInfo==nullptr) delete processInfo;
	processInfo=l_info;
}


void TProcGui::doubleClickedGrid(const QModelIndex &p_index)
{
	
	uint l_pid;
	l_pid=p_index.sibling(p_index.row() ,0).data(Qt::UserRole+1).toUInt();//get selected pid
	if(processInfo!=nullptr){
			TProcessInfo *l_info=processInfo->getByPid(l_pid);
			if(l_info != nullptr){
				TProcInfoDialog l_dialog(l_info,processInfo);
				l_dialog.exec();				
			}
		
	}

}

void TProcGui::fieldsDialog()
{
	TFieldsConfig l_fieldConfig;
	l_fieldConfig.exec();
	fillProcessList();
}

void TProcGui::resizeEvent(QResizeEvent *p_event)
{
	g_config.setMainWindowSize(p_event->size().width() ,p_event->size().height());
	g_config.sync();
}

void TProcGui::refreshTimeout()
{
	fillProcessList();
}

void TProcGui::userFilterChange(int p_index)
{
	fillProcessList();
}

void TProcGui::killProcess()
{
	QModelIndexList l_list=ui->processList->selectionModel()->selectedRows();
	QListIterator<QModelIndex> l_iter(l_list);
	QModelIndex l_index;
	while(l_iter.hasNext()){
		l_index=l_iter.next();
		int  l_pid=l_index.data(Qt::UserRole + 1).toInt();
		kill(l_pid,SIGKILL);
	}
	
}


void TProcGui::editDetails()
{
	QModelIndexList l_list=ui->processList->selectionModel()->selectedRows();
	if(l_list.size()>0){		
		doubleClickedGrid(l_list.first());
	} else{
		QMessageBox l_box;
		l_box.setText("Please select a process first");
		l_box.setStandardButtons(QMessageBox::Ok);		
		l_box.exec();
	}
}
