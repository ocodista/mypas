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

void mypas(void);

void declarative(void);
void imperative(void);
void vardecl(void);
void sbpdecl(void);
void varlist(void);
int typemod(void);

void procedure(void);
void function(void);

void sbrhead(void);
void sbrtail(void);
void formparm(void);

void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);

int expr(int);
int smpexpr(int);
int term(int);
int fact(int);

int do_relop(int expr_type, int left_type);
int mov_compat_type(int obj_type);
int mov_var_type(int fact_type, int var_type);

int validate_l_value(int expr_type, int fact_type, char *name);
int validate_r_value(int fact_type, char *name);

void match(int expected);

extern int lookahead;
extern char lexeme[];
extern FILE *source;
