#pragma once
#include <enums.h>
#include <string.h>
#include <stdlib.h>
#include <lexer.h>

extern int line_number;

char *get_object_type_label(int object_type);

char *get_var_type_id(int var_type);
char *get_var_label(int var_type, char *var_name);

void show_error(char *message);
