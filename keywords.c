#include <keywords.h>
/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

// Array with all the pascal keywords accepted by this compiler
char *keyword[] = {
	"begin",
	"not",
	"and",
	"or",
	"div",
	"mod",
	"program",
	"procedure",
	"function",
	"var",
	"boolean",
	"integer",
	"real",
	"double",
	"if",
	"then",
	"else",
	"while",
	"do",
	"repeat",
	"until",
	"true",
	"false",
	"return",
	"end",
};

/**********************************************************************************
 * This function checks if the variable name is present at the array of keywords. 
 * 
 * Returns the position of the keyword found or 0
 **********************************************************************************/
int iskeyword(const char *name)
{
	int i;
	for (i = BEGIN; i < END + 1; i++)
	{
		if (strcmp(keyword[i - BEGIN], name) == 0)
			return i;
	}
	return 0;
}
