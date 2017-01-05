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
#include <QHash>
#include <QHashIterator>
#include "hyfiller.h"       
#include <QStyledItemDelegate>
#include <QPainter>

/**
 *   This class is for painting grid lines in the tree
 * 
 */

class TGridDelegate : public QStyledItemDelegate
{
public:
    TGridDelegate(QObject* p_parent ) : QStyledItemDelegate(p_parent) { }
 
    void paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index ) const
    {
        p_painter->save();
        p_painter->setPen(QColor(Qt::lightGray));
        p_painter->drawRect(p_option.rect);
        p_painter->restore();
 
        QStyledItemDelegate::paint(p_painter, p_option, p_index);
    }
};

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
	connect(ui->displayAsTree,SIGNAL(clicked()),this,SLOT(checkDisplayAsTree()));
	fillProcessList();
	ui->displayAsTree->setCheckState(g_config.getDisplayAsTree()?Qt::Checked:Qt::Unchecked);
	ui->processList->setItemDelegate(new TGridDelegate(ui->processList));
}

TProcGui::~TProcGui()
{
    delete ui;
    if(processInfo!=nullptr)delete processInfo;
}


/**
 *   Fills/refresh the list with users. Before the refresh, the current selection is saved and restored after the refresh. 
 *   The list is sorted on the description. The associated data item  is the user id.
 */

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

/**
 *  There is a check box "Display as tree" on the main Gui. When this check box is checked ' unchecked this routine is called
 *  The check state is save in the configuration file and the process list is refreshed.
 */

void TProcGui::checkDisplayAsTree()
{
	g_config.setDisplayAsTree(ui->displayAsTree->checkState()==Qt::Checked);
	g_config.sync();
	fillProcessList();
}

/**
 *  Fills/refreshes  the process list 
 * 
 */

void TProcGui::fillProcessList()
{
	TProcessInfoList *l_info=new TProcessInfoList();
	l_info->readInfo();	
	fillUserFilter();
	THyrFiller l_filler(this,ui->processList,l_info);
	l_filler.setExeFilter(ui->exeFilter->text());
	l_filler.setUserFilter(ui->userFilter->itemData(ui->userFilter->currentIndex()).toUInt());
	l_filler.fillProcessList(g_config.getDisplayAsTree());
	if(processInfo==nullptr) delete processInfo;
	processInfo=l_info;
}

/** 
 *  Event handler for double clicking the process list
 *  When double clicking the process list, the details dialog with information about the selected process is shown.
 */
void TProcGui::doubleClickedGrid(const QModelIndex &p_index)
{
	
	uint l_pid;
	l_pid=p_index.sibling(p_index.row() ,0).data(Qt::UserRole+1).toUInt();//get selected pid
	if(processInfo!=nullptr){
			TProcessInfo *l_info=processInfo->getByPid(l_pid);
			if(l_info != nullptr){
				refresh.stop();
				TProcInfoDialog l_dialog(l_info,processInfo);
				l_dialog.exec();				
				refresh.start();
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
