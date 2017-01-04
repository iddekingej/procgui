#ifndef __HYRFILLER_H_
#define __HYRFILLER_H_

#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QModelIndexList>
#include <QTreeView>
#include "src/data/processinfo.h"
#include "src/data/processinfolist.h"
class THyrFiller
{
private:
	QStandardItemModel *model;
	QModelIndexList     newSelected;
	QModelIndexList     newExpanded;
	QVector<int>       *fields;
	QTreeView          *processList;
	QString             exeFilter;
	uint                userFilter;
	TProcessInfoList   *processInfoList;
	QSet<int>           selected;
	QSet<int>           expanded;
	QStandardItem* makeRow(TProcessInfo* p_procInfo,int p_row, QStandardItem* p_item);
	void makeChildTreeRow(TProcessInfo *p_procInfo,int p_row,QStandardItem* p_item);
	void selectProcesses(QModelIndexList& p_list);
	void getSelected();
	void getExpandedChilderen(QStandardItem *l_item);
	void getExpanded();
	void restoreExpanded();



public:
	THyrFiller(QWidget* p_parent, QTreeView* p_treeView, TProcessInfoList* p_processInfoList);
	inline void setUserFilter(uint p_userFilter){ userFilter=p_userFilter;}
	inline void setExeFilter(const QString  &p_exeFilter){ exeFilter=p_exeFilter;}
	inline QStandardItemModel *getModel(){ return model;}
	void fillProcessList(bool p_asTree);
};

#endif
