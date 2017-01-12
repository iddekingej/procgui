#ifndef _PROCGUI_H
#define _PROCGUI_H

#include <QMainWindow>
#include "ui_procgui.h"
#include <QResizeEvent>
#include <QTimer>
#include <QTableView>
#include <QModelIndexList>
#include <QStandardItemModel>
#include "src/data/processinfolist.h"
#include <QTableView>
class TProcGui : public QMainWindow
{
    Q_OBJECT
private slots:
	void fieldsDialog();
	void resizeEvent(QResizeEvent *p_event);
	void refreshTimeout();
	void userFilterChange(int p_index);
	void doubleClickedGrid(const QModelIndex &p_index);
	void killProcess();
	void showDetails();
	void checkDisplayAsTree();
private:
	Ui::procgui *ui;    
	QTimer refresh;
	TProcessInfoList *processInfo=nullptr;
	QTableView *userSelection;
	void fillProcessList(TProcessInfoList *p_processList);
	void fillUserFilter(TProcessInfoList *p_processList);
public:
	explicit TProcGui(QWidget *parent = 0);
	~TProcGui();
};

#endif // PROCGUI_H
