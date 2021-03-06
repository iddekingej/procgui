#include "hyfiller.h"
#include "src/base/config.h"
#include "src/base/fieldlist.h"
#include <iostream>
#include "src/base/utils.h"
#include "sortproxy.h"

/**
 *  This class fills the process list as a hierarchical tree
 *  or a flat list.  (Depends on parameter of fillProcessList)
 * 
 */

THyrFiller::THyrFiller(QWidget *p_parent,QTreeView *p_treeView,TProcessInfoList *p_processInfoList)
{
	fields=g_config.getFields();
	processInfoList=p_processInfoList;
	exeFilter="";
	userFilter=UINT_MAX;
	processList=p_treeView;
	model=new QStandardItemModel(0,3,p_parent);
}

/**
 *  Fills one row of processlist with data from process 
 *  The fields displayed in the grid can be configured. First the configured list of fields (A QVector) is loaded 
 *  Next for each field in the row the cell is filled with the corresponding value;
 * 
 * \param  p_procInfo - Object representing process. Row is filled with data from this process
 * \param  p_row      - Row number (relative from parent)
 * \param  p_parent   - QStandardItem from parent row or null for root node(s)
 * \return            - QStandardItem from column 0. This object is used for adding child processes
 */
QStandardItem* THyrFiller::makeRow(TProcessInfo* p_procInfo,int p_row,  QStandardItem* p_parent)
{
		QVector<int> *l_fields=g_config.getFields();
		QVector<QString> l_row;
		p_procInfo->getInfo(l_row);
		QVectorIterator<int> l_fi(*l_fields);
		int l_col=0;	
		int l_fieldId;
		QStandardItem *l_return=nullptr;
		while(l_fi.hasNext()){
			l_fieldId=l_fi.next();
			QStandardItem *l_item=new QStandardItem(l_row[l_fieldId]);

			if(p_parent != nullptr){
				p_parent->setChild(p_row,l_col,l_item);
			} else {
				model->setItem(p_row,l_col,l_item);
			}
			if(l_col==0){
				l_item->setData(p_procInfo->getPid(),Qt::UserRole+1);			
				l_return=l_item;
			}
			l_col++;					
		}	
		return l_return;
}

/** 
 * Add row with information about proces under the row of the parent process
 * 
 * \param p_procInfo - Object with information about the process
 * \param p_row      - Row number (relative to parent)
 * \param p_item     - Parent QStandardItem to which the row is added
 */

void THyrFiller::makeChildTreeRow(TProcessInfo *p_procInfo,int p_row,QStandardItem* p_item)
{
	QStandardItem *l_item=makeRow(p_procInfo,p_row,p_item);
	QHash<uint,TProcessInfo *> *l_sub=p_procInfo->getSubProcesses();
	QHashIterator<uint,TProcessInfo*> l_processIterator(*l_sub);
	TProcessInfo *l_info;
	int l_row=0;
	while(l_processIterator.hasNext()){
		l_processIterator.next();
		l_info=l_processIterator.value();
		makeChildTreeRow(l_info,l_row,l_item);
		l_row++;
	}
}

void THyrFiller::restoreExpandChilderen(QModelIndex &p_item)
{
	int l_pid=p_item.data(Qt::UserRole+1).toInt();
	if(expanded.contains(l_pid)){		
		processList->expand(p_item);
	}
	int l_row=0;
	QModelIndex l_childIndex;
	while(true){
		l_childIndex=p_item.child(l_row,0);
		if(!l_childIndex.isValid()) break;
		restoreExpandChilderen(l_childIndex);
		l_row++;
	}
}


/**
 *  Before a fresh a list is filled with tree nodes that are expanded after the expand state is restorred
 * 
 */
void THyrFiller::restoreExpand(TSortProxy *p_proxy)
{
	QModelIndex l_index;
	for(int l_row=0;l_row<p_proxy->rowCount();l_row++){
		l_index=p_proxy->index(l_row,0,QModelIndex());	
		restoreExpandChilderen(l_index);
	}
	
}


void THyrFiller::selectProcessChilderen(QModelIndex& p_item)
{	
	int l_pid=p_item.data(Qt::UserRole+1).toInt();
	if(selected.contains(l_pid)){
		QItemSelectionModel *l_selectionModel=processList->selectionModel();
		l_selectionModel->select(p_item,QItemSelectionModel::Rows|QItemSelectionModel::Select);
	}
	int l_row=0;
	QModelIndex l_childIndex;
	while(true){
		l_childIndex=p_item.child(l_row,0);
		if(!l_childIndex.isValid()) break;
		selectProcessChilderen(l_childIndex);
		l_row++;
	}
}


/**
 *  Selects a process in QModelIndexList
 *  This currently used to preserve the selection after a refresh
 *
 *  \param p_list -  A list of QModelIndex   to select
 */

void THyrFiller::selectProcesses(TSortProxy *p_proxy)
{
	QModelIndex l_index;
	for(int l_row=0;l_row<p_proxy->rowCount();l_row++){
		l_index=p_proxy->index(l_row,0,QModelIndex());		
		selectProcessChilderen(l_index);
	}	
}

/**
 * Before the list is refreshed this route checks which row is selected. After the refresh the previous rows are selected again in 
 * the selectProcesses function. This way the selection is preserved after a refresh. 
 * The object variable selected is a set of pid's which are selected
 */

void THyrFiller::getSelected()
{
	
	if(processList->model() !=nullptr){
		if(processList->model()->rowCount()>0){
			QModelIndexList l_selected=processList->selectionModel()->selectedRows(0);
			QListIterator<QModelIndex> l_iter(l_selected);
			QModelIndex l_index;			
			while(l_iter.hasNext()){
				l_index=l_iter.next();
				if(l_index.isValid()){
					int  l_pid=l_index.data(Qt::UserRole + 1).toInt();				
					selected += l_pid;
				}
			}
		}
	}
}


/**
 *   Check which nodes are expanded. Called recursive from getExpanded 
 * 
 *   \param p_item  Check which children from p_item are expanded.
 */
void THyrFiller::getExpandedChilderen(QModelIndex &p_index)
{
	QModelIndex l_index;
	if(processList->isExpanded(p_index)){					
		expanded += p_index.data(Qt::UserRole + 1).toInt();			
	}
	int l_row=0;
	while(true){
		l_index=p_index.child(l_row,0);	
		if(!l_index.isValid()) break;
		getExpandedChilderen(l_index);		
		l_row++;
	}
}

/**
 *  Get which nodes from the process list are expanded, the expand 
 * state is restored after the process list is restored.
 * 
 */

void THyrFiller::getExpanded()
{
	TSortProxy *l_model=dynamic_cast<TSortProxy *>(processList->model());	
	QModelIndex l_index;

	if(l_model !=nullptr){		
		for(int l_cnt=0;l_cnt<l_model->rowCount();l_cnt++){
			l_index=l_model->index(l_cnt,0,QModelIndex());			
			getExpandedChilderen(l_index);
		}
	}
}

/**
 *  	Fill the process list.
 *  
 *      \param p_asTree true-fill it as a tree , or false=fill as flat list.
 */

void THyrFiller::fillProcessList(bool p_asTree)
{
	getSelected();
	getExpanded();
	TLinkListIterator<TProcessInfo> l_iter(processInfoList);
	TProcessInfo *l_pi;
	int l_cnt=0;
	int l_col;
	int l_fieldId;
	QVector<int> *l_fields=g_config.getFields();
	QVectorIterator<int> l_hi(*l_fields);
	l_col=0;

	while(l_hi.hasNext()){
		l_fieldId=l_hi.next();
		model->setHorizontalHeaderItem(l_col,new QStandardItem(g_fields[l_fieldId].title));
		
		l_col++;
	}
	l_col=0;
	while(l_iter.hasNext()){
		l_pi=l_iter.next();
		
		if((userFilter != UINT_MAX) && (l_pi->getOwnerId() != userFilter)) continue;
		if(exeFilter.length()>0){
			if(l_pi->getExe().indexOf(exeFilter)!=-1){
				if(p_asTree){
					makeChildTreeRow(l_pi,l_cnt,nullptr);
				} else {
					makeRow(l_pi,l_cnt,nullptr);
				}
				l_cnt++;
			}
		} else {
			if(p_asTree){
				if(userFilter ==UINT_MAX){
					if(l_pi->getParent()!=nullptr)continue;
				} else {
					if(l_pi->getParent() != nullptr){
						if(l_pi->getParent()->getOwnerId() == userFilter)continue;
					}
				}
				makeChildTreeRow(l_pi,l_cnt,nullptr);
			} else {
				makeRow(l_pi,l_cnt,nullptr);
			}
			l_cnt++;		
		}
	}
	processList->setUpdatesEnabled(false);
	
		
	QAbstractItemModel *l_originalModel=processList->model();
	TSortProxy *l_proxy=dynamic_cast<TSortProxy *>(l_originalModel);
	
	if(l_proxy != nullptr){	
		l_proxy->setSourceModel(model);
		selectProcesses(l_proxy);	
		restoreExpand(l_proxy);
	} else {
		setViewModel(processList,model);	
	}
	
	
	processList->setUpdatesEnabled(true);
}
