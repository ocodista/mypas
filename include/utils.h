#pragma once
#include <enums.h>
#include <string.h>
#include <stdlib.h>
#include <lexer.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

// Declaration at lexer.c
extern int line_number;

// Functions used to transform enum/type into string
char *get_object_type_label(int object_type);
char *get_var_type_id(int var_type);
char *get_var_label(int var_type, char *var_name);

// Function to print compilation error
void show_error(char *message);
