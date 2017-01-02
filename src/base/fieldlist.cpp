#include "fieldlist.h"
#include <klocalizedstring.h>
//List of fields shown in "Device" tab


const  char *g_fields[]={
	I18N_NOOP("Pid")
,	I18N_NOOP("Parent pid")
,	I18N_NOOP("Executable")
,	I18N_NOOP("Command")
,	I18N_NOOP("State")
,	I18N_NOOP("Owner id")
,	I18N_NOOP("Owner username")
,	I18N_NOOP("Current working directory")
,	I18N_NOOP("Command line")
};
const  int g_numFields=sizeof(g_fields)/sizeof(char*);


