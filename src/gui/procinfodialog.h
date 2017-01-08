#ifndef __PROCINFO_H_
#define __PROCINFO_H_
#include <QDialog>
#include "src/data/processinfo.h"
#include "src/data/processinfolist.h"
#include "ui_procinfodialog.h"
#include <QTimer>

class TProcInfoDialog:public QDialog
{
	Q_OBJECT
private slots:
	void refreshInfo();
private:
	TProcessInfo *info;
	TProcessInfoList *list;
	Ui::procinfo ui;
	QTimer refresh;
	void fillData(TProcessInfo *p_processInfo);
	void fillThreats();
public:
	TProcInfoDialog(TProcessInfo *p_procInfo,TProcessInfoList *p_procInfoList);
	~TProcInfoDialog();
};
#endif
