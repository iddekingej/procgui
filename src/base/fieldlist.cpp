#include "fieldlist.h"
#include <klocalizedstring.h>
/**
 *  List of fields showen in the proceslist. 
 *  The selected fields are save as indexes of this array
 *  The text is used as the column header. (please don't change the order)
 */


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
,	I18N_NOOP("Start time")
,	I18N_NOOP("Process group ID")
,	I18N_NOOP("Session ID")
,	I18N_NOOP("Virtual mem. size")
,	I18N_NOOP("Res. mem. size")
,	I18N_NOOP("User time")
,	I18N_NOOP("System time")
,	I18N_NOOP("Diff. user time")
,	I18N_NOOP("Diff. system time")
};
const  int g_numFields=sizeof(g_fields)/sizeof(char*);


