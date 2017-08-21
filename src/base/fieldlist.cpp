#include "fieldlist.h"
#include <klocalizedstring.h>
/**
 *  List of fields displayed in the proceslist. 
 *  The selected fields are save as indexes of this array
 *  The text is used as the column header. (please don't change the order)
 */

const  TFieldInfo g_fields[]={
	{I18N_NOOP("Pid"),'N'}
,	{I18N_NOOP("Parent pid"),'N'}
,	{I18N_NOOP("Executable"),'S'}
,	{I18N_NOOP("Command"),'S'}
,	{I18N_NOOP("State"),'S'}
,	{I18N_NOOP("Owner id"),'N'}
,	{I18N_NOOP("Owner user name"),'S'}
,	{I18N_NOOP("Current working directory"),'S'}
,	{I18N_NOOP("Command line"),'S'}
,	{I18N_NOOP("Start time"),'S'}
,	{I18N_NOOP("Process group ID"),'N'}
,	{I18N_NOOP("Session ID"),'N'}
,	{I18N_NOOP("Virtual mem. size"),'N'}
,	{I18N_NOOP("Res. mem. size"),'N'}
,	{I18N_NOOP("User time"),'S'}
,	{I18N_NOOP("System time"),'S'}
,	{I18N_NOOP("Diff. user time"),'S'}
,	{I18N_NOOP("Diff. system time"),'N'}
,	{I18N_NOOP("Total time"),'S'}
,	{I18N_NOOP("Diff. total time"),'N'}
,   {I18N_NOOP("Priority"),'N'}
};


const  int g_numFields=sizeof(g_fields)/sizeof(TFieldInfo);


