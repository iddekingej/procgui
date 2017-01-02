#ifndef __PROCINFO_H_
#define __PROCINFO_H_
#include <QDialog>
#include "src/data/processinfo.h"
#include "src/data/processinfolist.h"
#include "ui_procinfodialog.h"
class TProcInfoDialog:public QDialog
{
private:
	TProcessInfo *info;
	TProcessInfoList *list;
	Ui::procinfo ui;
	void fillData();
	void fillThreats();
public:
	TProcInfoDialog(TProcessInfo *p_procInfo,TProcessInfoList *p_procInfoList);
	~TProcInfoDialog();
};
#endif
