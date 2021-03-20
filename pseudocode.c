#include <pseudocode.h>
//TODO: FINISH COMMENTING THIS

/**********************************************
 * Function to retrieve value from accumulator
 * And store into var "name"
 * 
 * NOTE: Each type has its own accumulator
 **********************************************/
void R_value(int var_type, const char *name)
{
	char *acc_label = get_var_label(var_type, "acc");
	mov(var_type, name, acc_label);
}

/***********************************************
 * Function that retrieves value from var "name"
 * And store into accumulator
 * 
 * NOTE: Each type has its own accumulator
 **********************************************/
void L_value(int var_type, const char *name)
{
	char *acc_label = get_var_label(var_type, "acc");
	mov(var_type, acc_label, name);
}

/************************************************
 * Generate assembly pseudocode for function add
 ************************************************/
void add(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	pop(var_type, reg_label);
	printf("\tadd%s %s, %s\n", type_label, reg_label, acc_label);
}

void subtract(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	mov(var_type, acc_label, reg_label);
	pop(var_type, acc_label);
	printf("\tsub%s %s, %s\n", type_label, reg_label, acc_label);
}

void multiply(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);

	pop(var_type, reg_label);
	printf("\tmul%s %s, %s\n", type_label, reg_label, acc_label);
}

void divide(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	mov(var_type, acc_label, reg_label);
	pop(var_type, acc_label);
	printf("\tdiv%s %s, %s\n", type_label, reg_label, acc_label);
}

void negate(int var_type)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tnegate%s acc%s\n", type_label, type_label);
}

void pop(int var_type, char *address)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tpop%s %s\n", type_label, address);
}

void push(int var_type, char *address)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tpush%s %s\n", type_label, address);
}

void mov(int var_type, const char *dest, const char *src)
{
	char *typelabel = get_var_type_id(var_type);
	printf("\tmov%s %s, %s\n", typelabel, dest, src);
}

void cmp(int relop, int var_type, char *left_operator, char *right_operator)
{
	char *type_label = get_var_type_id(var_type);
	char *aux_label = get_var_label(var_type, left_operator);
	char *acc_label = get_var_label(var_type, right_operator);
	switch (relop)
	{
	case '>':
		printf("\tabove%s %s, %s\n", type_label, left_operator, right_operator);
		break;
	case '<':
		printf("\tbelow%s %s, %s\n", type_label, left_operator, right_operator);
		break;
	case '=':
		printf("\tcmp%s %s, %s\n", type_label, left_operator, right_operator);
		break;
	}
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

// Create label to be jumped
void mklabel(int value)
{
	printf(".L%d:\n", value);
}
/**********************
 * end jump functions
 **********************/
