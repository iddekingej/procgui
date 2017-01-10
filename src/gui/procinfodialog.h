#ifndef _PROCINFODIALOG_H_
#define _PROCINFODIALOG_H_
#include <QDialog>
#include "src/data/processinfo.h"
#include "src/data/processinfolist.h"
#include "ui_procinfodialog.h"
#include <QTimer>
/**
 *   When clicking a process this dialog is popped-up with detail 
 *   information about the selected process.
 */
class TProcInfoDialog:public QDialog
{
	Q_OBJECT
private slots:
	void refreshInfo();
private:
	TProcessInfo *info;
	TProcessInfoList *processList=nullptr;
	Ui::procinfo ui;
	QTimer refresh;
	void fillData(TProcessInfo *p_processInfo);
	void fillThreats();
public:
	TProcInfoDialog(TProcessInfo *p_procInfo);
	~TProcInfoDialog();
};
#endif
