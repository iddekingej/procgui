#ifndef _HYRFILLER_H_
#define _HYRFILLER_H_

#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QModelIndexList>
#include <QList>
#include <QTreeView>
#include "src/data/processinfo.h"
#include "src/data/processinfolist.h"
#include "sortproxy.h"
class THyrFiller
{
private:
	QStandardItemModel *model;
	QList<QStandardItem *> newSelected;
	QList<QStandardItem *> newExpanded;
	QVector<int>       *fields;
	QTreeView          *processList;
	QString             exeFilter;
	uint                userFilter;
	TProcessInfoList   *processInfoList;
	QSet<int>           selected;
	QSet<int>           expanded;
	QStandardItem* makeRow(TProcessInfo* p_procInfo,int p_row, QStandardItem* p_item);
	void makeChildTreeRow(TProcessInfo *p_procInfo,int p_row,QStandardItem* p_item);
	void selectProcessChilderen(QModelIndex &p_item);
	void selectProcesses(TSortProxy *p_proxy);	
	void getSelected();
	void getExpandedChilderen(QModelIndex &l_item);
	void getExpanded();	
	void restoreExpand(TSortProxy *p_proxy);
	void restoreExpandChilderen(QModelIndex &p_item);



public:
	THyrFiller(QWidget* p_parent, QTreeView* p_treeView, TProcessInfoList* p_processInfoList);
	inline void setUserFilter(uint p_userFilter){ userFilter=p_userFilter;}
	inline void setExeFilter(const QString  &p_exeFilter){ exeFilter=p_exeFilter;}
	inline QStandardItemModel *getModel(){ return model;}
	void fillProcessList(bool p_asTree);
};

#endif
