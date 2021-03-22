#pragma once
#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <enums.h>
#include <utils.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

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
void add(int var_type);
void divide(int var_type);
void multiply(int var_type);
void negate(int var_type);
void subtract(int var_type);
/************************
 * end of math functions
 ************************/

/************************
 * stack functions
 ************************/
void pop(int var_type, char *address);
void push(int var_type, char *address);
/************************
 * end of stack functions
 ************************/

/************************
 * variable functions
 ************************/
void cmp(int relop, int var_type, char *left_operator, char *right_operator);
void mklabel(int value);
/************************
 * end variable functions
 ************************/