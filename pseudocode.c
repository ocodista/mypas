#include <pseudocode.h>
//TODO: FINISH COMMENTING THIS

/************************************************
 * Function used to identify variable type label 
 * 
 * INT32 -> l
 * INT64 -> q
 * FLT32 -> f
 * FLT64 -> df
 ************************************************/
char *get_type_label(int var_type)
{
	char *label = malloc(3);

	switch (var_type)
	{
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

/**********************************************
 * Function to retrieve value from accumulator
 * And store into var "name"
 * 
 * NOTE: Each type has its own accumulator
 **********************************************/
void R_value(int var_type, const char *name)
{
	char *type_label = get_type_label(var_type);
	printf("\tmov%s %s, acc%s\n", type_label, name, type_label);
}

void L_value(int var_type, const char *name)
{
	char *type_label = get_type_label(var_type);
	printf("\tmov%s acc%s, %s\n", type_label, type_label, name);
}

void add(int var_type)
{
	char *type_label = get_type_label(var_type);
	printf("\tpop%s reg%s\n", type_label, type_label);
	printf("\tadd%s reg%s, acc%s\n", type_label, type_label, type_label);
}

void subtract(int var_type)
{
	char *type_label = get_type_label(var_type);
	printf("\tmov%s acc%s, reg%s\n", type_label, type_label, type_label);
	printf("\tpop%s acc%s\n", type_label, type_label);
	printf("\tsub%s reg%s, acc%s\n", type_label, type_label, type_label);
}
void multiply(int var_type)
{
	char *type_label = get_type_label(var_type);
	printf("\tpop%s reg%s\n", type_label, type_label);
	printf("\tmul%s reg%s, acc%s\n", type_label, type_label, type_label);
}
void divide(int var_type)
{
	char *type_label = get_type_label(var_type);

	printf("\tmov%s acc%s, reg%s\n", type_label, type_label, type_label);
	printf("\tpop%s acc%s\n", type_label, type_label);
	printf("\tdiv%s reg%s, acc%s\n", type_label, type_label, type_label);
}

void negate(int var_type)
{
	char *type_label = get_type_label(var_type);
	printf("\tnegate%s acc%s\n", type_label, type_label);
}
void push(int var_type)
{
	char *type_label = get_type_label(var_type);
	printf("\tpush%s acc%s\n", type_label, type_label);
}

void mov(int var_type, const char *dest, const char *src)
{
	char *typelabel = get_type_label(var_type);
	printf("\tmov%s %s, %s\n", typelabel, dest, src);
}

/**********************
 * jump functions
 **********************/
void gofalse(int value)
{
	printf("\tgofalse .L%d\n", value);
}

void golabel(int value)
{
	printf("\tgoto .L%d\n", value);
}

void mklabel(int value)
{
	printf(".L%d:\n", value);
}
/**********************
 * end jump functions
 **********************/