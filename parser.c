/**@<parser.c>::**/
#include <parser.h>

/***************************
 * Variables declaration
 ***************************/

// Buffer used to write messages
char message[100];

// Used to map lexical level of current execution
/**/ int lexical_level = 0; /**/

// Explanation for valid values for next variables can be found at enums.h
/**/ int objtype;	  /**/
/**/ int transp_type; /**/

// Counter for semantic errors
/**/ int semantic_error = 0; /**/

/*********************************
 * End of declaration of variables 
 *********************************/

/**********************************
 * Internal subroutines
 **********************************/

/*****************************************************************************
 * mypas -> PROGRAM ID ; declarative imperative .
 *****************************************************************************/
void mypas(void)
{
	match(PROGRAM);
	match(ID);
	match(SEMICOLON);
	/**/ lexical_level++; /**/
	declarative();
	imperative();
	/**/ lexical_level--; /**/
	match(DOT);
}
/*****************************************************************************
 * declarative -> vardecl sbpdecl
 *****************************************************************************/
void declarative(void)
{
	vardecl();
	sbpdecl();
}

/***************************************************************************************************
 * This function is used to map the compatibility between two types of variable
 * 
 * iscompat table: 
 *        bool   int32   int64   flt32   flt64     '+'     '-'     '*'     '/'    NOT    OR    AND
 * bool   bool   -----   -----   -----   -----    -----   -----   -----   -----  bool   bool   bool
 * int32  ----   int32   int64   flt32   flt64    int32   int32   int32   int32  ----   ----   ----
 * int64  ----   int64   int64   flt32   flt64    int64   int64   int64   int64  ----   ----   ----
 * flt32  ----   flt32   flt32   flt32   flt64    flt32   flt32   flt32   flt32  ----   ----   ----
 * flt64  ----   flt64   flt64   flt64   flt64    flt64   flt64   flt64   flt64  ----   ----   ----
 * 
 ***************************************************************************************************/
int iscompat(int acc_type, int syn_type)
{
	switch (acc_type)
	{
	case VOID:
		return syn_type;
	case BOOL:
		if (syn_type == AND || syn_type == OR || syn_type == NOT || syn_type == BOOL)
			return BOOL;
		break;
	case INT32:
		if (syn_type >= INT32 && syn_type <= FLT64)
			return syn_type;
		break;
	case FLT32:
		if (syn_type > FLT32 && syn_type <= FLT64)
			return syn_type;
		if (syn_type == INT32)
			return FLT32;
		break;
	case FLT64:
		if (syn_type >= INT32 && syn_type <= FLT64)
			return FLT64;
	}
	if (acc_type >= INT32 && acc_type <= FLT64)
	{
		switch (syn_type)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case DIV:
		case MOD:
			return acc_type;
		}
	}
	return INCOMPTBL;
}

/*****************************************************************************
 * vardecl ->  [ VAR varlist : typemod ; { varlist : typemod ; } ]
 *****************************************************************************/
void vardecl(void)
{
	if (lookahead == VAR)
	{
		match(VAR);
		/**/ int first_pos;							/**/
		/**/ objtype = transp_type = LOCAL_STORAGE; /**/
	_varlist_head:
		/**/ first_pos = symtab_next_entry; /**/
		varlist();
		match(COLON);
		/**/ int type = /**/ typemod();
		/**/ symtab_update_type(first_pos, type); /**/
		match(SEMICOLON);
		if (lookahead == ID)
		{
			goto _varlist_head;
		}
	}
	else
	{
		;
	}
}

/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
void varlist(void)
{
_match_id_head:
	/**/ symtab_append(lexeme, lexical_level, objtype, transp_type); /**/
	match(ID);
	if (lookahead == COMMA)
	{
		match(COMMA);
		goto _match_id_head;
	}
}
/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
int typemod(void)
{
	/**/ int type; /**/
	switch (lookahead)
	{
	case INTEGER:
		/**/ type = INT32; /**/
		break;
	case REAL:
		/**/ type = FLT32; /**/
		break;
	case DOUBLE:
		/**/ type = FLT64; /**/
		break;
	default:
		/**/ type = BOOL; /**/
	}
	match(lookahead);
	return type;
}

/*****************************************************************************
 * sbpdecl -> { PROCEDURE sbphead ; sbptail | FUNCTION sbphead : typemod ; sbptail }
 *****************************************************************************/
void sbpdecl(void)
{
	/**/ int isfunc = 0;			 /**/
	/**/ char sbpname[MAXIDLEN + 1]; /**/
	/**/ int symtab_sentinel;		 /**/
_switch_head:
	switch (lookahead)
	{
	case FUNCTION:
		isfunc = 1;
	case PROCEDURE:
		if (isfunc)
			objtype = OT_FUNCTION;
		else
			objtype = OT_PROCEDURE;

		match(lookahead);
		/**/ strcpy(sbpname, lexeme);												   /**/
		/**/ int sbppos = symtab_append(sbpname, lexical_level, objtype, transp_type); /**/
		match(ID);
		/**/ symtab_sentinel = symtab_next_entry; /**/
		/**/ lexical_level++;					  /**/
		formparm();
		if (isfunc)
		{
			/**/ isfunc = 0; /**/
			match(COLON);
			/**/ int rettype = /**/ typemod();
			/**/ symtab[sbppos].type = rettype; /**/
		}
		else
		{
			/**/ symtab[sbppos].type = VOID; /**/
		}
		match(SEMICOLON);
		declarative();
		imperative();
		/**/ lexical_level--;					  /**/
		/**/ symtab_next_entry = symtab_sentinel; /**/
		match(SEMICOLON);
		if (lookahead == FUNCTION || lookahead == PROCEDURE)
		{
			goto _switch_head;
		}
	default:;
	}
}
/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
void formparm(void)
{
	if (lookahead == OPEN_PARENTHESES)
	{
		match(OPEN_PARENTHESES);
		/**/ int first_pos;			/**/
		/**/ objtype = OT_VARIABLE; /**/
	parm_list:
		/**/ first_pos = symtab_next_entry; /**/
		if (lookahead == VAR)
		{
			match(VAR);
			/**/ transp_type = PASS_BY_REF; /**/
		}
		else
		{
			/**/ transp_type = PASS_BY_VALUE; /**/
		}
		varlist();
		match(COLON);
		/**/ int type = /**/ typemod();
		/**/ symtab_update_type(first_pos, type); /**/
		if (lookahead == SEMICOLON)
		{
			match(SEMICOLON);
			goto parm_list;
		}
		match(CLOSE_PARENTHESES);
	}
	else
	{
		;
	}
}

/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
void imperative(void)
{
	match(BEGIN);
stmt_list:
	stmt();
	if (lookahead == SEMICOLON)
	{
		match(SEMICOLON);
		goto stmt_list;
	}
	match(END);
}

/*****************************************************************************
 * rtrn -> RETURN expr
 *****************************************************************************/
void rtrn(void)
{
	match(RETURN);
	expr(VOID);
}

/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | fact | <empty>
 *****************************************************************************/
void stmt(void)
{
	/**/ int fact_type; /**/
	switch (lookahead)
	{
	case BEGIN:
		imperative();
		break;
	case IF:
		ifstmt();
		break;
	case WHILE:
		whlstmt();
		break;
	case REPEAT:
		rptstmt();
		break;
	case RETURN:
		rtrn();
		break;
	case OPEN_PARENTHESES:
	case ID:
	case BOOL:
	case UINT:
	case FLOAT:
		/**/ fact_type = /**/ fact(VOID);
		break;
	default:;
	}
}

/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *****************************************************************************/
/**/ int loop_count = 1; /**/
void ifstmt(void)
{
	/**/ int expr_type, else_count, endif_count; /**/
	match(IF);
	/**/ expr_type = /**/ expr(BOOL);
	match(THEN);
	/**/ gofalse(else_count = endif_count = loop_count++); /**/
	stmt();
	if (lookahead == ELSE)
	{
		match(ELSE);
		/**/
		golabel(endif_count = loop_count++);
		mklabel(else_count);
		/**/
		stmt();
	}
	/**/ mklabel(endif_count); /**/
}

/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
	/**/ int expr_type, whlhead, whltail; /**/
	match(WHILE);
	/**/ mklabel(whlhead = loop_count++); /**/
	/**/ expr_type = /**/ expr(BOOL);
	match(DO);
	/**/ gofalse(whltail = loop_count++); /**/
	stmt();
	/**/ golabel(whlhead); /**/
	/**/ mklabel(whltail); /**/
}

/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
	/**/ int expr_type;
	int replbl;
	mklabel(replbl = loop_count++); /**/
stmt_head:
	stmt();
	if (lookahead == SEMICOLON)
	{
		match(SEMICOLON);
		goto stmt_head;
	}
	match(UNTIL);
	/**/ expr_type = /**/ expr(BOOL);
	/**/ gofalse(replbl); /**/
}

/**************************************
 * isrelop -> = | > | < | >= | <> | <=
 **************************************/
int isrelop(void)
{
	switch (lookahead)
	{
	case '=':
	case '>':
	case '<':
	case GEQ:
	case NEQ:
	case LEQ:
		return lookahead;
	}
	return 0;
}

/**************************************
 * expr -> smpexprt [ relop smpexpr ]
 **************************************/
int expr(int expr_type)
{
	/**/ int left_type = /**/ smpexpr(VOID);

	if (isrelop())
	{
		expr_type = do_relop(expr_type, left_type);
	}
	else
	{
		/**/ expr_type = iscompat(expr_type, left_type); /**/
	}
	/**/ return expr_type; /**/
}

/***********************************************
 * Validates if relop is valid
 * 
 * Add left side to correct accumulator
 * Uses comp function from pseudocode to compare
 ************************************************/
int do_relop(int expr_type, int left_type)
{
	/**/ int relop, right_type; /**/

	// Identifies relop operator
	/**/ relop = lookahead; /**/

	// Here, we call get_var_label passing the smpexpr_type to
	// Get the correct accumulator and aux, according to var type
	/**/ char *acc = get_var_label(left_type, "acc"); /**/
	/**/ char *aux = get_var_label(left_type, "aux"); /**/

	// Set save left_type value to accumulator
	/**/ mov(left_type, acc, aux); /**/

	// If expression_type is not boolean(is not a condition/validation)
	// then set expr_type to incompatible;
	/**/ if (expr_type != BOOL)
		expr_type = INCOMPTBL; /**/

	match(lookahead);

	/**/ right_type = /**/ smpexpr(left_type);
	/**/ left_type = iscompat(left_type, right_type); /**/
	/**/
	if (left_type != VOID)
	{
		cmp(relop, left_type, "aux", "acc");
		return expr_type;
	}
	else
	{
		show_error("Left side of comparation cannot be null");
	}
	/**/

	return expr_type;
}

/****************************************
 * smpexpr -> ['+''-'] term { (+) term } 
 ****************************************/
int smpexpr(int smpexpr_type)
{
	/**/ int signal = 0; /**/
	if (lookahead == '+' || lookahead == '-' || lookahead == NOT)
	{
		/**/ signal = lookahead;							 /**/
		/***/ smpexpr_type = iscompat(smpexpr_type, signal); /***/

		match(lookahead);
	}

	/***/ int term_type = /***/ term(smpexpr_type);	  /**/
	smpexpr_type = iscompat(smpexpr_type, term_type); /**/
	/**/ if (signal == '-' || signal == NOT)
		negate(smpexpr_type);								/**/
	/***/ smpexpr_type = iscompat(smpexpr_type, term_type); /***/
	/**/ char *acc_label;									/**/
	while (lookahead == '+' || lookahead == '-' || lookahead == OR)
	{
		/**/ int oplus = lookahead;							/**/
		/***/ smpexpr_type = iscompat(smpexpr_type, oplus); /***/
		acc_label = get_var_label(smpexpr_type, "acc");
		/**/ push(smpexpr_type, acc_label); /**/

		match(lookahead); /***/
		term_type = /***/ term(smpexpr_type);

		/***/ smpexpr_type = iscompat(smpexpr_type, term_type); /***/

		// Here, we call get_var_label passing the smpexpr_type to
		// Get the correct accumulator and aux, according to var type
		/**/ char *acc = get_var_label(smpexpr_type, "acc"); /**/
		/**/ char *aux = get_var_label(smpexpr_type, "aux"); /**/
		/**/ mov(smpexpr_type, acc, aux);					 /**/

		/**/
		switch (oplus)
		{
		case '+':
			add(smpexpr_type);
			break;
		default:
			subtract(smpexpr_type);
		}
		/**/
	}

	/***/ return smpexpr_type; /***/
}

/**************************************
 * term -> fact { (*) fact } 
 **************************************/
int term(int term_type)
{
	/***/
	int fact_type = /***/ fact(term_type);
	char *acc_label, *aux_label;

	// Check compatibility with next term
	/**/ term_type = iscompat(term_type, fact_type); /**/

	// Realize math operations between terms
	while (lookahead == '*' || lookahead == '/' || lookahead == DIV | lookahead == MOD | lookahead == AND)
	{
		/**/ int otimes = lookahead;					  /**/
		/***/ term_type = iscompat(term_type, otimes);	  /***/
		/**/ acc_label = get_var_label(term_type, "acc"); /**/
		/**/ push(term_type, acc_label);				  /**/

		match(lookahead);

		// Calculates value of next fact
		/***/ fact_type = /***/ fact(term_type);

		// Gets compatible type from returned fact
		/***/ term_type = iscompat(term_type, fact_type); /***/

		// Here, we call get_var_label passing the term_type to
		// Get the correct accumulator and aux, according to var type

		/**/ acc_label = get_var_label(term_type, "acc"); /**/
		/**/ aux_label = get_var_label(term_type, "aux"); /**/
		/**/ mov(term_type, acc_label, aux_label);		  /**/

		/**/
		switch (otimes)
		{
		case '*':
			multiply(term_type);
			break;
		default:
			divide(term_type);
		}
		/**/
	}

	/***/ return term_type; /***/
}

/**************************************************
 * FACT ->  ( expr ) | n | v [ = expr ]
 * 
 * This function handles:
 * - The call of an expression returning its value
 * - The value of a variable
 * - An assigment 
 **************************************************/
int fact(int fact_type)
{
	/**/ char name[MAXIDLEN + 1]; /**/
	/***/ int expr_type;		  /***/
	switch (lookahead)
	{
	case OPEN_PARENTHESES:
		match(OPEN_PARENTHESES);

		/***/ expr_type = /***/ expr(fact_type);
		match(CLOSE_PARENTHESES);
		/***/ fact_type = iscompat(fact_type, expr_type); /***/
		break;

	// Gets compatible type and store into acc
	// BOOL -> accb
	// UINT -> accl
	// FLT32 -> accf
	case BOOL:
		fact_type = mov_var_type(fact_type, BOOL);
		match(BOOL);
		break;
	case UINT:
		fact_type = mov_var_type(fact_type, INT32);
		match(UINT);
		break;
	case FLOAT:
		fact_type = mov_var_type(fact_type, FLT32);
		match(FLOAT);
		break;
	default:

		/**/ strcpy(name, lexeme); /**/

		match(ID);
		if (lookahead == ASGN)
		{
			/**** L-Value ****/
			/**/ fact_type = validate_l_value(expr_type, fact_type, name); /**/
		}
		else
		{
			/**** R-Value ****/
			/**/ fact_type = validate_r_value(fact_type, name); /**/
		}
	}
	return fact_type;
}
/**************************************************************************
 * Once identified that next token is ASGN, validates if left-side of 
 * expression is valid, checking if entry was already declared at symtab
 * and checking if object type is assignable 
 * 
 * Returns 'parsed' fact_type
 ****************************************************************************/
int validate_l_value(int expr_type, int fact_type, char *name)
{
	// Labels will be used to identify correct accumulator and object type
	/**/ char *acc_label; /**/
	/**/ char *ot_label;  /**/

	match(ASGN);

	// Identifies expression type
	/***/ expr_type = /***/ expr(fact_type);

	// Checks if symbol table has any entry where symbol == name
	if (symtab_lookup(name) < 0)
	{
		// Cant assign an undeclared entry
		snprintf(message, sizeof(message), "%s was not declared", name);
		semantic_error++;
		show_error(message);
	}
	else
	{
		// Validates if entry is variable
		if (symtab[symtab_entry].objtype != OT_VARIABLE)
		{
			// Gets label from object type (Function or Parameter)
			ot_label = get_object_type_label(symtab[symtab_entry].objtype);

			// Throws error because function/parameter shouldn't exist on left side of assignment
			snprintf(message, sizeof(message), "%s %s cannot be used as variable (left-assign)", ot_label, name);
			show_error(message);

			semantic_error++;
		}
		else
		{
			// Assign value to declared variable located at symtab[symtab_entry]
			fact_type = mov_compat_type(expr_type);
		}
	}
	return fact_type;
}

/*******************************************************************************
 * In this function, the goal is to identify object type  as variable, function
 * or parameter, validate if entry exists on system table and call pseudocode 
 * mov to represent return of value
 * 
 * Returns 'parsed' fact_type
 *******************************************************************************/
int validate_r_value(int fact_type, char *name)
{
	// Account label is used to identify accumulator according with fact_type
	/**/ char *acc_label; /**/

	// Checks if entry exists on system table
	if (symtab_lookup(name) < 0)
	{
		// Cannot use undeclared id
		snprintf(message, sizeof(message), "%s was not declared 2", name);
		show_error(message);
		semantic_error++;
	}
	else
	{
		switch (symtab[symtab_entry].objtype)
		{
		case OT_VARIABLE:
			// Set value of fact_type into accumulator
			fact_type = mov_compat_type(fact_type);
			break;

		case OT_PROCEDURE:
			// TODO: Implement call of procedure
			break;

		case OT_FUNCTION:
			if (lookahead == OPEN_PARENTHESES)
			{
				match(OPEN_PARENTHESES);
			_expr_list:
				expr(VOID);
				if (lookahead == COMMA)
				{
					match(COMMA);
					goto _expr_list;
				}
				match(CLOSE_PARENTHESES);
			}

			fact_type = mov_compat_type(fact_type);
			return fact_type;
			break;
		}
	}
	return fact_type;
}

/**************************************************************************
 * This function is used to parse the obj_type variable using the 
 * iscompat table, then calling mov function from pseudocode.c to simulate 
 * the store of symtab[symtab_entry] value into correct accumulator
 * 
 * Returns 'parsed' fact_type
 ****************************************************************************/
int mov_compat_type(int obj_type)
{
	// Get compatible type from iscompat table
	/**/ int compat_type = iscompat(obj_type, symtab[symtab_entry].type); /**/

	// Get the correct accumulator according to var type
	/**/ char *acc_label = get_var_label(compat_type, "acc"); /**/

	/*** Value from compat_type is set to entry[offset] ***/
	mov(compat_type, acc_label, symtab[symtab_entry].offset);

	return compat_type;
}

/****************************************************************************
 * This function translate var_type into compatible type from iscompat table 
 * and prints mov command setting fact_type value to correct accumulator
 * 
 * Returns 'parsed' fact_type
 ****************************************************************************/
int mov_var_type(int fact_type, int var_type)
{
	char *acc_label;

	// Gets compatible type from iscompat table
	/***/ fact_type = iscompat(fact_type, var_type); /***/

	// Get the correct accumulator according to var type
	/**/ acc_label = get_var_label(fact_type, "acc"); /**/

	// Calls pseucode to represent mov command
	/**/ mov(fact_type, lexeme, acc_label); /**/

	return fact_type;
}

void match(int expected)
{
	if (lookahead == expected)
	{
		lookahead = gettoken(source);
	}
	else
	{
		snprintf(message, sizeof(message), "Token mismatch: expected %d whereas found %d", expected, lookahead);
		show_error(message);
	}
}