/**@<parser.c>::**/

/*
 * simplified recursive LL(1) parser for expressions.
 *
 * LL(1)-grammar:
 *
 * E -> T R
 * R -> oplus T R | <>
 * R =>* (oplus T)*
 * The above notation is not valid in grammar context
 * In this case, it is introduced the EBNF expansion
 * R -> { T }  -- this a Kleene closure of the sentencial form T
 *
 * E -> T { T }
 *
 * void R(void) {
 * 	if (lookahead == '+' || lookahead == '-') {
 * 		match (lookahead); T(); R();
 * 	} else {
 * 		;
 * 	}
 * }
 * T -> F Q
 * Q -> otimes F Q | <>
 * F -> (E) | UINT | ID
 *
 * oplus = '+' | '-'
 * otimes = '*' | '/'
 * UINT = [1-9][0-9]* | 0
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 */

#include <parser.h>

/* E -> T { oplus T } */
void E(void) 
{
	_T:
	T(); 
	if ( lookahead == '+' || lookahead == '-' ) { 
		match (lookahead); goto _T;
	}
}

/* T -> F { otimes F } */
void T(void)
{ 
	/** FOLLOW(T) = FIRST(R) - {<>} \cup FOLLOW(E)
	 *            = { '+', '-', EOF, ')' } 
	 *            = FOLLOW(Q) **/
	_F:
	F(); 
	if ( lookahead == '*' || lookahead == '/' ) {
		match (lookahead); goto _F;
	}
}

void F(void) 
{ 
	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
		case HEX:
		case OCT:
			match(lookahead);
			break;
		default:
			match(ID);
	}
}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch: expected %d whereas found %d\n",
		expected, lookahead);
		exit(-2);
	}
}
