

#pragma once
#include <string.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/*********************************
 * Pascal accepted keywords enum
 *********************************/
enum
{
	BEGIN = 10001,
	NOT,
	AND,
	OR,
	DIV,
	MOD,
	PROGRAM,
	PROCEDURE,
	FUNCTION,
	VAR,
	BOOLEAN,
	INTEGER,
	REAL,
	DOUBLE,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	TRUE,
	FALSE,
	RETURN,
	END,
};

// Checks if keyword is present at keywords enum
int iskeyword(const char *);
