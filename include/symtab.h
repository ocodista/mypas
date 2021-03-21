/**@<symtab.h>::**/

#pragma once

#include <string.h>
#include <stdio.h>
#include <enums.h>
#include <constants.h>

typedef struct __symtab__
{
	char symbol[MAXIDLEN + 1];
	char offset[MAXIDLEN + 1];
	int type;
	int data_size;
	/** inserted on feb 17th: **/
	int lexical_level;
	int objtype;
	int transp_type;
} SYMTAB;

int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type);
int symtab_lookup(const char *);
void symtab_update_type(int, int);

// Variables declared at symtab.c
extern SYMTAB symtab[MAXSTBENTRIES];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
