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
#include <QTableView>
#include <klocalizedstring.h>
/**
 *  Main windows. This windows displays the processList.
 *  
 */



/**
 * A Treeview doesn't have grid lines.
 * This class is for painting grid lines in the tree.
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


/**
 *  Constructor
 *  Setup windows and initialize all events
 */

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
	connect(ui->detailsButton,SIGNAL(clicked()),this,SLOT(showDetails()));
	connect(ui->displayAsTree,SIGNAL(clicked()),this,SLOT(checkDisplayAsTree()));
	ui->displayAsTree->setCheckState(g_config.getDisplayAsTree()?Qt::Checked:Qt::Unchecked);
	ui->processList->setItemDelegate(new TGridDelegate(ui->processList));
	userSelection=new QTableView(this);
	ui->userFilter->setView(userSelection);
	userSelection->setSelectionBehavior(QAbstractItemView::SelectRows);
	userSelection->setSelectionMode(QAbstractItemView::SingleSelection);
	userSelection->verticalHeader()->setVisible(false);
	refreshTimeout();
}

TProcGui::~TProcGui()
{
    delete ui;
    if(processInfo!=nullptr)delete processInfo;
}


/**
 *   Fills/refresh the list with users. Before the refresh, the current selection is saved and restored after the refresh. 
 *   The list is sorted on the description. The associated data item  is the user id.
 *   When the id=UINT_MAX this means display processes from  all users
 * 
 *   \param p_processList - process list, this information is used for marking users with a running proces.
 */

void TProcGui::fillUserFilter(TProcessInfoList *p_processList)
{
	QMap<uint,QString> l_list;
	QSet<int> l_uids;

	getAllUsers(l_list);	
	p_processList->uidWithProcess(l_uids);
	QMapIterator<uint,QString> l_iter(l_list);
	uint l_selectedUid;
	QStandardItemModel *l_model=new QStandardItemModel(0,2);
	l_model->setHorizontalHeaderItem(0,new QStandardItem(i18n("User")));
	l_model->setHorizontalHeaderItem(1,new QStandardItem(i18n("Proces owner?")));
	
	if(ui->userFilter->currentIndex() ==-1){		
		l_selectedUid=getuid();
		if(l_selectedUid==0)l_selectedUid=UINT_MAX;//Root user? display all
	} else {
		l_selectedUid=ui->userFilter->itemData(ui->userFilter->currentIndex(),Qt::UserRole+1).toUInt();
	}
	ui->userFilter->blockSignals(true);
	int l_index=1;
	int l_selectedIndex=0;
	QStandardItem *l_item;
	l_item=new QStandardItem("All users");
	l_item->setData(UINT_MAX);
	l_model->setItem(0,0,l_item);
	l_model->setItem(0,1,new QStandardItem(""));			
	while(l_iter.hasNext()){
		l_iter.next();
		if(l_iter.key()==l_selectedUid) l_selectedIndex=l_index;
		l_item=new QStandardItem(l_iter.value());
		l_item->setData(l_iter.key());
		l_model->setItem(l_index,0,l_item);
		l_model->setItem(l_index,1,new QStandardItem(l_uids.contains(l_iter.key())?"X":""));		
		l_index++;
		
	}
	ui->userFilter->setModelColumn(0);
	ui->userFilter->setModel(l_model);
	ui->userFilter->setCurrentIndex(l_selectedIndex);	
	userSelection->resizeRowsToContents();
	userSelection->resizeColumnsToContents();
	ui->userFilter->blockSignals(false);
	l_model->sort(0);

}

/**
 *  There is a check box "Display as tree" on the main Gui. When this check box is checked ' unchecked this routine is called
 *  The check state is save in the configuration file and the process list is refreshed.
 */

void TProcGui::checkDisplayAsTree()
{
	g_config.setDisplayAsTree(ui->displayAsTree->checkState()==Qt::Checked);
	g_config.sync();
	fillProcessList(processInfo);
}

/**
 *  Fills/refreshes  the process list 
 * 
 */

void TProcGui::fillProcessList(TProcessInfoList* p_processList)
{

	THyrFiller l_filler(this,ui->processList,p_processList);
	l_filler.setExeFilter(ui->exeFilter->text());
	l_filler.setUserFilter(ui->userFilter->itemData(ui->userFilter->currentIndex(),Qt::UserRole+1).toUInt());
	l_filler.fillProcessList(g_config.getDisplayAsTree());
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
				TProcInfoDialog l_dialog(l_info);
				l_dialog.exec();				
				refresh.start();
			}
		
	}

}

/***
 *  When selected the "fields"  it is possible to selected the column displayed in the main window.
 *  This method  display the dialog
 */

void TProcGui::fieldsDialog()
{	
	TFieldsConfig l_fieldConfig;
	l_fieldConfig.exec();
	fillProcessList(processInfo);
}

/**
 * When the main window is re-sized the new size is stored in the configuration file.
 * After restarting the program the previous size of the main window  is restored.
 */

void TProcGui::resizeEvent(QResizeEvent *p_event)
{
	g_config.setMainWindowSize(p_event->size().width() ,p_event->size().height());
	g_config.sync();
}


/**
 * This is an time out event of the "refresh"  QTimer.
 * Periodically (by default 1s) the process data is refreshed in this routine.
 */
void TProcGui::refreshTimeout()
{
	
	TProcessInfoList *l_info=new TProcessInfoList();
	l_info->readInfo();	
	if(processInfo != nullptr) l_info->diff(processInfo);
	fillUserFilter(l_info);
	fillProcessList(l_info);
	if(processInfo != nullptr) delete processInfo;
	processInfo=l_info;
}

/**
 * On the main window there is a user selection combobox. Only the processes with has the selected owner is displayed
 * After changing the selected user, the event is called. In this function the list is refreshed 
 */

void TProcGui::userFilterChange(int p_index)
{
	fillProcessList(processInfo);
}

/**
 *  This slot is called when the use selects a process and clicks the "kill process" button. 
 *  First the selected process is determined and then the process is killed.
 */
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

/**
 *  This slot is called when the show details button is pressed. When no process is 
 *  selected a warning is displayed, otherwise 
 *  the details dialog is displayed (same dialog when double clicking the process)
 */

void TProcGui::showDetails()
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
