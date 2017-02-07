#ifndef _FIELDLIST_H_
#define _FIELDLIST_H_
/**
 *  Definition of fields available for the main grid
 */
typedef struct {
	const char *title; //Title of field
	const char type;   //Sort type of field
} TFieldInfo;

extern const  TFieldInfo g_fields[];
extern const  int g_numFields;

#endif
