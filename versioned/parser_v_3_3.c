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

void mybc(void)
{
	E(); match(EOF);
}

void E(void) 
{
	if ( lookahead == '-' ) {
		match ('-');
	}

	_T:

	_F:

	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
		case FLOAT:
		case HEX:
		case OCT:
			match(lookahead);
			break;
		default:
			match(ID);
	}

	if ( lookahead == '*' || lookahead == '/' ) {
		match (lookahead); goto _F;
	}

	if ( lookahead == '+' || lookahead == '-' ) { 
		match (lookahead); goto _T;
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
