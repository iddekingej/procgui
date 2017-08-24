#include <KLocalizedString>
#include "priority.h"
#include <QString>
#include <sys/time.h>
#include <sys/resource.h>
#include <QMessageBox>
#include "src/base/utils.h"
PriorityDialog::PriorityDialog(TProcessInfo* p_info)
{
   	ui.setupUi(this);
    pid=p_info->getPid();
    ui.processId->setText(QString::number(p_info->getPid()));
    ui.processName->setText(p_info->getComm());
    ui.priority->setText(QString::number(p_info->getPriority()));
    connect(ui.okButton,SIGNAL(clicked()),this,SLOT(pressOk()));
    connect(ui.cancelButton,SIGNAL(clicked()),this,SLOT(pressCancel()));
}

void PriorityDialog::errorMessage(QString p_message)
{
        QMessageBox l_mb(QMessageBox::Warning ,i18n("Error"),p_message,QMessageBox::Ok);
        l_mb.exec();
}


void PriorityDialog::pressCancel()
{
    close();
}


void PriorityDialog::pressOk()
{
    bool l_ok;
    int l_newPrio=ui.newPriority->text().toInt(&l_ok);    
    if(l_ok){
        if(l_newPrio <0){
               errorMessage(i18n("Invalid priority, value must be equal or biger than 0"));
        } else{
            errno=0;
            int l_return=setpriority(PRIO_PROCESS,pid,l_newPrio);
            printf("Set priority of  pid=%d to %d return=%d \n",pid,l_newPrio,l_return);
            if(errno ==0){
                close();
            } else {
                errorMessage(strerror(errno));
            }        
        }
    } else {
            errorMessage(i18n("Invalid number"));
    }
    
}
