/**@<symtab.c>::**/
#include <symtab.h>
/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/**********************************************
 * Declaration of symtab structure at symtab.h
 * 
 * symtab array holds all the identifiers of 
 * variable, function and procedure present 
 * on the input code
 **********************************************/
SYMTAB symtab[MAXSTBENTRIES];

/*******************************************************************
 * This variable holds the last inserted(current) symbol position
 * Registered in the symtab, always set with count of symbols
 *******************************************************************/
int symtab_entry;

/*******************************************************************
 * This variable holds the next free position in symtab
 * Registered in the symtab, always set with (count of symbols + 1)
 *******************************************************************/
int symtab_next_entry = 0;

/**************************************************************
 * This function check if string symbol exists at symtab array
 * 
 * Return symbol position or 0
 **************************************************************/
int symtab_lookup(const char *symbol)
{
	for (symtab_entry = symtab_next_entry - 1; symtab_entry > -1; symtab_entry--)
	{
		if (strcmp(symtab[symtab_entry].symbol, symbol) == 0)
			return symtab_entry;
	}
	return symtab_entry = -1;
}

/**************************************************************
 * This function tryes to insert a new symbol at symtab
 * 
 * Throws error if symbol is already present at the same lexical level
 * 
 * Return symbol position or -2
 **************************************************************/
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type)
{
	char message[100];
	if (symtab_lookup(symbol) < 0 || symtab[symtab_entry].lexical_level <= lexical_level)
	{
		strcpy(symtab[symtab_next_entry].symbol, symbol);
		sprintf(symtab[symtab_next_entry].offset, "%s[%d]", symbol, lexical_level);
		symtab[symtab_next_entry].lexical_level = lexical_level;
		symtab[symtab_next_entry].objtype = objtype;
		symtab[symtab_next_entry].transp_type = transp_type;
		return symtab_next_entry++;
	}
	else
	{
		snprintf(message, strlen(message), "%s is already defined in current lexical level %d!", symbol, lexical_level);
		show_error(message);
		semantic_error++;
		return -2;
	}
}

/***********************************************************************
 * This function updates the type and data size of newly inserted symbols 
 * 
 * Update table
 * TYPE  - SIZE
 * BOOL  -> 1
 * INT32 -> 4
 * FLT32 -> 4
 * INT64 -> 8
 * FLT64 -> 8
 * OTHER -> 0
 **********************************************************************/
void symtab_update_type(int start, int type)
{
	int i;
	for (i = start; i < symtab_next_entry; i++)
	{
		symtab[i].type = type;
		switch (type)
		{
		case BOOL:
			symtab[i].data_size = 1;
			break;
		case INT32:
		case FLT32:
			symtab[i].data_size = 4;
			break;
		case INT64:
		case FLT64:
			symtab[i].data_size = 8;
			break;
		default:
			symtab[i].data_size = 0;
		}
	}
}