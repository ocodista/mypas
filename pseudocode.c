#include <pseudocode.h>
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
 * 
 * STEPS: 
 * 
 * Gets correct accumulador based on var_type
 * Gets correct register based on var_Type
 * POP register(var_type_label)
 * ADD reg + acc
 ************************************************/
void add(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	pop(var_type, reg_label);
	printf("\tadd%s %s, %s\n", type_label, reg_label, acc_label);
}

/******************************************************
 * Generate assembly pseudocode to simulate subtraction
 * 
 * Between acc(var_type_label) and reg(var_type_label)
 * Where var_type_label varies according with var_type
 ******************************************************/
void subtract(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	mov(var_type, acc_label, reg_label);
	pop(var_type, acc_label);
	printf("\tsub%s %s, %s\n", type_label, reg_label, acc_label);
}

/******************************************************
 * Generate assembly pseudocode to simulate multiplication
 * 
 * Between acc(var_type_label) and reg(var_type_label)
 * Where var_type_label varies according with var_type
 ******************************************************/
void multiply(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);

	pop(var_type, reg_label);
	printf("\tmul%s %s, %s\n", type_label, reg_label, acc_label);
}

/******************************************************
 * Generate assembly pseudocode to simulate division
 * 
 * Between acc(var_type_label) and reg(var_type_label)
 * Where type_label varies according with var_type
 ******************************************************/
void divide(int var_type)
{
	char *acc_label = get_var_label(var_type, "acc");
	char *reg_label = get_var_label(var_type, "reg");
	char *type_label = get_var_type_id(var_type);
	mov(var_type, acc_label, reg_label);
	pop(var_type, acc_label);
	printf("\tdiv%s %s, %s\n", type_label, reg_label, acc_label);
}

/***********************************************************
 * Generate assembly pseudocode to simulate function negate
 * 
 * Where type_label varies according with var_type
 ******************************************************/
void negate(int var_type)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tnegate%s acc%s\n", type_label, type_label);
}

/********************************************************
 * Generate assembly pseudocode to simulate pop of stack
 * 
 * Gets the type_label to switch pop and uses address to 
 * pop from stack
 *******************************************************/
void pop(int var_type, char *address)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tpop%s %s\n", type_label, address);
}

/********************************************************
 * Generate assembly pseudocode to simulate psuh of stack
 * 
 * Gets the type_label to switch push and uses address to 
 * pop from stack
 *******************************************************/
void push(int var_type, char *address)
{
	char *type_label = get_var_type_id(var_type);
	printf("\tpush%s %s\n", type_label, address);
}

/********************************************************
 * Generate assembly pseudocode of function mov  
 *******************************************************/
void mov(int var_type, const char *dest, const char *src)
{
	char *typelabel = get_var_type_id(var_type);
	printf("\tmov%s %s, %s\n", typelabel, dest, src);
}

/********************************************************
 * Generate assembly pseudocode of function cmp
 * 
 * Function used varies according with relop
 * 
 * > - above(var_type_label)
 * < - below(var_type_label)
 * = - cmp(var_type_label)
 *******************************************************/
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
// Pseudocode of gofalse functions
void gofalse(int value)
{
	printf("\tgofalse .L%d\n", value);
}

// Pseudocode of goto function
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
