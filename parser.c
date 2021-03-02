/**@<parser.c>::**/

#include <keywords.h>
#include <pseudocode.h>
#include <symtab.h>
#include <parser.h>

/******************************
 * Type token convenction table
 * ----------------------------
 * incomptbl	==	-1
 * void		==	0
 * bool		==	1
 * int32	==	2
 * int64	==	3
 * flt32	==	4
 * flt64	==	5
 ******************************/
enum
{
	INCOMPTBL = -1,
	VOID,
	BOOL,
	INT32,
	INT64,
	FLT32,
	FLT64,
};
#define ILGLTYP -1

/***************************************************************************************
  *	   bool  int32  int64  flt32 flt64    +      -      *      /     OR   AND   NOT
  * bool   bool  -----  -----  ----- -----  -----  -----  -----  -----  bool  bool  bool
  * int32  ----  int32  int64  flt32 flt64  int32  int32  int32  int32  ----  ----  ----
  * int64  ----  int64  int64  flt32 flt64  int64  int64  int64  int64  ----  ----  ----
  * flt32  ----  flt32  flt32  flt32 flt64  flt32  flt32  flt32  flt32  ----  ----  ----
  * flt64  ----  flt64  flt64  flt64 flt64  flt64  flt64  flt64  flt64  ----  ----  ----
  ***************************************************************************************/

int iscompat(int acc_type, int sel_type)
{
	switch (acc_type)
	{
	case VOID:
		return sel_type;
	case BOOL:
		switch (sel_type)
		{
		case BOOL:
			return BOOL;
		default:
			return ILGLTYP;
		}
		break;
	case INT32:
		switch (sel_type)
		{
		case INT32:
		case INT64:
		case FLT32:
		case FLT64:
			return sel_type;
		case '+':
		case '-':
		case '*':
		case '/':
			return INT32;
		default:
			return ILGLTYP;
		}
		break;
	case INT64:
		switch (sel_type)
		{
		case INT32:
		case INT64:
		case '+':
		case '-':
		case '*':
		case '/':
			return INT64;
		case FLT32:
		case FLT64:
			return sel_type;
		default:
			return ILGLTYP;
		}
		break;
	case FLT32:
		switch (sel_type)
		{
		case FLT64:
			return FLT64;
		case BOOL:
			return ILGLTYP;
		default:
			return FLT32;
		}
		break;
	case FLT64:
		switch (sel_type)
		{
		case BOOL:
			return ILGLTYP;
		default:
			return FLT64;
		}
		break;
	default:
		return ILGLTYP;
	}
}

/*****************************************************************************
 * mypas -> PROGRAM ID ; declarative imperative .
 *****************************************************************************/
void mypas(void)
{
	match(PROGRAM);
	match(ID);
	match(';');
	declarative();
	imperative();
	match('.');
}
/*****************************************************************************
 * declarative -> vardecl sbrdecl
 *****************************************************************************/
void declarative(void)
{
	vardecl();
	sbrdecl();
}
/*****************************************************************************
 * vardecl -> [ VAR varlist : typemod ;  { varlist : typemod ; } ]
 *****************************************************************************/
void vardecl(void)
{
	if (lookahead == VAR)
	{
		match(VAR);
		/**/ int initial_pos = symtab_nextentry; /**/
	_varlist_head:
		varlist();
		match(':');
		/**/ int type = /**/ typemod();
		/**/ for (; initial_pos < symtab_nextentry; initial_pos++)
		{
			symtab[initial_pos].type = type;
		} /**/
		match(';');
		if (lookahead == ID)
		{
			goto _varlist_head;
		}
	}
}
/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
/**/ int sem_error = 0; /**/
void varlist(void)
{
_id_head:
	/**/
	if (symtab_append(lexeme) < 0)
	{
		fprintf(stderr, "symtab_append: %s already defined. Fatal error.\n", lexeme);
		sem_error = -1;
	}
	/**/
	match(ID);
	if (lookahead == ',')
	{
		match(',');
		goto _id_head;
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
		/**/ type = INT32;
	case REAL:
		/**/ type = FLT32; /**/
	case DOUBLE:
		/**/ type = FLT64; /**/
		match(lookahead);
		break;
	default:
		/**/ type = BOOL; /**/
		match(BOOLEAN);
	}
	return type;
}
/****************************************************************************
 * sbrdecl -> { procedure | function }
 ****************************************************************************/
void sbrdecl(void)
{
_sbrdecl_head:
	switch (lookahead)
	{
	case PROCEDURE:
		procedure();
		goto _sbrdecl_head;
	case FUNCTION:
		function();
		goto _sbrdecl_head;
		break;
	default:
		/** Emulating empty word **/;
	}
}
/****************************************************************************
 * procedure -> PROCEDURE sbrhead ; sbrtail
 ****************************************************************************/
void procedure(void)
{
	match(PROCEDURE);
	sbrhead();
	match(';');
	sbrtail();
}
/****************************************************************************
 * function -> FUNCTION sbrhead : typemod ; sbrtail
 ****************************************************************************/
void function(void)
{
	match(FUNCTION);
	sbrhead();
	match(':');
	typemod();
	match(';');
	sbrtail();
}
/*****************************************************************************
 * sbrhead -> ID formparm
 *****************************************************************************/
void sbrhead(void)
{
	match(ID);
	formparm();
}
/*****************************************************************************
 * sbrtail -> declarative imperative ;
 *****************************************************************************/
void sbrtail(void)
{
	declarative();
	imperative();
	match(';');
}
/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
void formparm(void)
{
	if (lookahead == '(')
	{
		match('(');
	_parm_head:
		if (lookahead == VAR)
		{
			match(VAR);
		}
		varlist();
		match(':');
		typemod();
		if (lookahead == ';')
		{
			match(';');
			goto _parm_head;
		}
		match(')');
	}
}
/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
void imperative(void)
{
	match(BEGIN);
_stmt_head:
	stmt();
	if (lookahead == ';')
	{
		match(';');
		goto _stmt_head;
	}
}
/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | F | <empty>
 *****************************************************************************/
int is_first_fact(void)
{
	switch (lookahead)
	{
	case '(':
	case ID:
	case UINT:
	case FLOAT:
	case OCT:
	case HEX:
	case '+':
	case '-':
	case NOT:
	case TRUE:
	case FALSE:
		return 1;
	}
	return 0;
}
void stmt(void)
{
	/**/ int F_type = 0; /**/
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
	default:
		if (is_first_fact())
		{
			/**/ F_type = /**/ fact(F_type);
		}
		else
		{
			;
		}
	}
}
/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *****************************************************************************/
/**/ int labelcount = 1; /**/
void ifstmt(void)
{
	/**/ int expr_type;				/**/
	/**/ int elsecount, endifcount; /**/
	match(IF);						/**/
	expr_type = /**/ expr(BOOL);
	match(THEN);
	/**/ printf("\tgofalse .L%d\n", elsecount = endifcount = labelcount++); /**/
	stmt();
	if (lookahead == ELSE)
	{
		match(ELSE);
		/**/ printf("\tgoto .L%d\n", endifcount = labelcount++); /**/
		/**/ printf(".L%d:\n", elsecount);						 /**/
		stmt();
	}
	/**/ printf(".L%d:\n", endifcount); /**/
}
/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
	/**/ int expr_type, whilelbl, whendlbl; /**/
	match(WHILE);
	/**/ printf(".L%d:\n", whilelbl = labelcount++); /**/
	/**/ expr_type = /**/ expr(BOOL);
	match(DO);
	// gofalse()
	/**/ printf("\tgofalse .L%d\n", whendlbl = labelcount++); /**/
	stmt();
	/**/							  //gotolabel(whilelbl) /**/
									  //TODO \/ mklabel(whendlbl)
	/**/ printf(".L%d:\n", whendlbl); /**/
}
/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
	/**/ int expr_type, rptlbl; /**/
	match(REPEAT);
	// mklabel(rptlbl=labelcount++);
stmt_head:
	stmt();
	if (lookahead == ';')
	{
		match(';');
		goto stmt_head;
	}
	match(UNTIL); /**/
	expr_type = /**/ expr(BOOL);
}
/*****************************************************************************
 * expr -> smpexpr [ relop smpexpr ]
 * relop -> "=" | ">=" | "<=" | "<>"
 *****************************************************************************/
int isrelop(void)
{
	switch (lookahead)
	{
	case '=':
	case '<':
	case '>':
	case GEQ:
	case LEQ:
	case NEQ:
		return 1;
	}
	return 0;
}
int expr(int expr_type)
{
	/**/ int smpexpr1_type, smpexpr2_type; /**/
	/**/ smpexpr1_type = /**/ smpexpr(0);
	if (isrelop())
	{
		/**/ smpexpr2_type = /**/ smpexpr(smpexpr1_type);
		return smpexpr2_type;
	}
	else
	{
		;
	}
	return expr_type;
}
/* smpexpr -> ['+''-'] T { (+) T } */
int smpexpr(int smpexpr_type)
{
	/**/ int signal = 0, oplus;
	int T_type; /**/
	if (lookahead == '+' || lookahead == '-')
	{
		/**/ signal = lookahead;							/**/
		/**/ smpexpr_type = iscompat(smpexpr_type, signal); /**/

		match(lookahead);
	}

	/**/ T_type = /**/ term(smpexpr_type);
	/**/ smpexpr_type = iscompat(smpexpr_type, T_type); /**/
	/**/ if (signal == '-')
		negate(smpexpr_type); /**/

	while (lookahead == '+' || lookahead == '-')
	{
		/**/ oplus = lookahead;							   /**/
		/**/ smpexpr_type = iscompat(smpexpr_type, oplus); /**/
		/**/ push(smpexpr_type);						   /**/

		match(lookahead); /**/
		T_type = /**/ term(smpexpr_type);

		/**/ smpexpr_type = iscompat(smpexpr_type, T_type); /**/

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

	/**/ return smpexpr_type; /**/
}

/* T -> F { (*) F } */
int term(int term_type)
{
	/**/ int fact_type; /**/

	/**/ fact_type = /**/ fact(term_type);
	/**/ term_type = iscompat(term_type, fact_type); /**/

	while (lookahead == '*' || lookahead == '/')
	{
		/**/ int otimes = lookahead;				  /**/
		/**/ term_type = iscompat(term_type, otimes); /**/
		/**/ push(term_type);						  /**/

		match(lookahead); /**/
		fact_type = /**/ fact(term_type);

		/**/ term_type = iscompat(term_type, fact_type); /**/

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

	/**/ return term_type; /**/
}

/*  F ->  ( expr )
 *      | n
 *      | v [ = expr ] */
int fact(int fact_type)
{
	/**/ char name[MAXIDLEN + 1];
	int expr_type;
	int var_descriptor; /**/
	switch (lookahead)
	{
	case '(':
		match('('); /**/
		expr_type = /**/ expr(fact_type);
		match(')');
		break;
	case UINT:
		/**/ mov(fact_type, lexeme); /**/
		match(UINT);
		break;
	case FLOAT:
		match(FLOAT);
		break;
	default:
		/**/ strcpy(name, lexeme); /**/
		/**/
		if ((var_descriptor = symtab_lookup(name)) < 0)
		{
			fprintf(stderr, "symtab_lookup: %s not declared\n", name);
			sem_error = -2;
		}
		/**/
		match(ID);
		if (lookahead == ASGN)
		{
			match(ASGN);
			expr(fact_type);
			/**/ L_value(fact_type, name); /**/
		}
		else
		{
			/**/ R_value(fact_type, name); /**/
			;
		}
	}
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
		fprintf(stderr, "token mismatch: expected %d whereas found %d\n",
				expected, lookahead);
		exit(-2);
	}
}
