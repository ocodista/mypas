/**@<symtab.h>::**/

#pragma once

#include <constants.h>
#include <enums.h>
#include <string.h>
#include <stdio.h>
#include <utils.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/*********************
 * symtab structure
 *********************/
typedef struct __symtab__
{
	char symbol[MAXIDLEN + 1];
	char offset[MAXIDLEN + 1];
	int type;
	int data_size;
	int lexical_level;
	int objtype;
	int transp_type;
} SYMTAB;
/**************************
 * end of symtab structure
 **************************/

/*******************************
 * symtab manipulation methods
 *******************************/
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type);
int symtab_lookup(const char *);
void symtab_update_type(int, int);
/*************************************
 * end of symtab manipulation methods
 *************************************/

// Variables declared at symtab.c
extern SYMTAB symtab[MAXSTBENTRIES];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
