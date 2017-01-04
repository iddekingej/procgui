#ifndef PROCGUI_H
#define PROCGUI_H

#include <QMainWindow>
#include "ui_procgui.h"
#include <QResizeEvent>
#include <QTimer>
#include <QTableView>
#include <QModelIndexList>
#include <QStandardItemModel>
#include "src/data/processinfolist.h"
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
	void editDetails();
	void checkDisplayAsTree();
private:
	Ui::procgui *ui;    
	QTimer refresh;
	TProcessInfoList *processInfo=nullptr;
	void fillProcessList();
	void fillUserFilter();
public:
	explicit TProcGui(QWidget *parent = 0);
	~TProcGui();
};

#endif // PROCGUI_H
