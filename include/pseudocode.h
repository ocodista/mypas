#pragma once
#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <enums.h>

/****************************
 * assign/retrive functions
 ****************************/
void L_value(int, const char *name);
void R_value(int, const char *name);
void mov(int type, const char *dest, const char *src);
/**********************************
 * end of assign/retrive functions
 **********************************/

/**********************
 * jump functions
 **********************/
void gofalse(int value);
void golabel(int value);
/**********************
 * end jump functions
 **********************/

/*****************
 * math functions
 *****************/
void add(int);
void divide(int);
void multiply(int);
void negate(int type);
void subtract(int);
/************************
 * end of math functions
 ************************/

/************************
 * variable functions
 ************************/
void cmp(int relop, int var_type, char *left_operator, char *right_operator);
void mklabel(int value);
void push(int);
/************************
 * end variable functions
 ************************/