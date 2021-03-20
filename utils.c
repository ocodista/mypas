#include <utils.h>

/************************************************
 * Function used to identify variable type id 
 * 
 * BOOL  -> b
 * INT32 -> l
 * INT64 -> q
 * FLT32 -> f
 * FLT64 -> df
 ************************************************/
char *get_var_type_id(int var_type)
{
    char *label = malloc(3);

    switch (var_type)
    {
    case BOOL:
        strcpy(label, "b");
        break;
    case INT32:
        strcpy(label, "l");
        break;
    case INT64:
        strcpy(label, "q");
        break;
    case FLT32:
        strcpy(label, "f");
        break;
    case FLT64:
        strcpy(label, "df");
        break;
    default:;
    }
    return label;
}

/************************************************
 * Function used to return different variable
 * Depending on var type
 * 
 * BOOL  -> (varname)b
 * INT32 -> (varname)l
 * INT64 -> (varname)q
 * FLT32 -> (varname)f
 * FLT64 -> (varname)df
 ************************************************/
char *get_var_label(int var_type, char *var_name)
{
    char *type_label = get_var_type_id(var_type);
    char *result = malloc(strlen(var_name) + strlen(type_label) + 1);
    strcpy(result, var_name);
    strcat(result, type_label);
    return result;
}

/******************************************
 * This function returns the name of 
 * the object type as string                 
 ******************************************/
char *get_object_type_label(int object_type)
{
    char *object_type_label = malloc(11);
    switch (object_type)
    {
    case OT_VARIABLE:
        object_type_label = "Variable";
        break;
    case OT_PROCEDURE:
        object_type_label = "Procedure";
        break;

    default:
        object_type_label = "Function";
        break;
    }
    return object_type_label;
}

/**********************************
 * Function to show error with line
 **********************************/
void show_error(char *message)
{
    fprintf(stderr, "Error at line %d => %s!\n", line_number, message);
}