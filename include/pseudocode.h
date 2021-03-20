#pragma once
#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void L_value(int, const char *name);
void R_value(int, const char *name);
void add(int);
void subtract(int);
void multiply(int);
void divide(int);
void negate(int type);
void push(int);
void mov(int type, const char *dest, const char *src);
void gofalse(int value);
void golabel(int value);
void mklabel(int value);
