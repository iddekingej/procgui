#ifndef __PRIORITY_H__
#define __PRIORITY_H__
#include <QDialog>
#include "src/data/processinfo.h"
#include "ui_priority.h"

class PriorityDialog:public QDialog
{
private:
        pid_t pid;
    	Q_OBJECT
private slots:
        void pressOk();
        void pressCancel();
        void errorMessage(QString p_message);
public:
        Ui::PriorityDialog ui;
        PriorityDialog(TProcessInfo *p_info);
};

#endif
