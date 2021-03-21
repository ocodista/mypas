/**@<parser.h>::**/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>
#include <keywords.h>
#include <symtab.h>
#include <enums.h>
#include <pseudocode.h>
#include <utils.h>
#include <lexer.h>

/*********************
 * external variables
 *********************/
// Declared at mypas.c
extern FILE *source;
extern int lookahead;

// Declared at lexer.c
extern char lexeme[];
/****************************
 * end of external variables
 ****************************/

/******************
 * entry methods
 *****************/
void mypas(void);
void declarative(void);
void imperative(void);
/******************
 * end of entry methods
 *****************/

/****************************
 * declaration methods
 ****************************/
void sbpdecl(void);
void vardecl(void);
void varlist(void);
int typemod(void);
/****************************
 * end of declaration methods
 ****************************/

/***************************
 * Procedure/Function methods
 ***************************/
void formparm(void);
void function(void);
void procedure(void);
void sbp_end(void);
void sbrhead(void);
void sbrtail(void);
/***************************
 * End of procedure/function methods
 ***************************/

/****************************
 * statement methods
 ****************************/
void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);
/****************************
 * end of statement methods
 ****************************/

/****************************
 * math methods
 ****************************/
int expr(int);
int smpexpr(int);
int term(int);
int fact(int);
int do_relop(int expr_type, int left_type);
int do_otimes(int term_type, int fact_type);
int do_oplus(int term_type, int smpexpr_type);
/****************************
 * end of math methods
 ****************************/

/****************************
 * validation methods
 ****************************/
int iscompat(int acc_type, int syn_type);
int isrelop(void);
void match(int expected);
/****************************
 * end of validation methods
 ****************************/

/****************************
 * pseudocode help methods
 ****************************/
int mov_compat_type(int obj_type);
int mov_var_type(int fact_type, int var_type);
int validate_l_value(int expr_type, int fact_type, char *name);
int validate_r_value(int fact_type, char *name);
/****************************
 * end of pseudocode help methods
 ****************************/