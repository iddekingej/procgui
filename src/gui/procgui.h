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
#include "sortproxy.h"
#include <QTableView>

/**
 * Class for main window gui 
 */


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
	/**
	 * Gui layout
	 */ 
	Ui::procgui *ui;    
	
	/**
	 * Timer used for refreshing the  proces list
	 */
	
	QTimer refresh;
	
	/**
	 * Informaton about processes read from /proc
	 */
	TProcessInfoList *processInfo=nullptr;
	
	/**
	 * This grid is attached to the user selection combobox.
	 * It is used for displaying user information in the drop down list.
	 * 
	 */
	QTableView *userSelection;
	void fillProcessList(TProcessInfoList *p_processList);
	void fillUserFilter(TProcessInfoList *p_processList);
public:
	explicit TProcGui(QWidget *parent = 0);
	~TProcGui();
};

#endif // PROCGUI_H
