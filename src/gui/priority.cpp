#include "priority.h"
#include <QString>
#include <sys/time.h>
#include <sys/resource.h>
#include "src/base/utils.h"
PriorityDialog::PriorityDialog(TProcessInfo* p_info)
{
   	ui.setupUi(this);
    pid=p_info->getPid();
    ui.processId->setText(QString::number(p_info->getPid()));
    ui.processName->setText(p_info->getComm());
    ui.priority->setText(QString::number(p_info->getPriority()));
    connect(ui.okButton,SIGNAL(clicked()),this,SLOT(pressOk()));
}

void PriorityDialog::pressCancel()
{
}

void PriorityDialog::pressOk()
{
    bool l_ok;
    int l_newPrio=ui.priority->text().toInt(&l_ok);    
    if(l_ok){
        errno=0;
        setpriority(PRIO_PROCESS,pid,l_newPrio);
        if(errno ==0){
            close();
        }
        printf("Error=%d\n",errno);
    }
    
}
