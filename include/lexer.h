/**@<lexer.h>::**/
#pragma once
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tokens.h>
#include <constants.h>
#include <keywords.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/*********************
 * external variables
 *********************/
// Variables declared at mypas.c
extern int lookahead;
extern FILE *source;
/****************************
 * end of external variables
 ****************************/

/******************
 * buffer methods
 *****************/
int gettoken(FILE *source);
void skipunused(FILE *tape);
/************************
 * end of buffer methods
 ************************/

/****************************
 * type validation methods
 ***************************/
int isID(FILE *tape);
int isUINT(FILE *tape);
int isNUM(FILE *tape);
int isOCT(FILE *tape);
int isHEX(FILE *tape);
int isASGN(FILE *tape);
int isRELOP(FILE *tape);
int isCOMMA(FILE *tape);
int isCOLON(FILE *tape);
int isSEMICOLON(FILE *tape);
int isOPEN_PARENTHESES(FILE *tape);
int isCLOSE_PARENTHESES(FILE *tape);
int isDOT(FILE *tape);
/******************
 * entry methods
 *****************/